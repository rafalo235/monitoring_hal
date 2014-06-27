#include "valuegenerator.h"

ValueGenerator::ValueGenerator() {
}

void ValueGenerator::setValue(uint16_t value) {
    QMutexLocker locker(&mutex);
    _value = value;
}

uint16_t ValueGenerator::getValue() {
    QMutexLocker locker(&mutex);
    return _value;
}

void ValueGenerator::setMinValue(uint16_t minValue) {
    QMutexLocker locker(&mutex);
    _minValue = minValue;
}

uint16_t ValueGenerator::getMinValue() {
    QMutexLocker locker(&mutex);
    return _minValue;
}

void ValueGenerator::setMaxValue(uint16_t maxValue) {
    QMutexLocker locker(&mutex);
    _maxValue = maxValue;
}

uint16_t ValueGenerator::getMaxValue() {
    QMutexLocker locker(&mutex);
    return _maxValue;
}

void ValueGenerator::setAlarmValue(uint16_t alarmValue) {
    QMutexLocker locker(&mutex);
    _alarmValue = alarmValue;
}

uint16_t ValueGenerator::getAlarmValue() {
    QMutexLocker locker(&mutex);
    return _alarmValue;
}

void ValueGenerator::setValueType(SensorValueType valueType) {
    QMutexLocker locker(&mutex);
    _valueType = valueType;
}

SensorValueType ValueGenerator::getValueType() {
    QMutexLocker locker(&mutex);
    return _valueType;
}

void ValueGenerator::close() {
}
