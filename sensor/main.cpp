#include "mainwindow.h"
#include <QApplication>

#include "valuegenerator.h"
#include "sensor.h"

//
// sensor <PORT> <MINIMUM_SENSOR_VALUE> <MAXIMUM> <ALARM_SENSOR_VALUE> <INITIAL_VALUE>
//
int main(int argc, char *argv[])
{
    int port = 1502;
    uint16_t low = 0;
    uint16_t high = 50;
    uint16_t alarm = 30;
    uint16_t init = 5;

    /* Jeżeli podano port to zmieniamy z domyślnego */
    if (argc > 1) {
        port = atoi(argv[1]);
    }
    if (argc > 5) {
        low = atoi(argv[2]);
        high = atoi(argv[3]);
        alarm = atoi(argv[4]);
        init = atoi(argv[5]);
    }

    QApplication a(argc, argv);
    ValueGenerator generator;
    Sensor sensor(port, &generator);
    MainWindow w;


    generator.setMinValue(low);
    generator.setMaxValue(high);
    generator.setAlarmValue(alarm);
    generator.setValue(init);

    w.setGenerator(&generator); // musi być przed value range
    w.setPortNumber(port);
    w.setValueRange();
    w.setValue(init);
    w.setSliderValue(init);
    w.show();

    sensor.start();


    return a.exec();
}
