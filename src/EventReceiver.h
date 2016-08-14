#include <memory>
#include "EventDelegate.h"

class EventReceiver
{
public:
    EventReceiver(std::unique_ptr<EventDelegate> &&delegate) : m_delegate(std::move(delegate)) {}
    virtual ~EventReceiver() {}

    const std::unique_ptr<EventDelegate>& Delegate() const
    {
        return m_delegate;
    }

protected:
    bool HandleProximityEvent(
        const char *type,
        const char *pointerType,
        int pointerId
    );
    bool HandleTabletEvent(
        const char *type,
        double clientX,
        double clientY,
        double pressure,
        const char *pointerType,
        int pointerId
    );

private:
    std::unique_ptr<EventDelegate> m_delegate;
};

void InitEventReceiver();
std::unique_ptr<EventReceiver> CreateEventReceiver(void *handle, std::unique_ptr<EventDelegate> &&delegate);
