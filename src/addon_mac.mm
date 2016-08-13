#include "addon.h"
#include <iostream>
#include <map>
#import <Cocoa/Cocoa.h>
#import <objc/runtime.h>

std::map<NSWindow *, void *> handles;

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

@implementation NSWindow(InterceptTabletEvent)

-(void)sendEventIntercept:(NSEvent *)event
{
  if (handles.find(self) != handles.end()) {

    switch (event.type) {
      case NSTabletProximity: {
        const char *type;
        if (event.enteringProximity) {
          type = "enterProximity";
        } else {
          type = "leaveProximity";
        }
        //std::cout << "tablet proximity" << std::endl;
        EmitProximityEvent(handles[self], type, pointerTypeString(event.pointingDeviceType), event.uniqueID);
        break;
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

        CGPoint windowPos = event.locationInWindow;
        CGPoint localPos = [self.contentView convertPoint: windowPos fromView: nil];

        //std::cout << "mouse event at (" << localPos.x << "," << localPos.y << ") pressure " << event.pressure << std::endl;

        EmitTabletEvent(
          handles[self], type,
          localPos.x, self.contentView.bounds.size.height - localPos.y,
          event.pressure,
          pointerTypeString(event.pointingDeviceType),
          event.uniqueID
        );

        break;
      }
      default:
        break;
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
  Method fromMethod = class_getInstanceMethod(self, from);
  Method toMethod = class_getInstanceMethod(self, to);
  method_exchangeImplementations(fromMethod, toMethod);
}

@end

void InterceptWindow(void *handle) {
  NSView *view = reinterpret_cast<NSView *>(handle);
  NSWindow *window = view.window;

  handles[window] = handle;
}

void UninterceptWindow(void *handle) {
  NSView *view = reinterpret_cast<NSView *>(handle);
  NSWindow *window = view.window;

  handles.erase(window);
}

void Init() {
  [NSWindow switchSendEvent];
}
