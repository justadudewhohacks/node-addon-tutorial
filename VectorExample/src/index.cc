#include <nan.h>
#include "Vector.h"

NAN_MODULE_INIT(InitModule) {
  Vector::Init(target);
}

NODE_MODULE(myModule, InitModule);