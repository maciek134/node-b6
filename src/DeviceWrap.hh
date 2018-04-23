//
// Created by klh on 4/22/18.
//

#ifndef PROJECT_DEVICE_HH
#define PROJECT_DEVICE_HH

#include <nan.h>
#include <b6/Device.hh>

class DeviceWrap : public Nan::ObjectWrap {
public:
  static void Init(v8::Local<v8::Object> exports);

private:
  explicit DeviceWrap();
  ~DeviceWrap() override;

  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void GetCoreType(v8::Local<v8::String>, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void GetUpgradeType(v8::Local<v8::String>, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void GetLanguageId(v8::Local<v8::String>, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void GetCustomerId(v8::Local<v8::String>, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void GetHWVersion(v8::Local<v8::String>, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void GetSWVersion(v8::Local<v8::String>, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void GetIsEncrypted(v8::Local<v8::String>, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void GetCellCount(v8::Local<v8::String>, const Nan::PropertyCallbackInfo<v8::Value>& info);

  static void GetDefaultChargeProfile(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void SetCycleTime(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void SetTimeLimit(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void SetCapacityLimit(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void SetTempLimit(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void SetBuzzers(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static void GetSysInfo(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void GetChargeInfo(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static void StopCharging(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void StartCharging(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static void Listen(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static std::string BatteryTypeToString(b6::BATTERY_TYPE type) {
    switch (type) {
      case b6::BATTERY_TYPE::LIPO: return "li-po";
      case b6::BATTERY_TYPE::LIIO: return "li-ion";
      case b6::BATTERY_TYPE::LIFE: return "li-fe";
      case b6::BATTERY_TYPE::LIHV: return "li-hv";
      case b6::BATTERY_TYPE::NIMH: return "ni-mh";
      case b6::BATTERY_TYPE::NICD: return "ni-cd";
      case b6::BATTERY_TYPE::PB: return "pb";
    }
  }

  static b6::BATTERY_TYPE StringToBatteryType(const std::string& type) {
    if (type == "li-po") return b6::BATTERY_TYPE::LIPO;
    if (type == "li-ion") return b6::BATTERY_TYPE::LIIO;
    if (type == "li-fe") return b6::BATTERY_TYPE::LIFE;
    if (type == "li-hv") return b6::BATTERY_TYPE::LIHV;
    if (type == "ni-mh") return b6::BATTERY_TYPE::NIMH;
    if (type == "ni-cd") return b6::BATTERY_TYPE::NICD;
    if (type == "pb") return b6::BATTERY_TYPE::PB;
    throw std::range_error("battery type unknown");
  }

  static std::string ChargingModeToString(b6::CHARGING_MODE_LI mode) {
    switch (mode) {
      case b6::CHARGING_MODE_LI::STANDARD: return "standard";
      case b6::CHARGING_MODE_LI::DISCHARGE: return "discharge";
      case b6::CHARGING_MODE_LI::STORAGE: return "storage";
      case b6::CHARGING_MODE_LI::FAST: return "fast";
      case b6::CHARGING_MODE_LI::BALANCE: return "balance";
    }
  }

  static std::string ChargingModeToString(b6::CHARGING_MODE_NI mode) {
    switch (mode) {
      case b6::CHARGING_MODE_NI::STANDARD: return "standard";
      case b6::CHARGING_MODE_NI::AUTO: return "auto";
      case b6::CHARGING_MODE_NI::DISCHARGE: return "discharge";
      case b6::CHARGING_MODE_NI::REPEAK: return "re-peak";
      case b6::CHARGING_MODE_NI::CYCLE: return "cycle";
    }
  }

  static std::string ChargingModeToString(b6::CHARGING_MODE_PB mode) {
    switch (mode) {
      case b6::CHARGING_MODE_PB::CHARGE: return "charge";
      case b6::CHARGING_MODE_PB::DISCHARGE: return "discharge";
    }
  }

  static b6::CHARGING_MODE_LI StringToChargingModeLi(const std::string& mode) {
    if (mode == "standard") return b6::CHARGING_MODE_LI::STANDARD;
    if (mode == "discharge") return b6::CHARGING_MODE_LI::DISCHARGE;
    if (mode == "storage") return b6::CHARGING_MODE_LI::STORAGE;
    if (mode == "fast") return b6::CHARGING_MODE_LI::FAST;
    if (mode == "balance") return b6::CHARGING_MODE_LI::BALANCE;
  }

  static b6::CHARGING_MODE_NI StringToChargingModeNi(const std::string& mode) {
    if (mode == "standard") return b6::CHARGING_MODE_NI::STANDARD;
    if (mode == "auto") return b6::CHARGING_MODE_NI::AUTO;
    if (mode == "discharge") return b6::CHARGING_MODE_NI::DISCHARGE;
    if (mode == "re-peak") return b6::CHARGING_MODE_NI::REPEAK;
    if (mode == "cycle") return b6::CHARGING_MODE_NI::CYCLE;
  }

  static b6::CHARGING_MODE_PB StringToChargingModePb(const std::string& mode) {
    if (mode == "charge") return b6::CHARGING_MODE_PB::CHARGE;
    if (mode == "discharge") return b6::CHARGING_MODE_PB::DISCHARGE;
  }

  static Nan::Persistent<v8::Function> constructor;
  b6::Device *m_dev;
};


#endif //PROJECT_DEVICE_HH
