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

void MainWindow::setValue(int value) {
    ui->regValueOutput->setText(QString("%1").arg(value));
    if (value > _generator->getAlarmValue()) {
        ui->regValueOutput->setStyleSheet("QLabel { color : red; }");
    } else {
        ui->regValueOutput->setStyleSheet("QLabel { color : black; }");
    }
}

void MainWindow::setSliderValue(int value) {
    ui->sensorRegValueSldr->setValue(value);
}

void MainWindow::on_sensorRegValueSldr_valueChanged(int value)
{
    uint16_t v = (uint16_t) value;
    _generator->setValue(v);
    this->setValue(value);
}
