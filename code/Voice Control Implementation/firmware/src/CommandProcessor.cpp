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
    int walkCycle = walkCycles[0];
    switch (commandIndex)
    {
    case 0:
        if(speed < 3) speed++;
        switch (abs(speed))
        {
            case 1: walkCycle = walkCycles[0]; break;
            case 2: walkCycle = walkCycles[1]; break;
            case 3: walkCycle = walkCycles[2]; break;
        }
        if(speed < 0){
            while(newCommand == false){
                Serial.println("fwd task");
                gaits::straightWalkingSequenceRear(walkCycle);
            }
        }
        else if(speed > 0){
            while(newCommand == false){
                Serial.println("fwd task");
                gaits::straightWalkingSequence(walkCycle);
            }
        }
        break;
    case 1:
        if(speed > -3) speed--;
        switch (abs(speed))
        {
            case 1: walkCycle = walkCycles[0]; break;
            case 2: walkCycle = walkCycles[1]; break;
            case 3: walkCycle = walkCycles[2]; break;
        }
        if(speed < 0){
            while(newCommand == false){
                Serial.println("fwd task");
                gaits::straightWalkingSequenceRear(walkCycle);
            }
        }
        else if(speed > 0){
            while(newCommand == false){
                Serial.println("fwd task");
                gaits::straightWalkingSequence(walkCycle);
            }
        }
        break;
    case 2:
        if(lastCommandIndex != 3)
        {
            while(newCommand == false)
                gaits::turningWalkingSequenceCounterClockwise(walkCycles[1]);
        }
        Serial.println("left task");
        break;
    case 3: // right
        if(lastCommandIndex != 2)
        {
            while(newCommand == false)
                gaits::turningWalkingSequenceClockwise(walkCycles[1]);
        }
        Serial.println("right task");
        break;
    }
    lastCommandIndex = commandIndex;
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
