#include <nan.h>
#include <iostream>
#include <map>
#import <Cocoa/Cocoa.h>
#import <objc/runtime.h>

typedef Nan::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function> > FunctionPersistent;

std::map<NSWindow *, FunctionPersistent> emitCallbacks;

void emitProximityEvent(
  const v8::Local<v8::Function> &func,
  const char *type,
  const char *pointerType,
  int pointerId
) {
  v8::Local<v8::Object> event = Nan::New<v8::Object>();

  event->Set(Nan::New("pointerType").ToLocalChecked(), Nan::New(pointerType).ToLocalChecked());
  event->Set(Nan::New("pointerId").ToLocalChecked(), Nan::New(pointerId));

  v8::Local<v8::Value> argv[] = { Nan::New(type).ToLocalChecked(), event };
  func->Call(Nan::GetCurrentContext()->Global(), 2, argv);
}

void emitTabletEvent(
  const v8::Local<v8::Function> &func,
  const char *type,
  bool altKey,
  bool ctrlKey,
  bool metaKey,
  bool shiftKey,
  double clientX,
  double clientY,
  double pressure,
  double tiltX,
  double tiltY,
  const char *pointerType,
  int pointerId
) {
  v8::Local<v8::Object> event = Nan::New<v8::Object>();

  event->Set(Nan::New("altKey").ToLocalChecked(), Nan::New(altKey));
  event->Set(Nan::New("ctrlKey").ToLocalChecked(), Nan::New(ctrlKey));
  event->Set(Nan::New("metaKey").ToLocalChecked(), Nan::New(metaKey));
  event->Set(Nan::New("shiftKey").ToLocalChecked(), Nan::New(shiftKey));

  event->Set(Nan::New("clientX").ToLocalChecked(), Nan::New(clientX));
  event->Set(Nan::New("clientY").ToLocalChecked(), Nan::New(clientY));

  event->Set(Nan::New("pressure").ToLocalChecked(), Nan::New(pressure));

  event->Set(Nan::New("tiltX").ToLocalChecked(), Nan::New(tiltX));
  event->Set(Nan::New("tiltY").ToLocalChecked(), Nan::New(tiltY));

  event->Set(Nan::New("pointerType").ToLocalChecked(), Nan::New(pointerType).ToLocalChecked());
  event->Set(Nan::New("pointerId").ToLocalChecked(), Nan::New(pointerId));

  v8::Local<v8::Value> argv[] = { Nan::New(type).ToLocalChecked(), event };
  func->Call(Nan::GetCurrentContext()->Global(), 2, argv);
}

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
  if (emitCallbacks.find(self) != emitCallbacks.end()) {

    switch (event.type) {
      case NSTabletProximity: {
        const char *type;
        if (event.enteringProximity) {
          type = "enterProximity";
        } else {
          type = "leaveProximity";
        }
        std::cout << "tablet proximity" << std::endl;
        emitProximityEvent(Nan::New(emitCallbacks[self]), type, pointerTypeString(event.pointingDeviceType), event.uniqueID);
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

        std::cout << "mouse event at (" << localPos.x << "," << localPos.y << ") pressure " << event.pressure << std::endl;

        emitTabletEvent(
          Nan::New(emitCallbacks[self]), type,
          event.modifierFlags & NSAlternateKeyMask,
          event.modifierFlags & NSControlKeyMask,
          event.modifierFlags & NSCommandKeyMask,
          event.modifierFlags & NSShiftKeyMask,
          localPos.x, self.contentView.bounds.size.height - localPos.y,
          event.pressure,
          0, 0,
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

void intercept(const Nan::FunctionCallbackInfo<v8::Value> &info) {
  if (info.Length() != 2) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  char *buf = node::Buffer::Data(info[0]);
  NSView *view = *reinterpret_cast<NSView **>(buf);
  NSWindow *window = view.window;

  std::cout << window << std::endl;

  v8::Local<v8::Function> cb = info[1].As<v8::Function>();

  emitCallbacks[window] = FunctionPersistent(cb);

  info.GetReturnValue().Set(Nan::Undefined());
}

void init(v8::Local<v8::Object> exports) {
  [NSWindow switchSendEvent];

  exports->Set(Nan::New("intercept").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(intercept)->GetFunction());
}

NODE_MODULE(intercept_tablet_event, init)
