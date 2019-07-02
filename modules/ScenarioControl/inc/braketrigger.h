#ifndef BRAKETRIGGER_H
#define BRAKETRIGGER_H

#include "trigger.h"

class BrakeTrigger : public Trigger
{
public:
    //BrakeTrigger(TriggerType_t triggerID);
    ~BrakeTrigger();

    TriggerType_t getType() { return TRIGGER_BRAKE; }

    TriggerReturnCode_t appendTriggerParameter(uint32_t triggerParameter);
};

#endif // BRAKETRIGGER_H
