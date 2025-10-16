#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Qtimer>
#include <QString>
#include <QDial>
#include <QLineEdit>
#include <QProgressBar>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    //general
    void activacion(Qt::CheckState check);
    void UpdateTanks();
    void check_qouts(double tv, int max, QDial *dial);
    void check_qins(double tv, int max, QDial *dial);
    bool check_tank(int max, double tv, const Qt::CheckState check);
    void convertion(QString text, QLineEdit *line);
    void update_clines(QString text, QProgressBar *t);
    void update_qlines(QString text, QDial *d);

    //prueba
    void TestTimer();

    //LCDs
    void Updateq1Indicator(int value);
    void Updateq2Indicator(int value);
    void Updateq3Indicator(int value);
    void Updateq4Indicator(int value);

    //line edits
    void check_numberc1(QString x);
    void check_numberc2(QString x);
    void check_numberc3(QString x);
    void check_numberq1(QString x);
    void check_numberq2(QString x);
    void check_numberq3(QString x);
    void check_numberq4(QString x);

    //Update Maximums
    void update_c1();
    void update_c2();
    void update_c3();
    void update_q1_max();
    void update_q2_max();
    void update_q3_max();
    void update_q4_max();

private:
    Ui::MainWindow *ui;
    QTimer timer;
    double tank_value1;
    double tank_value2;
    double tank_value3;
};
#endif // MAINWINDOW_H
