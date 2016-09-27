
class EventDelegate
{
public:
    virtual ~EventDelegate() {}
    virtual bool OnTabletEvent(
        const char *type,
        int button,
        double clientX,
        double clientY,
        double pressure,
        const char *pointerType,
        int pointerId
    ) = 0;
};
