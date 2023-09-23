#include <Blackliner/EventTrigger.h>

void __EventTriggerExit(int fd, short event, void *arg)
{
    // 打印消息并退出事件循环
      TriggerEvent *triggerEvent = (TriggerEvent *)(arg);
    if (triggerEvent->trigger->base)
    {
        event_base_loopexit(triggerEvent->trigger->base, NULL);
    }
}
Trigger *TriggerRegister()
{
    Trigger *trigger = NULL;
    if (SkeletonAllocate((void **)&trigger, sizeof(Trigger)))
    {
        if (!(trigger->base = event_base_new()))
        {
            fprintf(stderr, "TriggerRegister fail \n");
            SkeletonDeallocate(trigger);
        }
    }

    return trigger;
}

void TriggerLogout(Trigger *trigger)
{
    if (trigger)
    {
        if (trigger->base)
        {
            event_base_free(trigger->base);
        }
        SkeletonDeallocate(trigger);
    }
    libevent_global_shutdown();
}

TriggerEvent *TriggerEventRegister(Trigger *trigger)
{
    TriggerEvent *triggerEvent = NULL;

    if (SkeletonAllocate((void **)&triggerEvent, sizeof(TriggerEvent)))
    {
        triggerEvent->trigger = trigger;
        if (!(triggerEvent->event = event_new(triggerEvent->trigger->base, SIGINT, EV_SIGNAL | EV_PERSIST, __EventTriggerExit, triggerEvent)))
        {
            perror("事件增加失败");
            SkeletonDeallocate(triggerEvent);
        }
        if (event_add(triggerEvent->event, NULL) == -1)
        {
            perror("事件加入失败");
            event_free(triggerEvent->event);
            SkeletonDeallocate(triggerEvent);
        }
    }
    return triggerEvent;
}

void TriggerEventLogout(TriggerEvent *triggerEvent)
{
    if (triggerEvent)
    {
        if (triggerEvent->event)
        {
            event_free(triggerEvent->event);
        }
        SkeletonDeallocate(triggerEvent);
    }
}

Exception TriggerDispatch(Trigger *trigger)
{
    if ((trigger == NULL) && (trigger->base == NULL))
    {
        return IllegalArgumentException;
    }
    else
    {
        event_base_dispatch(trigger->base);
    }
    return OkException;
}