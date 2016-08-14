#include "EventReceiver.h"

bool EventReceiver::HandleProximityEvent(
    const char *type,
    const char *pointerType,
    int pointerId
) {
    Delegate()->OnProximityEvent(type, pointerType, pointerId);
    return true;
}

bool EventReceiver::HandleTabletEvent(
    const char *type,
    double clientX,
    double clientY,
    double pressure,
    const char *pointerType,
    int pointerId
) {
    Delegate()->OnTabletEvent(type, clientX, clientY, pressure, pointerType, pointerId);
    return true;
}
