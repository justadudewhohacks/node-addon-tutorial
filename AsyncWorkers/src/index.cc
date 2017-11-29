#include <nan.h>
#include "MyAsyncBinding.h"

NAN_MODULE_INIT(InitModule) {
  MyAsyncBinding::Init(target);
}

NODE_MODULE(myModule, InitModule);