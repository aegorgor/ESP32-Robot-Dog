#include <Arduino.h>
#include "CommandProcessor.h"
#include "gaits.h"


const char *words[] = {
    "forward",
    "backward",
    "left",
    "right",
    "_nonsense",
};

void commandQueueProcessorTask(void *param)
{
    CommandProcessor *commandProcessor = (CommandProcessor *)param;
    while (true)
    {
        uint16_t commandIndex = 0;
        if (xQueueReceive(commandProcessor->m_command_queue_handle, &commandIndex, portMAX_DELAY) == pdTRUE)
        {
            commandProcessor->processCommand(commandIndex);
        }
    }
}

void CommandProcessor::processCommand(uint16_t commandIndex)
{
    newCommand = false;
    switch (commandIndex)
    {
    case 0: // forward
        while(newCommand == false)
            gaits::straightWalkingSequence(walkCycle);
        vTaskDelay(1000 / portTICK_PERIOD_MS);  //does this need to be removed?
        break;
    case 1: // backward
        while(newCommand == false)
            gaits::straightWalkingSequenceRear(walkCycle);
        Serial.println("bwd task");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        break;
    case 2: // left
        while(newCommand == false)
            gaits::turningWalkingSequenceCounterClockwise(walkCycle);
        Serial.println("left task");
        vTaskDelay(500 / portTICK_PERIOD_MS);
        break;
    case 3: // right
        while(newCommand == false)
            gaits::turningWalkingSequenceClockwise(walkCycle);
        Serial.println("right task");
        vTaskDelay(500 / portTICK_PERIOD_MS);
        break;
    }

}

CommandProcessor::CommandProcessor()
{
    // allow up to 5 commands to be in flight at once
    m_command_queue_handle = xQueueCreate(5, sizeof(uint16_t));
    if (!m_command_queue_handle)
    {
        Serial.println("Failed to create command queue");
    }
    // kick off the command processor task
    TaskHandle_t command_queue_task_handle;
    xTaskCreate(commandQueueProcessorTask, "Command Queue Processor", 8192, this, 1, &command_queue_task_handle); //1024
}

void CommandProcessor::queueCommand(uint16_t commandIndex, float best_score)
{
    // unsigned long now = millis();
    if (commandIndex != 5 && commandIndex != -1)
    {
        Serial.printf("***** %ld Detected command %s(%f)\n", millis(), words[commandIndex], best_score);
        if (xQueueSendToBack(m_command_queue_handle, &commandIndex, 0) != pdTRUE)
        {
            Serial.println("No more space for command");
        }
    }
}
