#include <nan.h>

class MyAsyncBinding {
public:
  static NAN_MODULE_INIT(Init);
  static NAN_METHOD(DoSyncStuff);
  static NAN_METHOD(DoAsyncStuff);
};