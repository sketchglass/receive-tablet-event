#include <nan.h>

typedef Nan::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function> > FunctionPersistent;

void EmitProximityEvent(
  const FunctionPersistent &func,
  const char *type,
  const char *pointerType,
  int pointerId
);

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
);

void InterceptWindow(void *handle, const FunctionPersistent &callback);
void UninterceptWindow(void *handle);
void Init();
