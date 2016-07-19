#include <nan.h>
#include <iostream>

void intercept(const Nan::FunctionCallbackInfo<v8::Value> &info) {
  if (info.Length() != 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  char *buf = node::Buffer::Data(info[0]);
  std::cout << buf << std::endl;
}

void init(v8::Local<v8::Object> exports) {
  exports->Set(Nan::New("intercept").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(intercept)->GetFunction());
}

NODE_MODULE(tablet_event_interceptor, init)
