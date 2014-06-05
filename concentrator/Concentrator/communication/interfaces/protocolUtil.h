#ifndef PROTOCOLUTIL_H
#define PROTOCOLUTIL_H

#include "communication/interfaces/protocol.h"

namespace NProtocol{


  bool operator>(const CData& data1, const CData& data2){

    switch(data1.getType()){
    case EValueType::INT_8:
      return data1.getValue().vInt8 > data2.getValue().vInt8;
    case EValueType::INT_16:
      return data1.getValue().vInt8 > data2.getValue().vInt16;
    case EValueType::INT_32:
      return data1.getValue().vInt8 > data2.getValue().vInt32;
    case EValueType::INT_64:
      return data1.getValue().vInt8 > data2.getValue().vInt64;
    case EValueType::UINT_8:
      return data1.getValue().vUInt8 > data2.getValue().vUInt8;
    case EValueType::UINT_16:
      return data1.getValue().vUInt8 > data2.getValue().vUInt16;
    case EValueType::UINT_32:
      return data1.getValue().vUInt8 > data2.getValue().vUInt32;
    case EValueType::UINT_64:
      return data1.getValue().vUInt8 > data2.getValue().vUInt64;
    case EValueType::DOUBLE_64:
      return data1.getValue().vDouble64 > data2.getValue().vDouble64;
    case EValueType::FLOAT_32:
      return data1.getValue().vFloat32 > data2.getValue().vFloat32;
    case EValueType::VOID:
      return false;
    }
    return true;
  }

  bool operator<(const CData& data1, const CData& data2){

    switch(data1.getType()){
    case EValueType::INT_8:
      return data1.getValue().vInt8 < data2.getValue().vInt8;
    case EValueType::INT_16:
      return data1.getValue().vInt8 < data2.getValue().vInt16;
    case EValueType::INT_32:
      return data1.getValue().vInt8 < data2.getValue().vInt32;
    case EValueType::INT_64:
      return data1.getValue().vInt8 < data2.getValue().vInt64;
    case EValueType::UINT_8:
      return data1.getValue().vUInt8 < data2.getValue().vUInt8;
    case EValueType::UINT_16:
      return data1.getValue().vUInt8 < data2.getValue().vUInt16;
    case EValueType::UINT_32:
      return data1.getValue().vUInt8 < data2.getValue().vUInt32;
    case EValueType::UINT_64:
      return data1.getValue().vUInt8 < data2.getValue().vUInt64;
    case EValueType::DOUBLE_64:
      return data1.getValue().vDouble64 < data2.getValue().vDouble64;
    case EValueType::FLOAT_32:
      return data1.getValue().vFloat32 < data2.getValue().vFloat32;
    case EValueType::VOID:
      return false;
    }
    return true;
  }




}
#endif // PROTOCOLUTIL_H
