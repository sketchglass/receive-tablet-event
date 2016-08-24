#include "EventReceiver.h"

// do nothing

void InitEventReceiver()
{
}

std::unique_ptr<EventReceiver> CreateEventReceiver(void *handle, std::unique_ptr<EventDelegate> &&delegate)
{
    return std::unique_ptr<EventReceiver>(
        new EventReceiver(std::move(delegate))
    );
}
