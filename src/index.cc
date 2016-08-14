#include <nan.h>
#include <iostream>
#include <unordered_map>
#include "EventReceiver.h"

typedef Nan::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>> FunctionPersistent;

class EventDelegateNode : public EventDelegate {
public:
    EventDelegateNode(const FunctionPersistent &callback) : m_callback(callback) {}

    void OnProximityEvent(
        const char *type,
        const char *pointerType,
        int pointerId
    ) override
    {
        v8::Local<v8::Object> event = Nan::New<v8::Object>();

        event->Set(Nan::New("pointerType").ToLocalChecked(), Nan::New(pointerType).ToLocalChecked());
        event->Set(Nan::New("pointerId").ToLocalChecked(), Nan::New(pointerId));

        v8::Local<v8::Value> argv[] = { Nan::New(type).ToLocalChecked(), event };
        Nan::New(m_callback)->Call(Nan::GetCurrentContext()->Global(), 2, argv);
    }

    void OnTabletEvent(
        const char *type,
        double clientX,
        double clientY,
        double pressure,
        const char *pointerType,
        int pointerId
    ) override
    {
        v8::Local<v8::Object> event = Nan::New<v8::Object>();

        event->Set(Nan::New("clientX").ToLocalChecked(), Nan::New(clientX));
        event->Set(Nan::New("clientY").ToLocalChecked(), Nan::New(clientY));

        event->Set(Nan::New("pressure").ToLocalChecked(), Nan::New(pressure));

        event->Set(Nan::New("pointerType").ToLocalChecked(), Nan::New(pointerType).ToLocalChecked());
        event->Set(Nan::New("pointerId").ToLocalChecked(), Nan::New(pointerId));

        v8::Local<v8::Value> argv[] = { Nan::New(type).ToLocalChecked(), event };
        Nan::New(m_callback)->Call(Nan::GetCurrentContext()->Global(), 2, argv);
    }
private:
    FunctionPersistent m_callback;
};

static std::unordered_map<void *, std::unique_ptr<EventReceiver>> eventReceivers;

static void Intercept(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    if (info.Length() != 2)
    {
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

static void SetTargetRectangle(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    if (info.Length() != 5)
    {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }
    char *buf = node::Buffer::Data(info[0]);
    void *handle = *reinterpret_cast<void **>(buf);

    auto left = info[1]->NumberValue();
    auto top = info[2]->NumberValue();
    auto width = info[3]->NumberValue();
    auto height = info[4]->NumberValue();

    if (eventReceivers.find(handle) != eventReceivers.end()) {
        Rectangle rect { left, top, width, height };
        eventReceivers[handle]->SetTargetRectangle(rect);
    }

    info.GetReturnValue().Set(Nan::Undefined());
}

static void Unintercept(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    if (info.Length() != 1)
    {
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
        Nan::New("setTargetRectangle").ToLocalChecked(),
        Nan::New<v8::FunctionTemplate>(SetTargetRectangle)->GetFunction()
    );
    exports->Set(
        Nan::New("unintercept").ToLocalChecked(),
        Nan::New<v8::FunctionTemplate>(Unintercept)->GetFunction()
    );
}

NODE_MODULE(receive_tablet_event, InitModule)
