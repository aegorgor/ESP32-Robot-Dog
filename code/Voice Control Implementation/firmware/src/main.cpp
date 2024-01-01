#include <Arduino.h>
#include <WiFi.h>
#include <driver/i2s.h>
#include <esp_task_wdt.h>
#include "I2SMicSampler.h"
#include "ADCSampler.h"
#include "config.h"
#include "CommandDetector.h"
#include "CommandProcessor.h"
#include "Legs_and_servos.h"
#include <Wire.h>

#include "gaits.h"


RServo aServo0(1, 0, 1.5778, 98);
RServo fServo0(0, 2, 2.4079, 45);
RServo tServo0(0, 1, 2.4079, 11);

RServo aServo1(0, 4, 1.5778, 90.6);
RServo fServo1(0, 6, 2.4079, 40);
RServo tServo1(0, 5, 2.34, 10);

RServo aServo2(1, 8, 1.5778, 91.3);
RServo fServo2(1, 10, 2.3544, 49);
RServo tServo2(1, 9, 2.41, 5);

RServo aServo3(0, 12, 1.6333, 94); //1.6333 ratio originally
RServo fServo3(1, 14, 2.3276, 46);
RServo tServo3(1, 13, 2.35, 5);

Leg leg0(&aServo0, &fServo0, &tServo0);
Leg leg1(&aServo1, &fServo1, &tServo1);
Leg leg2(&aServo2, &fServo2, &tServo2);
Leg leg3(&aServo3, &fServo3, &tServo3);

// i2s config for using the internal ADC
i2s_config_t adcI2SConfig = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S_LSB, //I2S_COMM_FORMAT_STAND_I2S
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 64,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0};

// i2s config for reading from both channels of I2S
i2s_config_t i2sMemsConfigBothChannels = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_MIC_CHANNEL,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S), //I2S_COMM_FORMAT_STAND_I2S
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 64,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0};

// i2s microphone pins
i2s_pin_config_t i2s_mic_pins = {
    .bck_io_num = I2S_MIC_SERIAL_CLOCK,
    .ws_io_num = I2S_MIC_LEFT_RIGHT_CLOCK,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_MIC_SERIAL_DATA};

// This task does all the heavy lifting for our application
void applicationTask(void *param)
{
  CommandDetector *commandDetector = static_cast<CommandDetector *>(param);

  const TickType_t xMaxBlockTime = pdMS_TO_TICKS(100);
  while (true)
  {
    // wait for some audio samples to arrive
    uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE, xMaxBlockTime);
    if (ulNotificationValue > 0)
    {
      commandDetector->run();
    }
  }
}

void setup()
{
  pca9685.begin();
  pca9685.setPWMFreq(50);
  Serial.begin(9600);
  leg0.setZ(170);
  leg1.setZ(170);
  leg2.setZ(170);
  leg3.setZ(170);
  gaits::interpolate("0123", 500000);
  
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting up");

  // make sure we don't get killed for our long running tasks
  esp_task_wdt_init(10, false);

  // start up the I2S input (from either an I2S microphone or Analogue microphone via the ADC)
#ifdef USE_I2S_MIC_INPUT
  // Direct i2s input from INMP441 or the SPH0645
  I2SSampler *i2s_sampler = new I2SMicSampler(i2s_mic_pins, false);
#else
  // Use the internal ADC
  I2SSampler *i2s_sampler = new ADCSampler(ADC_UNIT_1, ADC_MIC_CHANNEL);
#endif
  // the command processor
  CommandProcessor *command_processor = new CommandProcessor();

  // create our application
  CommandDetector *commandDetector = new CommandDetector(i2s_sampler, command_processor);

  // set up the i2s sample writer task
  TaskHandle_t applicationTaskHandle;
  xTaskCreatePinnedToCore(applicationTask, "Command Detect", 8192, commandDetector, 1, &applicationTaskHandle, 0);

  // start sampling from i2s device - use I2S_NUM_0 as that's the one that supports the internal ADC
#ifdef USE_I2S_MIC_INPUT
  i2s_sampler->start(I2S_NUM_0, i2sMemsConfigBothChannels, applicationTaskHandle);
#else
  i2s_sampler->start(I2S_NUM_0, adcI2SConfig, applicationTaskHandle);
#endif
}

void loop()
{
  //vTaskDelay(1);
  vTaskDelay(pdMS_TO_TICKS(1000));
}