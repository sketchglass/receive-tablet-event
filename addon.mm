#include <nan.h>
#include <iostream>
#include <map>
#import <Cocoa/Cocoa.h>
#import <objc/runtime.h>

typedef Nan::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function> > FunctionPersistent;

std::map<NSWindow *, FunctionPersistent> callbacks;

@implementation NSWindow(InterceptTabletEvent)

-(void)sendEventIntercept:(NSEvent *)event
{
  switch (event.type) {
    case NSTabletProximity: {
      std::cout << "tablet proximity" << std::endl;
      break;
    }
    case NSMouseMoved:
    case NSLeftMouseDown:
    case NSRightMouseDown:
    case NSLeftMouseUp:
    case NSRightMouseUp:
    case NSTabletPoint:
    case NSLeftMouseDragged:
    case NSRightMouseDragged: {
      CGPoint pos = event.locationInWindow;
      std::cout << "mouse event at (" << pos.x << "," << pos.y << ") pressure" << event.pressure << std::endl;
      break;
    }
    default:
      break;
  }

  /*
  if (callbacks.find(self) != callbacks.end()) {
    v8::Local<v8::Function> func = Nan::New(callbacks[self]);
    v8::Local<v8::Object> eventObj = Nan::New<v8::Object>();
    eventObj->Set(Nan::New("timestamp").ToLocalChecked(), Nan::New(event.timestamp));
    v8::Local<v8::Value> argv[] = { eventObj };
    Nan::MakeCallback(Nan::GetCurrentContext()->Global(), func, sizeof(argv), argv);
  }
  */

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

  callbacks[window] = FunctionPersistent(cb);

  info.GetReturnValue().Set(Nan::Undefined());
}

void init(v8::Local<v8::Object> exports) {
  [NSWindow switchSendEvent];

  exports->Set(Nan::New("intercept").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(intercept)->GetFunction());
}

NODE_MODULE(intercept_tablet_event, init)
