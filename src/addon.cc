#include <nan.h>
#include <iostream>
#include "addon.h"

void EmitProximityEvent(
  const FunctionPersistent &func,
  const char *type,
  const char *pointerType,
  int pointerId
) {
  v8::Local<v8::Object> event = Nan::New<v8::Object>();

  event->Set(Nan::New("pointerType").ToLocalChecked(), Nan::New(pointerType).ToLocalChecked());
  event->Set(Nan::New("pointerId").ToLocalChecked(), Nan::New(pointerId));

  v8::Local<v8::Value> argv[] = { Nan::New(type).ToLocalChecked(), event };
  Nan::New(func)->Call(Nan::GetCurrentContext()->Global(), 2, argv);
}

void EmitTabletEvent(
  const FunctionPersistent &func,
  const char *type,
  bool altKey,
  bool ctrlKey,
  bool metaKey,
  bool shiftKey,
  double clientX,
  double clientY,
  double pressure,
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

  event->Set(Nan::New("pointerType").ToLocalChecked(), Nan::New(pointerType).ToLocalChecked());
  event->Set(Nan::New("pointerId").ToLocalChecked(), Nan::New(pointerId));

  v8::Local<v8::Value> argv[] = { Nan::New(type).ToLocalChecked(), event };
  Nan::New(func)->Call(Nan::GetCurrentContext()->Global(), 2, argv);
}

void intercept(const Nan::FunctionCallbackInfo<v8::Value> &info) {
  if (info.Length() != 2) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  char *buf = node::Buffer::Data(info[0]);
  void *handle = *reinterpret_cast<void **>(buf);
  v8::Local<v8::Function> cb = info[1].As<v8::Function>();
  InterceptWindow(handle, FunctionPersistent(cb));

  info.GetReturnValue().Set(Nan::Undefined());
}

void unintercept(const Nan::FunctionCallbackInfo<v8::Value> &info) {
  if (info.Length() != 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  char *buf = node::Buffer::Data(info[0]);
  void *handle = *reinterpret_cast<void **>(buf);
  UninterceptWindow(handle);

  info.GetReturnValue().Set(Nan::Undefined());
}

void initModule(v8::Local<v8::Object> exports) {
  Init();

  exports->Set(Nan::New("intercept").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(intercept)->GetFunction());
  exports->Set(Nan::New("unintercept").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(unintercept)->GetFunction());
}

NODE_MODULE(receive_tablet_event, initModule)
