
class EventDelegate
{
public:
    virtual ~EventDelegate() {}
    virtual void OnProximityEvent(
        const char *type,
        const char *pointerType,
        int pointerId
    ) = 0;
    virtual void OnTabletEvent(
        const char *type,
        double clientX,
        double clientY,
        double pressure,
        const char *pointerType,
        int pointerId
    ) = 0;
};
