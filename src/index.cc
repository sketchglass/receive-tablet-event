#include <nan.h>
#include <iostream>
#include <unordered_map>
#include "EventReceiver.h"

typedef Nan::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>> FunctionPersistent;

class EventDelegateNode : public EventDelegate {
public:
    EventDelegateNode(const FunctionPersistent &callback) : m_callback(callback) {}

    bool OnTabletEvent(
        const char *type,
        int button,
        double clientX,
        double clientY,
        double pressure,
        const char *pointerType,
        int pointerId
    ) override
    {
        v8::Local<v8::Value> argv[] = {
            Nan::New(type).ToLocalChecked(),
            Nan::New(button),
            Nan::New(clientX),
            Nan::New(clientY),
            Nan::New(pressure),
            Nan::New(pointerType).ToLocalChecked(),
            Nan::New(pointerId)
        };
        auto result = Nan::New(m_callback)->Call(Nan::GetCurrentContext()->Global(), 6, argv);
        return result->BooleanValue();
    }
private:
    FunctionPersistent m_callback;
};

static std::unordered_map<void *, std::unique_ptr<EventReceiver>> eventReceivers;

static void Intercept(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    if (info.Length() != 2) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }

    char *buf = node::Buffer::Data(info[0]);
    void *handle = *reinterpret_cast<void **>(buf);
    v8::Local<v8::Function> cb = info[1].As<v8::Function>();

    eventReceivers[handle] = CreateEventReceiver(
        handle,
        std::unique_ptr<EventDelegate>(new EventDelegateNode(FunctionPersistent(cb)))
    );

    info.GetReturnValue().Set(Nan::Undefined());
}

static void OnReload(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    if (info.Length() != 1) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }

    char *buf = node::Buffer::Data(info[0]);
    void *handle = *reinterpret_cast<void **>(buf);

    eventReceivers[handle]->OnReload();

    info.GetReturnValue().Set(Nan::Undefined());
}

static void Unintercept(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    if (info.Length() != 1) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }

    char *buf = node::Buffer::Data(info[0]);
    void *handle = *reinterpret_cast<void **>(buf);
    eventReceivers.erase(handle);

    info.GetReturnValue().Set(Nan::Undefined());
}

static void InitModule(v8::Local<v8::Object> exports) {
    InitEventReceiver();

    exports->Set(
        Nan::New("intercept").ToLocalChecked(),
        Nan::New<v8::FunctionTemplate>(Intercept)->GetFunction()
    );
    exports->Set(
        Nan::New("onReload").ToLocalChecked(),
        Nan::New<v8::FunctionTemplate>(OnReload)->GetFunction()
    );
    exports->Set(
        Nan::New("unintercept").ToLocalChecked(),
        Nan::New<v8::FunctionTemplate>(Unintercept)->GetFunction()
    );
}

NODE_MODULE(receive_tablet_event, InitModule)
