#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSpinBox>
#include <QListWidget>
#include <QGraphicsView>
#include <QTimer>
#include <QStatusBar>
#include <QVector>
#include <QtGlobal>

#include "common.h"
#include "therapy.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QTimer* sysTimer;
    QTimer* autoHaltTimer;

    bool powerStatus;
    bool onSkin;
    bool deviceError;

    double PowerLevel;
    int currentTimerCount;

    TreatDataType treat;
    Therapy* tP;
    void sysInit();
    void sysHalt();
    void drainBattery();
    void flushBatIcon();

private slots:
    void applyToSkin(int);
    void powerChange(bool);
    void flushTreatTimer();
    void flushSysTimer();
    void flushAutoHaltTimer();

    void on_pushButton_clk_clicked();
    void on_pushButton_up_clicked();
    void on_pushButton_down_clicked();
    void on_pushButton_errorTrigger_clicked();
    void on_spinBox_batteryLevel_valueChanged(double arg1);
    void on_comboBox_waveType_currentIndexChanged(const QString &arg1);
    void on_comboBox_freSel_currentIndexChanged(const QString &arg1);
};
#endif // MAINWINDOW_H
