#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setGenerator(ValueGenerator *generator) {
    _generator = generator;
}

void MainWindow::setPortNumber(int port) {
    ui->portLabel->setText(MainWindow::PORT_LABEL.arg(port));
}

void MainWindow::setValueRange() {
    ui->sensorRegValueSldr->setRange(_generator->getMinValue(), _generator->getMaxValue());
}

void MainWindow::on_sensorRegValueSldr_actionTriggered(int value) {
    _generator->setValue(value);
    ui->regValueOutput->setText(QString("%1").arg(value));
    if (value > _generator->getAlarmValue()) {
        ui->regValueOutput->setStyleSheet("QLabel { color : red; }");
    } else {
        ui->regValueOutput->setStyleSheet("QLabel { color : black; }");
    }
}
