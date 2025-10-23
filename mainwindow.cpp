#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    tank_value1 = 0.0;
    tank_value2 = 0.0;
    tank_value3 = 0.0;

    //General
    connect(ui->start,
            SIGNAL(checkStateChanged(Qt::CheckState)),
            this,
            SLOT(activacion(Qt::CheckState)));
    connect(&timer,
            SIGNAL(timeout()),
            this,
            SLOT(UpdateTanks()));
    connect(ui->enable_t2,
            SIGNAL(checkStateChanged(Qt::CheckState)),
            this,
            SLOT(change_text_enable2(Qt::CheckState)));
    connect(ui->enable_t3,
            SIGNAL(checkStateChanged(Qt::CheckState)),
            this,
            SLOT(change_text_enable3(Qt::CheckState)));
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

    //Upper buttons
    connect(ui->actionSave,
            SIGNAL(triggered(bool)),
            this,
            SLOT(save_file()));
    connect(ui->actionSave_as,
            SIGNAL(triggered(bool)),
            this,
            SLOT(save_file_as()));
    connect(ui->actionOpen_File1,
            SIGNAL(triggered(bool)),
            this,
            SLOT(open_file()));
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
        ui->start->setText("Parar");
        timer.start(1000);
        ui->capacity1->setReadOnly(true);
        ui->actionOpen_file->setChecked(true);

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
        ui->start->setText("Arranque");
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
    double qin1 = ui->q1->value()/3600.0;
    double qout1 = ui->q2->value()/3600.0;
    tank_value1 = tank_value1 + qin1 - qout1;
    tank_value1 = isoverflow(ui->tank1->maximum(), tank_value1);
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
    double qout2 = ui->q3->value()/3600.0;
    double qout3 = ui->q4->value()/3600.0;
    tank_value2 = tank_value2 + qin2 - qout2;
    tank_value3 = tank_value3 + qin3 - qout3;
    tank_value2 = isoverflow(ui->tank2->maximum(), tank_value2);
    tank_value3 = isoverflow(ui->tank3->maximum(), tank_value3);
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
        ui->tank1->setValue(tv);
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
    if(text.isEmpty())
    {
        t->setMaximum(1000);
    }
    else
    {
        t->setMaximum(text.toInt());
    }
}
void MainWindow::update_qlines(QString text, QDial *d)
{
    if(text.isEmpty())
    {
        d->setMaximum(1000);
    }
    else
    {
        d->setMaximum(text.toInt());
    }
}
double MainWindow::isoverflow(int max, double tv)
{
    if(tv >= max)
    {
        tv = max;
        return tv;
    }
    else if(tv <= 0)
    {
        return 0;
    }
    else
    {
        return tv;
    }
}
void MainWindow::change_text_enable2(Qt::CheckState check)
{
    if(check==2)
    {
        ui->enable_t2->setText("Deshabilitar");
    }
    else if(check==0)
    {
        ui->enable_t2->setText("Habilitar");
    }
}
void MainWindow::change_text_enable3(Qt::CheckState check)
{
    if(check==2)
    {
        ui->enable_t3->setText("Deshabilitar");
    }
    else if(check==0)
    {
        ui->enable_t3->setText("Habilitar");
    }
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

//Upper buttons
void MainWindow::save_file()
{
    if(curFile.isEmpty())
    {
        save_file_as();
    }

    QFile file(curFile);
    if(file.open(QFile::WriteOnly))
    {
        QDataStream code(&file);
        CheckBoxs.append(ui->start);
        CheckBoxs.append(ui->enable_t2);
        CheckBoxs.append(ui->enable_t3);
        for(int i = 0 ; i < 3 ; i++)
        {
            code << CheckBoxs[i]->isChecked();
        }

        tanks.append(ui->tank1);
        tanks.append(ui->tank2);
        tanks.append(ui->tank3);
        for(int i = 0 ; i < 3 ; i++)
        {
            code << tanks[i]->maximum();
            code << tanks[i]->value();
        }
        dials.append(ui->q1);
        dials.append(ui->q2);
        dials.append(ui->q3);
        dials.append(ui->q4);
        for(int i = 0 ; i < 4 ; i++)
        {
            code << dials[i]->maximum();
            code << dials[i]->value();
        }

        lines.append(ui->capacity1);
        lines.append(ui->capacity2);
        lines.append(ui->capacity3);
        lines.append(ui->q1_max);
        lines.append(ui->q2_max);
        lines.append(ui->q3_max);
        lines.append(ui->q4_max);
        for(int i = 0 ; i < 7 ; i++)
        {
            code << lines[i]->text();
        }
        file.close();
    }
}

void MainWindow::save_file_as()
{
    timer.stop();
    QString filename = QFileDialog::getSaveFileName(this, "Save File as", "", "Dat Files (*.dat);;All files (*.*)");
    if(!filename.isEmpty())
    {
        curFile = filename;
        save_file();
    }
    if(ui->start->isChecked())
    {
        timer.start(1000);
    }
}

void MainWindow::open_file()
{
    curFile = QFileDialog::getOpenFileName(this,"Open File", "", "Dat Files (*.dat);;All files (*.*)");
    QFile file(curFile);
    if(file.open(QFile::ReadOnly))
    {
        QDataStream code(&file);
        bool ch1;
        bool ch2;
        bool ch3;
        code>> ch1
            >> ch2
            >> ch3;
        int t1_max;
        int t1;
        int t2_max;
        int t2;
        int t3_max;
        int t3;
        code>> t1_max
            >> t1
            >> t2_max
            >> t2
            >> t3_max
            >> t3;
        int d1_max;
        int d1;
        int d2_max;
        int d2;
        int d3_max;
        int d3;
        int d4_max;
        int d4;
        code>> d1_max
            >> d1
            >> d2_max
            >> d2
            >> d3_max
            >> d3
            >> d4_max
            >> d4;
        QString l1;
        QString l2;
        QString l3;
        QString l4;
        QString l5;
        QString l6;
        QString l7;
        code>> l1
            >> l2
            >> l3
            >> l4
            >> l5
            >> l6
            >> l7;

        tank_value1 = t1*1.0;
        tank_value2 = t2*1.0;
        tank_value3 = t3*1.0;
        ui->start->setChecked(ch1);
        ui->enable_t2->setChecked(ch2);
        ui->enable_t3->setChecked(ch3);
        ui->tank1->setMaximum(t1_max);
        ui->tank1->setValue(tank_value1);
        ui->tank2->setMaximum(t2_max);
        ui->tank2->setValue(tank_value2);
        ui->tank3->setMaximum(t3_max);
        ui->tank3->setValue(tank_value3);
        ui->q1->setMaximum(d1_max);
        ui->q1->setValue(d1);
        ui->q2->setMaximum(d2_max);
        ui->q2->setValue(d2);
        ui->q3->setMaximum(d3_max);
        ui->q3->setValue(d3);
        ui->q4->setMaximum(d4_max);
        ui->q4->setValue(d4);
        ui->capacity1->setText(l1);
        ui->capacity2->setText(l2);
        ui->capacity3->setText(l3);
        ui->q1_max->setText(l4);
        ui->q2_max->setText(l5);
        ui->q3_max->setText(l6);
        ui->q4_max->setText(l7);
        file.close();
    }
}

