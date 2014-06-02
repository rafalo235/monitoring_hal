#ifndef PROTOCOLUTIL_H
#define PROTOCOLUTIL_H

#include "communication/interfaces/protocol.h"

namespace NProtocol{


  bool operator>(const SData& data1, const SData& data2){

    switch(data1.type){
    case EValueType::INT_8:
      return data1.value.vInt8 > data2.value.vInt8;
    case EValueType::INT_16:
      return data1.value.vInt8 > data2.value.vInt16;
    case EValueType::INT_32:
      return data1.value.vInt8 > data2.value.vInt32;
    case EValueType::INT_64:
      return data1.value.vInt8 > data2.value.vInt64;
    case EValueType::UINT_8:
      return data1.value.vUInt8 > data2.value.vUInt8;
    case EValueType::UINT_16:
      return data1.value.vUInt8 > data2.value.vUInt16;
    case EValueType::UINT_32:
      return data1.value.vUInt8 > data2.value.vUInt32;
    case EValueType::UINT_64:
      return data1.value.vUInt8 > data2.value.vUInt64;
    case EValueType::DOUBLE_64:
      return data1.value.vDouble64 > data2.value.vDouble64;
    case EValueType::FLOAT_32:
      return data1.value.vFloat32 > data2.value.vFloat32;
    case EValueType::VOID:
      return false;
    }
    return true;
  }

  bool operator<(const SData& data1, const SData& data2){

    switch(data1.type){
    case EValueType::INT_8:
      return data1.value.vInt8 < data2.value.vInt8;
    case EValueType::INT_16:
      return data1.value.vInt8 < data2.value.vInt16;
    case EValueType::INT_32:
      return data1.value.vInt8 < data2.value.vInt32;
    case EValueType::INT_64:
      return data1.value.vInt8 < data2.value.vInt64;
    case EValueType::UINT_8:
      return data1.value.vUInt8 < data2.value.vUInt8;
    case EValueType::UINT_16:
      return data1.value.vUInt8 < data2.value.vUInt16;
    case EValueType::UINT_32:
      return data1.value.vUInt8 < data2.value.vUInt32;
    case EValueType::UINT_64:
      return data1.value.vUInt8 < data2.value.vUInt64;
    case EValueType::DOUBLE_64:
      return data1.value.vDouble64 < data2.value.vDouble64;
    case EValueType::FLOAT_32:
      return data1.value.vFloat32 < data2.value.vFloat32;
    case EValueType::VOID:
      return false;
    }
    return true;
  }
}
#endif // PROTOCOLUTIL_H
