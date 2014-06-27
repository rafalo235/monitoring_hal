#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "valuegenerator.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setGenerator(ValueGenerator *);
    void setPortNumber(int);
    void setValueRange();
    void setValue(int);
    void setSliderValue(int);

private slots:

    void on_sensorRegValueSldr_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    ValueGenerator *_generator;

    QString PORT_LABEL = QString("Working on port %1");
};

#endif // MAINWINDOW_H
