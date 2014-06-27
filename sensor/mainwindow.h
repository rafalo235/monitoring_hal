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

private slots:
    void on_sensorRegValueSldr_actionTriggered(int action);

private:
    Ui::MainWindow *ui;
    ValueGenerator *_generator;

    QString PORT_LABEL = QString("Working on port %1");
};

#endif // MAINWINDOW_H
