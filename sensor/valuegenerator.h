#ifndef VALUEGENERATOR_H
#define VALUEGENERATOR_H

#include <cstdint>
#include <random>
#include <QMutex>
#include <QMutexLocker>

typedef std::mt19937 MyRNG;

enum SensorValueType {
    INTEGER
};

class ValueGenerator
{
public:
    ValueGenerator();

    void setValue(uint16_t);
    uint16_t getValue();

    void setMinValue(uint16_t);
    uint16_t getMinValue();

    void setMaxValue(uint16_t);
    uint16_t getMaxValue();

    void setAlarmValue(uint16_t);
    uint16_t getAlarmValue();

    void setValueType(SensorValueType valueType);
    SensorValueType getValueType();

    void close();
private:
    uint16_t _value;
    uint16_t _minValue;
    uint16_t _maxValue;
    uint16_t _alarmValue;

    QMutex mutex;

    SensorValueType _valueType = INTEGER;
};

#endif // VALUEGENERATOR_H
