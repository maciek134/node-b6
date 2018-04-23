//
// Created by klh on 4/22/18.
//

#include "DeviceWrap.hh"

Nan::Persistent<v8::Function> DeviceWrap::constructor;

void DeviceWrap::Init(v8::Local<v8::Object> exports) {
  Nan::HandleScope scope;

  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Device").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(19);

  v8::Local<v8::ObjectTemplate> itpl = tpl->InstanceTemplate();

  Nan::SetAccessor(itpl, Nan::New("coreType").ToLocalChecked(), GetCoreType);
  Nan::SetAccessor(itpl, Nan::New("upgradeType").ToLocalChecked(), GetUpgradeType);
  Nan::SetAccessor(itpl, Nan::New("languageId").ToLocalChecked(), GetLanguageId);
  Nan::SetAccessor(itpl, Nan::New("customerId").ToLocalChecked(), GetCustomerId);
  Nan::SetAccessor(itpl, Nan::New("hwVersion").ToLocalChecked(), GetHWVersion);
  Nan::SetAccessor(itpl, Nan::New("swVersion").ToLocalChecked(), GetSWVersion);
  Nan::SetAccessor(itpl, Nan::New("isEncrypted").ToLocalChecked(), GetIsEncrypted);
  Nan::SetAccessor(itpl, Nan::New("cellCount").ToLocalChecked(), GetCellCount);

  Nan::SetPrototypeMethod(tpl, "getDefaultChargeProfile", GetDefaultChargeProfile);
  Nan::SetPrototypeMethod(tpl, "setCycleTime", SetCycleTime);
  Nan::SetPrototypeMethod(tpl, "setTimeLimit", SetTimeLimit);
  Nan::SetPrototypeMethod(tpl, "setCapacityLimit", SetCapacityLimit);
  Nan::SetPrototypeMethod(tpl, "setTempLimit", SetTempLimit);
  Nan::SetPrototypeMethod(tpl, "setBuzzers", SetBuzzers);
  Nan::SetPrototypeMethod(tpl, "stopCharging", StopCharging);
  Nan::SetPrototypeMethod(tpl, "startCharging", StartCharging);

  Nan::SetPrototypeMethod(tpl, "getSysInfo", GetSysInfo);
  Nan::SetPrototypeMethod(tpl, "getChargeInfo", GetChargeInfo);

  Nan::SetPrototypeMethod(tpl, "_listen", Listen);

  constructor.Reset(tpl->GetFunction());
  exports->Set(Nan::New("Device").ToLocalChecked(), tpl->GetFunction());
}

DeviceWrap::DeviceWrap() = default;

DeviceWrap::~DeviceWrap() {
  delete m_dev;
}

