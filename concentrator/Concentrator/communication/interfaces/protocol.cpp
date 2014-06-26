#include "protocol.h"

namespace NProtocol {

  const uint32_t CData::headerSize = sizeof(type);

  const uint32_t CSensorData::headerSize = sizeof(idData) + sizeof(idSensor) +
                                     sizeof(timeStamp) +
                                     sizeof(sensorState) + sizeof(dangerLevel);

  const uint32_t CMonitorData::headerSize = sizeof(sendTime) + sizeof(sensorsAmount) +
                                      sizeof(uint32_t);

  const uint32_t CConfigurationValue::headerSize = sizeof(idSensor) + sizeof(configurationType);

  const uint32_t CConfiguration::headerSize = sizeof(uint8_t);

  const uint32_t CConfigurationResponse::headerSize = sizeof(status) + sizeof(idRequestPackage);

  const uint32_t CRequest::headerSize = sizeof(idSensor) + sizeof(configurationType);
  const uint32_t CServerRequest::headerSize = sizeof(uint8_t);
  const uint32_t CServerResponse::headerSize = sizeof(status) + sizeof(idRequestPackage);
  const uint32_t CProtocol::headerSize = sizeof(version) + sizeof(size) +
                                   sizeof(idConcentrator) + sizeof(idPackage) + sizeof(type) + sizeof(crc);

  //!
  //! \brief operator> Operator wiekszosci przyjmujacy ze parametry maja dane tego samego typu
  //! \param data2 prawa strona
  //! \return true gdy lewa strona > prawa strona
  bool CData::operator>(const CData& data2) const
  {

    switch(type){
    case EValueType::INT_8:
      return value.vInt8 > data2.value.vInt8;
    case EValueType::INT_16:
      return value.vInt8 > data2.value.vInt16;
    case EValueType::INT_32:
      return value.vInt8 > data2.value.vInt32;
    case EValueType::INT_64:
      return value.vInt8 > data2.value.vInt64;
    case EValueType::UINT_8:
      return value.vUInt8 > data2.value.vUInt8;
    case EValueType::UINT_16:
      return value.vUInt8 > data2.value.vUInt16;
    case EValueType::UINT_32:
      return value.vUInt8 > data2.value.vUInt32;
    case EValueType::UINT_64:
      return value.vUInt8 > data2.value.vUInt64;
    case EValueType::DOUBLE_64:
      return value.vDouble64 > data2.value.vDouble64;
    case EValueType::FLOAT_32:
      return value.vFloat32 > data2.value.vFloat32;
    case EValueType::VOID:
      return false;
    }
    return true;
  }

  //! \brief operator< Operator mniejszosci przyjmujacy ze parametry maja dane tego samego typu
  //! \param data2 prawa strona
  //! \return true gdy lewa strona < prawa strona
  bool CData::operator<(const CData& data2) const
  {

    switch(type){
    case EValueType::INT_8:
      return value.vInt8 < data2.value.vInt8;
    case EValueType::INT_16:
      return value.vInt8 < data2.value.vInt16;
    case EValueType::INT_32:
      return value.vInt8 < data2.value.vInt32;
    case EValueType::INT_64:
      return value.vInt8 < data2.value.vInt64;
    case EValueType::UINT_8:
      return value.vUInt8 < data2.value.vUInt8;
    case EValueType::UINT_16:
      return value.vUInt8 < data2.value.vUInt16;
    case EValueType::UINT_32:
      return value.vUInt8 < data2.value.vUInt32;
    case EValueType::UINT_64:
      return value.vUInt8 < data2.value.vUInt64;
    case EValueType::DOUBLE_64:
      return value.vDouble64 < data2.value.vDouble64;
    case EValueType::FLOAT_32:
      return value.vFloat32 < data2.value.vFloat32;
    case EValueType::VOID:
      return false;
    }
    return true;
  }

  //! \brief operator== Operator porownania przyjmujacy ze parametry maja dane tego samego typu
  //! \param data2 prawa strona
  //! \return true gdy lewa strona == prawa strona
  bool CData::operator==(const CData& data2) const
  {

    switch(type){
    case EValueType::INT_8:
      return value.vInt8 == data2.value.vInt8;
    case EValueType::INT_16:
      return value.vInt8 == data2.value.vInt16;
    case EValueType::INT_32:
      return value.vInt8 == data2.value.vInt32;
    case EValueType::INT_64:
      return value.vInt8 == data2.value.vInt64;
    case EValueType::UINT_8:
      return value.vUInt8 == data2.value.vUInt8;
    case EValueType::UINT_16:
      return value.vUInt8 == data2.value.vUInt16;
    case EValueType::UINT_32:
      return value.vUInt8 == data2.value.vUInt32;
    case EValueType::UINT_64:
      return value.vUInt8 == data2.value.vUInt64;
    case EValueType::DOUBLE_64:
      return value.vDouble64 == data2.value.vDouble64;
    case EValueType::FLOAT_32:
      return value.vFloat32 == data2.value.vFloat32;
    case EValueType::VOID:
      return true;
    }
    return false;
  }
}
