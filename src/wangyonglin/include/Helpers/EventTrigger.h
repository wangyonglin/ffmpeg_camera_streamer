/*
  Trigger *trigger = TriggerRegister();
  TriggerEvent * triggerEvent =TriggerEventRegister(trigger);
  TriggerDispatch(trigger);
  
  TriggerEventLogout(triggerEvent);
  TriggerLogout(trigger);
  */
#if !defined(INCLUDE_UWANG_EVENTTRIGGER_H)
#define INCLUDE_UWANG_EVENTTRIGGER_H
#include <Helpers/String.h>

typedef struct __Trigger_t
{
  struct event_base *base;
} Trigger, *pTrigger;

typedef struct
{
  Trigger *trigger;
  struct event *event;
} TriggerEvent;

Trigger *TriggerRegister();
Exception TriggerDispatch(Trigger *trigger);
void TriggerLogout(Trigger *trigger);


TriggerEvent *TriggerEventRegister(Trigger *trigger);
void TriggerEventLogout(TriggerEvent *triggerEvent);

#endif