void DeviceWrap::New(const Nan::FunctionCallbackInfo<v8::Value> &info) {
  if (info.IsConstructCall()) {
    auto obj = new DeviceWrap();
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    const int argc = 0;
    v8::Local<v8::Value> argv[argc] = {};
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    info.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

void DeviceWrap::GetCoreType(v8::Local<v8::String>, const Nan::PropertyCallbackInfo<v8::Value> &info) {
  auto obj = Nan::ObjectWrap::Unwrap<DeviceWrap>(info.Holder());
  info.GetReturnValue().Set(Nan::New(obj->m_dev->getCoreType()).ToLocalChecked());
}

void DeviceWrap::GetUpgradeType(v8::Local<v8::String>, const Nan::PropertyCallbackInfo<v8::Value> &info) {
  auto obj = Nan::ObjectWrap::Unwrap<DeviceWrap>(info.Holder());
  info.GetReturnValue().Set(Nan::New(obj->m_dev->getUpgradeType()));
}

void DeviceWrap::GetLanguageId(v8::Local<v8::String>, const Nan::PropertyCallbackInfo<v8::Value> &info) {
  auto obj = Nan::ObjectWrap::Unwrap<DeviceWrap>(info.Holder());
  info.GetReturnValue().Set(Nan::New(obj->m_dev->getLanguageID()));
}

void DeviceWrap::GetCustomerId(v8::Local<v8::String>, const Nan::PropertyCallbackInfo<v8::Value> &info) {
  auto obj = Nan::ObjectWrap::Unwrap<DeviceWrap>(info.Holder());
  info.GetReturnValue().Set(Nan::New(obj->m_dev->getCustomerID()));
}

void DeviceWrap::GetHWVersion(v8::Local<v8::String>, const Nan::PropertyCallbackInfo<v8::Value> &info) {
  auto obj = Nan::ObjectWrap::Unwrap<DeviceWrap>(info.Holder());
  info.GetReturnValue().Set(Nan::New(obj->m_dev->getHWVersion()));
}

void DeviceWrap::GetSWVersion(v8::Local<v8::String>, const Nan::PropertyCallbackInfo<v8::Value> &info) {
  auto obj = Nan::ObjectWrap::Unwrap<DeviceWrap>(info.Holder());
  info.GetReturnValue().Set(Nan::New(obj->m_dev->getSWVersion()));
}

void DeviceWrap::GetIsEncrypted(v8::Local<v8::String>, const Nan::PropertyCallbackInfo<v8::Value> &info) {
  auto obj = Nan::ObjectWrap::Unwrap<DeviceWrap>(info.Holder());
  info.GetReturnValue().Set(Nan::New(obj->m_dev->isEncrypted()));
}

void DeviceWrap::GetCellCount(v8::Local<v8::String>, const Nan::PropertyCallbackInfo<v8::Value> &info) {
  auto obj = Nan::ObjectWrap::Unwrap<DeviceWrap>(info.Holder());
  info.GetReturnValue().Set(Nan::New(obj->m_dev->getCellCount()));
}

void DeviceWrap::GetDefaultChargeProfile(const Nan::FunctionCallbackInfo<v8::Value> &info) {
  if (info.Length() < 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  if (!info[0]->IsString()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  auto obj = Nan::ObjectWrap::Unwrap<DeviceWrap>(info.Holder());
  auto type = static_cast<b6::BATTERY_TYPE>(info[0]->Int32Value());

  b6::ChargeProfile profile = obj->m_dev->getDefaultChargeProfile(type);

  v8::Local<v8::Object> ret = Nan::New<v8::Object>();
  ret->Set(Nan::New("batteryType").ToLocalChecked(), Nan::New(BatteryTypeToString(profile.batteryType)).ToLocalChecked());
  ret->Set(Nan::New("mode").ToLocalChecked(), Nan::New(
    b6::Device::isBatteryLi(profile.batteryType) ? ChargingModeToString(profile.mode.li) :
    b6::Device::isBatteryNi(profile.batteryType) ? ChargingModeToString(profile.mode.ni) :
    ChargingModeToString(profile.mode.pb)
  ).ToLocalChecked());
  ret->Set(Nan::New("cellCount").ToLocalChecked(), Nan::New(profile.cellCount));
  ret->Set(Nan::New("rePeakCount").ToLocalChecked(), Nan::New(profile.rPeakCount));
  ret->Set(Nan::New("cycleType").ToLocalChecked(), Nan::New(profile.cycleType));
  ret->Set(Nan::New("cycleCount").ToLocalChecked(), Nan::New(profile.cycleCount));
  ret->Set(Nan::New("chargeCurrent").ToLocalChecked(), Nan::New(profile.chargeCurrent));
  ret->Set(Nan::New("dischargeCurrent").ToLocalChecked(), Nan::New(profile.dischargeCurrent));
  ret->Set(Nan::New("cellDischargeVoltage").ToLocalChecked(), Nan::New(profile.cellDischargeVoltage));
  ret->Set(Nan::New("endVoltage").ToLocalChecked(), Nan::New(profile.endVoltage));
  ret->Set(Nan::New("trickleCurrent").ToLocalChecked(), Nan::New(profile.trickleCurrent));

  info.GetReturnValue().Set(ret);
}

void DeviceWrap::SetCycleTime(const Nan::FunctionCallbackInfo<v8::Value> &info) {
  if (info.Length() < 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  if (!info[0]->IsInt32()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  auto obj = Nan::ObjectWrap::Unwrap<DeviceWrap>(info.Holder());
  int cycleTime = info[0]->Int32Value();

  info.GetReturnValue().Set(Nan::New(obj->m_dev->setCycleTime(cycleTime)));
}

void DeviceWrap::SetTimeLimit(const Nan::FunctionCallbackInfo<v8::Value> &info) {
  if (info.Length() < 2) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  if (!info[0]->IsBoolean() || !info[1]->IsInt32()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  auto obj = Nan::ObjectWrap::Unwrap<DeviceWrap>(info.Holder());
  bool enabled = info[0]->BooleanValue();
  int limit = info[1]->Int32Value();

  info.GetReturnValue().Set(Nan::New(obj->m_dev->setTimeLimit(enabled, limit)));
}

void DeviceWrap::SetCapacityLimit(const Nan::FunctionCallbackInfo<v8::Value> &info) {
  if (info.Length() < 2) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  if (!info[0]->IsBoolean() || !info[1]->IsInt32()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  auto obj = Nan::ObjectWrap::Unwrap<DeviceWrap>(info.Holder());
  bool enabled = info[0]->BooleanValue();
  int limit = info[1]->Int32Value();

  info.GetReturnValue().Set(Nan::New(obj->m_dev->setCapacityLimit(enabled, limit)));
}

void DeviceWrap::SetTempLimit(const Nan::FunctionCallbackInfo<v8::Value> &info) {
  if (info.Length() < 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  if (!info[0]->IsInt32()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  auto obj = Nan::ObjectWrap::Unwrap<DeviceWrap>(info.Holder());
  int tempLimit = info[0]->Int32Value();

  info.GetReturnValue().Set(Nan::New(obj->m_dev->setTempLimit(tempLimit)));
}

void DeviceWrap::SetBuzzers(const Nan::FunctionCallbackInfo<v8::Value> &info) {
  if (info.Length() < 2) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  if (!info[0]->IsBoolean() || !info[1]->IsBoolean()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  auto obj = Nan::ObjectWrap::Unwrap<DeviceWrap>(info.Holder());
  bool sys = info[0]->BooleanValue();
  bool key = info[1]->BooleanValue();

  info.GetReturnValue().Set(Nan::New(obj->m_dev->setCapacityLimit(sys, key)));
}

void DeviceWrap::StopCharging(const Nan::FunctionCallbackInfo<v8::Value> &info) {
  auto obj = Nan::ObjectWrap::Unwrap<DeviceWrap>(info.Holder());
  obj->m_dev->stopCharging();
}

void DeviceWrap::StartCharging(const Nan::FunctionCallbackInfo<v8::Value> &info) {
  if (info.Length() < 11) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  if (!info[0]->IsString() || !info[1]->IsString() || !info[2]->IsUint32() || !info[3]->IsUint32() || !info[4]->IsUint32() ||
      !info[5]->IsUint32() || !info[6]->IsUint32() || !info[7]->IsUint32() || !info[8]->IsUint32() || !info[9]->IsUint32() ||
      !info[10]->IsUint32()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  auto obj = Nan::ObjectWrap::Unwrap<DeviceWrap>(info.Holder());

  try {
    b6::BATTERY_TYPE type = StringToBatteryType(*v8::String::Utf8Value(info[0]->ToString()));

    b6::ChargeProfile profile = obj->m_dev->getDefaultChargeProfile(type);
    profile.batteryType = type;

    v8::String::Utf8Value modeStr(info[1]->ToString());
    if (b6::Device::isBatteryLi(type)) {
      profile.mode.li = StringToChargingModeLi(*modeStr);
    } else if (b6::Device::isBatteryNi(type)) {
      profile.mode.ni = StringToChargingModeNi(*modeStr);
    } else {
      profile.mode.pb = StringToChargingModePb(*modeStr);
    }

    profile.cellCount = static_cast<uint8_t>(info[2]->Uint32Value());
    profile.rPeakCount = static_cast<uint8_t>(info[3]->Uint32Value());
    profile.cycleType = static_cast<uint8_t>(info[4]->Uint32Value());
    profile.cycleCount = static_cast<uint8_t>(info[5]->Uint32Value());
    profile.chargeCurrent = static_cast<uint16_t>(info[6]->Uint32Value());
    profile.dischargeCurrent = static_cast<uint16_t>(info[7]->Uint32Value());
    profile.cellDischargeVoltage = static_cast<uint16_t>(info[8]->Uint32Value());
    profile.endVoltage = static_cast<uint16_t>(info[9]->Uint32Value());
    profile.trickleCurrent = static_cast<uint16_t>(info[10]->Uint32Value());

    obj->m_dev->startCharging(profile);
  } catch (std::range_error& e) {
    Nan::ThrowTypeError("unknown battery type");
    return;
  }
}

void DeviceWrap::GetSysInfo(const Nan::FunctionCallbackInfo<v8::Value> &info) {
  auto obj = Nan::ObjectWrap::Unwrap<DeviceWrap>(info.Holder());

  b6::SysInfo sinfo = obj->m_dev->getSysInfo();

  v8::Local<v8::Object> ret = Nan::New<v8::Object>();
  ret->Set(Nan::New("cycleTime").ToLocalChecked(), Nan::New(sinfo.cycleTime));
  ret->Set(Nan::New("timeLimit").ToLocalChecked(), Nan::New(sinfo.timeLimit));
  ret->Set(Nan::New("timeLimitEnabled").ToLocalChecked(), Nan::New(sinfo.timeLimitOn));
  ret->Set(Nan::New("capLimit").ToLocalChecked(), Nan::New(sinfo.capLimit));
  ret->Set(Nan::New("capLimitEnabled").ToLocalChecked(), Nan::New(sinfo.capLimitOn));
  ret->Set(Nan::New("lowDCLimit").ToLocalChecked(), Nan::New(sinfo.lowDCLimit));
  ret->Set(Nan::New("tempLimit").ToLocalChecked(), Nan::New(sinfo.tempLimit));
  ret->Set(Nan::New("voltage").ToLocalChecked(), Nan::New(sinfo.voltage));
  ret->Set(Nan::New("keyBuzzer").ToLocalChecked(), Nan::New(sinfo.keyBuzzer));
  ret->Set(Nan::New("sysBuzzer").ToLocalChecked(), Nan::New(sinfo.systemBuzzer));

  v8::Local<v8::Array> cells = Nan::New<v8::Array>();
  for (uint32_t i = 0; i < obj->m_dev->getCellCount(); i++) {
    cells->Set(i, Nan::New(sinfo.cells[i]));
  }

  ret->Set(Nan::New("cells").ToLocalChecked(), cells);

  info.GetReturnValue().Set(ret);
}

void DeviceWrap::GetChargeInfo(const Nan::FunctionCallbackInfo<v8::Value> &info) {
  auto obj = Nan::ObjectWrap::Unwrap<DeviceWrap>(info.Holder());

  try {
    b6::ChargeInfo cinfo = obj->m_dev->getChargeInfo();

    v8::Local<v8::Object> ret = Nan::New<v8::Object>();
    ret->Set(Nan::New("state").ToLocalChecked(), Nan::New(cinfo.state));
    ret->Set(Nan::New("tempExt").ToLocalChecked(), Nan::New(cinfo.tempExt));
    ret->Set(Nan::New("tempInt").ToLocalChecked(), Nan::New(cinfo.tempExt));
    ret->Set(Nan::New("capacity").ToLocalChecked(), Nan::New(cinfo.capacity));
    ret->Set(Nan::New("time").ToLocalChecked(), Nan::New(cinfo.time));
    ret->Set(Nan::New("voltage").ToLocalChecked(), Nan::New(cinfo.voltage));
    ret->Set(Nan::New("current").ToLocalChecked(), Nan::New(cinfo.current));
    ret->Set(Nan::New("impedance").ToLocalChecked(), Nan::New(cinfo.impendance));

    v8::Local<v8::Array> cells = Nan::New<v8::Array>();
    for (uint32_t i = 0; i < obj->m_dev->getCellCount(); i++) {
      cells->Set(i, Nan::New(cinfo.cells[i]));
    }

    ret->Set(Nan::New("cells").ToLocalChecked(), cells);

    info.GetReturnValue().Set(ret);
  } catch (b6::ChargingError& e) {
    Nan::ThrowError(e.message().c_str());
  }
}

void DeviceWrap::Listen(const Nan::FunctionCallbackInfo<v8::Value> &info) {
  auto obj = Nan::ObjectWrap::Unwrap<DeviceWrap>(info.Holder());
  try {
    obj->m_dev = new b6::Device();
  } catch (std::exception& e) {
    Nan::ThrowError(e.what());
  }
}
