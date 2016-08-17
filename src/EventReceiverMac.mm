#include "EventReceiver.h"
#include <iostream>
#include <unordered_map>
#import <Cocoa/Cocoa.h>
#import <objc/runtime.h>

class EventReceiverMac;

std::unordered_map<NSWindow *, EventReceiverMac *> eventReceivers;

const char *pointerTypeString(NSPointingDeviceType type) {
    switch (type) {
        case NSPenPointingDevice:
        return "pen";
        case NSCursorPointingDevice:
        return "cursor";
        case NSEraserPointingDevice:
        return "eraser";
        default:
        return "unknown";
    }
}

class EventReceiverMac : public EventReceiver {
public:
    EventReceiverMac(NSView *view, std::unique_ptr<EventDelegate> &&delegate) : EventReceiver(std::move(delegate))
    {
        m_window = view.window;
        eventReceivers[m_window] = this;
    }

    ~EventReceiverMac()
    {
        eventReceivers.erase(m_window);
    }

    bool OnEvent(NSEvent *event)
    {
        switch (event.type) {
        case NSTabletProximity:
        {
            const char *type;
            if (event.enteringProximity) {
                type = "enterProximity";
            } else {
                type = "leaveProximity";
            }
            Delegate()->OnTabletEvent(type, 0, 0, 0, pointerTypeString(event.pointingDeviceType), event.uniqueID);
            return false;
        }
        case NSLeftMouseDown:
        case NSRightMouseDown:
        case NSLeftMouseUp:
        case NSRightMouseUp:
        case NSMouseMoved:
        case NSTabletPoint:
        case NSLeftMouseDragged:
        case NSRightMouseDragged:
        {
            const char *type;
            switch (event.type) {
                case NSLeftMouseDown:
                case NSRightMouseDown:
                type = "down";
                break;
                case NSLeftMouseUp:
                case NSRightMouseUp:
                type = "up";
                break;
                default:
                type = "move";
                break;
            }

            auto view = m_window.contentView;
            auto windowPos = event.locationInWindow;
            auto localPos = [view convertPoint: windowPos fromView: nil];

            return Delegate()->OnTabletEvent(
                type,
                localPos.x, view.bounds.size.height - localPos.y,
                event.pressure,
                pointerTypeString(event.pointingDeviceType),
                event.uniqueID
            );
        }
        default:
            return false;
        }
    }

private:
    NSWindow *m_window;
};

@implementation NSWindow(InterceptTabletEvent)

-(void)sendEventIntercept:(NSEvent *)event
{
    if (eventReceivers.find(self) != eventReceivers.end()) {
        if (eventReceivers[self]->OnEvent(event)) {
            return;
        }
    }
    [self sendEventIntercept:event];
}

+(void)switchSendEvent
{
    [self switchMethodFrom:@selector(sendEvent:) to:@selector(sendEventIntercept:)];
}

+(void)switchMethodFrom:(SEL)from to:(SEL)to
{
    auto fromMethod = class_getInstanceMethod(self, from);
    auto toMethod = class_getInstanceMethod(self, to);
    method_exchangeImplementations(fromMethod, toMethod);
}

@end

void InitEventReceiver()
{
    [NSWindow switchSendEvent];
}

std::unique_ptr<EventReceiver> CreateEventReceiver(void *handle, std::unique_ptr<EventDelegate> &&delegate)
{
    return std::unique_ptr<EventReceiver>(
        new EventReceiverMac((NSView *)handle, std::move(delegate))
    );
}
