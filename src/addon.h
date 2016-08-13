
void EmitProximityEvent(
  void *handle,
  const char *type,
  const char *pointerType,
  int pointerId
);

void EmitTabletEvent(
  void *handle,
  const char *type,
  double clientX,
  double clientY,
  double pressure,
  const char *pointerType,
  int pointerId
);

void InterceptWindow(void *handle);
void UninterceptWindow(void *handle);
void Init();
