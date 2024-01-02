#ifndef _intent_processor_h_
#define _intent_processor_h_

#include <list>
class StepperMotor;
class Servo;

class CommandProcessor
{
private:
    QueueHandle_t m_command_queue_handle;
    void processCommand(uint16_t commandIndex);
    int walkCycles[3] = {700000, 500000, 300000};
    int speed = 0;
    int lastCommandIndex;
public:
    CommandProcessor();
    void queueCommand(uint16_t commandIndex, float score);
    friend void commandQueueProcessorTask(void *param);
    bool newCommand = false;
};

#endif
