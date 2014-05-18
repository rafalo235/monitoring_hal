#include "SensorConfiguration.h"
namespace NEngine {

  std::ofstream& operator <<(std::ofstream& stream, const EValueType& value)
  {
    stream<<static_cast<int8_t>(value);
    return stream;
  }

  std::ofstream& operator <<(std::ofstream& stream, const SData& data)
  {
    stream<<data.type;
    switch(data.type){
    case EValueType::INT_8:
      stream<<data.value.vInt8;
      break;
    case EValueType::UINT_8:
      stream<<data.value.vUInt8;
      break;
    case EValueType::INT_16:
      stream<<data.value.vInt16;
      break;
    case EValueType::UINT_16:
      stream<<data.value.vUInt16;
      break;
    case EValueType::INT_32:
      stream<<data.value.vInt32;
      break;
    case EValueType::UINT_32:
      stream<<data.value.vUInt32;
      break;
    case EValueType::INT_64:
      stream<<data.value.vInt64;
      break;
    case EValueType::UINT_64:
      stream<<data.value.vUInt64;
      break;
    case EValueType::FLOAT_32:
      stream<<data.value.vFloat32;
      break;
    case EValueType::DOUBLE_64:
      stream<<data.value.vDouble64;
      break;
    case EValueType::VOID:
      stream<<data.value.vVoid8;
      break;
    }
    return stream;
  }

  std::ifstream& operator>>(std::ifstream& stream, EValueType& value)
  {
    int8_t buf;
    stream>>buf;
    value = static_cast<EValueType>(buf);
    return stream;
  }

  std::ifstream& operator >>(std::ifstream& stream, SData& data)
  {
    stream>>data.type;
    switch(data.type){
    case EValueType::INT_8:
      stream>>data.value.vInt8;
      break;
    case EValueType::UINT_8:
      stream>>data.value.vUInt8;
      break;
    case EValueType::INT_16:
      stream>>data.value.vInt16;
      break;
    case EValueType::UINT_16:
      stream>>data.value.vUInt16;
      break;
    case EValueType::INT_32:
      stream>>data.value.vInt32;
      break;
    case EValueType::UINT_32:
      stream>>data.value.vUInt32;
      break;
    case EValueType::INT_64:
      stream>>data.value.vInt64;
      break;
    case EValueType::UINT_64:
      stream>>data.value.vUInt64;
      break;
    case EValueType::FLOAT_32:
      stream>>data.value.vFloat32;
      break;
    case EValueType::DOUBLE_64:
      stream>>data.value.vDouble64;
      break;
    case EValueType::VOID:
      stream>>data.value.vVoid8;
      break;
    }
    return stream;
  }

  std::ifstream& operator >>(std::ifstream& stream, CSensorConfiguration& obj)
  {
    stream>>obj.turnOn;
    stream>>obj.warningLvl;
    stream>>obj.dangerLvl;
    return stream;
  }


  std::ofstream& operator <<(std::ofstream& stream, const CSensorConfiguration& obj)
  {
    stream<<obj.turnOn;
    stream<<obj.warningLvl;
    stream<<obj.dangerLvl;
    return stream;
  }


}
