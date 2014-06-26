#include "mainwindow.h"
#include <QApplication>

#include "valuegenerator.h"
#include "sensor.h"

int main(int argc, char *argv[])
{
    int port = 1024;
    QApplication a(argc, argv);
    ValueGenerator generator;
    Sensor sensor(port, &generator);
    MainWindow w;

    /* Jeżeli podano port to zmieniamy z domyślnego */
    if (argc > 1) {
        port = atoi(argv[1]);
    }

    /* Ustawienie tablicy z wartościami zwracanymi */
    generator.initialize();

    /* Utworzenie kontekstu przypisanego do localhosta i podanego portu */
    sensor.listen();

    w.show();
    sensor.loop();


    return a.exec();
}
