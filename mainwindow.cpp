#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QStyleFactory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    tank_value1 = 0.0;
    tank_value2 = 0.0;
    tank_value3 = 0.0;
    //Boton de Activación
    connect(ui->start,
            SIGNAL(checkStateChanged(Qt::CheckState)),
            this,
            SLOT(activacion(Qt::CheckState)));
    connect(&timer,
            SIGNAL(timeout()),
            this,
            SLOT(UpdateTanks()));

    //LCDs
    connect(ui->q1,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(Updateq1Indicator(int)));
    connect(ui->q2,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(Updateq2Indicator(int)));
    connect(ui->q3,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(Updateq3Indicator(int)));
    connect(ui->q4,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(Updateq4Indicator(int)));

    //Line edits
    connect(ui->capacity1,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(check_numberc1(QString)));
    connect(ui->q1_max,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(check_numberq1(QString)));
    connect(ui->q2_max,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(check_numberq2(QString)));
    connect(ui->q3_max,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(check_numberq3(QString)));
    connect(ui->q4_max,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(check_numberq4(QString)));

    //Maximum Updates
    connect(ui->capacity1,
            SIGNAL(editingFinished()),
            this,
            SLOT(update_c1()));
    connect(ui->capacity2,
            SIGNAL(editingFinished()),
            this,
            SLOT(update_c2()));
    connect(ui->capacity3,
            SIGNAL(editingFinished()),
            this,
            SLOT(update_c3()));
    connect(ui->q1_max,
            SIGNAL(editingFinished()),
            this,
            SLOT(update_q1_max()));
    connect(ui->q2_max,
            SIGNAL(editingFinished()),
            this,
            SLOT(update_q2_max()));
    connect(ui->q3_max,
            SIGNAL(editingFinished()),
            this,
            SLOT(update_q3_max()));
    connect(ui->q4_max,
            SIGNAL(editingFinished()),
            this,
            SLOT(update_q4_max()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
//General
void MainWindow::activacion(Qt::CheckState check)
{
    if(check==2)
    {
        timer.start(100);
        ui->capacity1->setReadOnly(true);

        ui->q1_max->setReadOnly(true);
        ui->q1->setEnabled(true);
        ui->q2_max->setReadOnly(true);
        ui->q2->setEnabled(true);
        ui->q3_max->setReadOnly(true);
        ui->q3->setEnabled(true);
        ui->q4_max->setReadOnly(true);
        ui->q4->setEnabled(true);
    }
    else if(check==0)
    {
        timer.stop();
        ui->capacity1->setReadOnly(false);

        ui->q1_max->setReadOnly(false);
        ui->q1->setEnabled(false);
        ui->q2_max->setReadOnly(false);
        ui->q2->setEnabled(false);
        ui->q3_max->setReadOnly(false);
        ui->q3->setEnabled(false);
        ui->q4_max->setReadOnly(false);
        ui->q4->setEnabled(false);
    }
}
void MainWindow::UpdateTanks()
{
    //tanque principal
    double qin1 = ui->q1->value()/36000.0;
    double qout1 = ui->q2->value()/36000.0;
    tank_value1 = tank_value1 + qin1 - qout1;
    ui->tank1->setValue(tank_value1);
    check_qouts(tank_value1, ui->tank1->maximum(), ui->q2);
    check_qins(tank_value1, ui->tank1->maximum(), ui->q1);

    //tanques auxiliares
    double qin2;
    double qin3;
    bool enabledtank2;
    bool enabledtank3;
    enabledtank2 = check_tank(ui->tank2->maximum(),tank_value2,ui->enable_t2->checkState());
    enabledtank3 = check_tank(ui->tank3->maximum(),tank_value3,ui->enable_t3->checkState());
    if(enabledtank2 != enabledtank3)
    {
        if(enabledtank2 == true)
        {
            qin2 = qout1;
            qin3 = 0;
        }
        else
        {
            qin3 = qout1;
            qin2 = 0;
        }
    }
    else
    {
        if(enabledtank2 == true)
        {
            qin2 = qout1/2.0;
            qin3 = qout1/2.0;
        }
        else
        {
            ui->q2->setEnabled(false);
            ui->q2->setValue(0);
            qin2 = 0;
            qin3 = 0;
        }
    }
    double qout2 = ui->q3->value()/36000.0;
    double qout3 = ui->q4->value()/36000.0;
    tank_value2 = tank_value2 + qin2 - qout2;
    tank_value3 = tank_value3 + qin3 - qout3;
    ui->tank2->setValue(tank_value2);
    ui->tank3->setValue(tank_value3);
    check_qouts(tank_value2,ui->tank2->maximum(),ui->q3);
    check_qouts(tank_value3,ui->tank3->maximum(),ui->q4);
}
void MainWindow::check_qouts(double tv, double max, QDial *dial)
{
    if(tv < (max*0.1))
    {
        dial->setEnabled(false);
        dial->setValue(0);
    }
    else
    {
        dial->setEnabled(true);
    }
}
void MainWindow::check_qins(double tv, int max, QDial *dial)
{
    if(tv == max)
    {
        dial->setEnabled(false);
        dial->setValue(0);
    }
    else
    {
        dial->setEnabled(true);
    }
}
bool MainWindow::check_tank(int max, double tv, Qt::CheckState check)
{
    if(check == 2 && tv < max)
    {
        return true;
    }
    else
    {
        return false;
    }
}
void MainWindow::convertion(QString text, QLineEdit *line)
{
    if(text.toInt() == 0)
    {
        text.removeLast();
        line->setText(text);
    }
}
void MainWindow::update_clines(QString text, QProgressBar *t)
{
    t->setMaximum(text.toInt());
}
void MainWindow::update_qlines(QString text, QDial *d)
{
    d->setMaximum(text.toInt());
}

//Prueba
void MainWindow::TestTimer()
{
    qDebug() << "1000";
}

//LCDs
void MainWindow::Updateq1Indicator(int value)
{
    ui->q1_indicator->display(value);
}
void MainWindow::Updateq2Indicator(int value)
{
    ui->q2_indicator->display(value);
}
void MainWindow::Updateq3Indicator(int value)
{
    ui->q3_indicator->display(value);
}
void MainWindow::Updateq4Indicator(int value)
{
    ui->q4_indicator->display(value);
}

//Line edits
void MainWindow::check_numberc1(QString x)
{
    convertion(x, ui->capacity1);
}
void MainWindow::check_numberc2(QString x)
{
    convertion(x, ui->capacity2);
}
void MainWindow::check_numberc3(QString x)
{
    convertion(x, ui->capacity3);
}
void MainWindow::check_numberq1(QString x)
{
    convertion(x, ui->q1_max);
}
void MainWindow::check_numberq2(QString x)
{
    convertion(x, ui->q2_max);
}
void MainWindow::check_numberq3(QString x)
{
    convertion(x, ui->q3_max);
}
void MainWindow::check_numberq4(QString x)
{
    convertion(x, ui->q4_max);
}

//Update Maximums
void MainWindow::update_c1()
{
    update_clines(ui->capacity1->text(), ui->tank1);
}
void MainWindow::update_c2()
{
    update_clines(ui->capacity2->text(), ui->tank2);
}
void MainWindow::update_c3()
{
    update_clines(ui->capacity3->text(), ui->tank3);
}
void MainWindow::update_q1_max()
{
    update_qlines(ui->q1_max->text(), ui->q1);
}
void MainWindow::update_q2_max()
{
    update_qlines(ui->q2_max->text(), ui->q2);
}
void MainWindow::update_q3_max()
{
    update_qlines(ui->q3_max->text(), ui->q3);
}
void MainWindow::update_q4_max()
{
    update_qlines(ui->q4_max->text(), ui->q4);
}

