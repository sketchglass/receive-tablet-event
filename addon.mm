#include <nan.h>
#include <iostream>

void intercept(const Nan::FunctionCallbackInfo<v8::Value> &info) {
  if (info.Length() != 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  char *buf = node::Buffer::Data(info[0]);
  void *handle = *reinterpret_cast<void **>(buf);
  std::cout << handle << std::endl;

  info.GetReturnValue().Set(Nan::Undefined());
}

void init(v8::Local<v8::Object> exports) {
  exports->Set(Nan::New("intercept").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(intercept)->GetFunction());
}

NODE_MODULE(intercept_tablet_event, init)
