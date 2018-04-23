//
// Created by klh on 4/22/18.
//

#include <nan.h>
#include "DeviceWrap.hh"

void InitAll(v8::Local<v8::Object> exports) {
  DeviceWrap::Init(exports);
}

NODE_MODULE(node_b6, InitAll)