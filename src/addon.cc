#include <nan.h>
#include <iostream>
#include <map>
#include "addon.h"

typedef Nan::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function> > FunctionPersistent;

std::map<void *, FunctionPersistent> emitCallbacks;

void EmitProximityEvent(
  void *handle,
  const char *type,
  const char *pointerType,
  int pointerId
) {
  v8::Local<v8::Object> event = Nan::New<v8::Object>();

  event->Set(Nan::New("pointerType").ToLocalChecked(), Nan::New(pointerType).ToLocalChecked());
  event->Set(Nan::New("pointerId").ToLocalChecked(), Nan::New(pointerId));

  v8::Local<v8::Value> argv[] = { Nan::New(type).ToLocalChecked(), event };
  Nan::New(emitCallbacks[handle])->Call(Nan::GetCurrentContext()->Global(), 2, argv);
}

void EmitTabletEvent(
  void *handle,
  const char *type,
  double clientX,
  double clientY,
  double pressure,
  const char *pointerType,
  int pointerId
) {
  v8::Local<v8::Object> event = Nan::New<v8::Object>();

  event->Set(Nan::New("clientX").ToLocalChecked(), Nan::New(clientX));
  event->Set(Nan::New("clientY").ToLocalChecked(), Nan::New(clientY));

  event->Set(Nan::New("pressure").ToLocalChecked(), Nan::New(pressure));

  event->Set(Nan::New("pointerType").ToLocalChecked(), Nan::New(pointerType).ToLocalChecked());
  event->Set(Nan::New("pointerId").ToLocalChecked(), Nan::New(pointerId));

  v8::Local<v8::Value> argv[] = { Nan::New(type).ToLocalChecked(), event };
  Nan::New(emitCallbacks[handle])->Call(Nan::GetCurrentContext()->Global(), 2, argv);
}

void intercept(const Nan::FunctionCallbackInfo<v8::Value> &info) {
  if (info.Length() != 2) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  char *buf = node::Buffer::Data(info[0]);
  void *handle = *reinterpret_cast<void **>(buf);
  v8::Local<v8::Function> cb = info[1].As<v8::Function>();
  emitCallbacks[handle] = FunctionPersistent(cb);
  InterceptWindow(handle);

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
  emitCallbacks.erase(handle);

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
