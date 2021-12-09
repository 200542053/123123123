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
#include <QStandardItemModel>

#include "common.h"
#include "therapy.h"
#include "historyDBManager.h"

/* Class Purpose: The main window of the application, acts as the control of the application
 *
 * Data Members:
 *
 * -Ui::MainWindow *ui: A ui pointer for the UI in mainwindow
 * -QStandardItemModel *model: A widget for displaying the menus
 *
 * -QTimer* sysTimer: A timer pointer is used to reset the treatment after disconnecting the skin
 * -QTimer* autoHaltTimer: A timer pointer for automatic shutdown without operation
 *
 * -Therapy* tP: A pointer for the current running therapy
 * -HistoryDBManager* db: A database variable
 *
 * -bool powerStatus: whether the denas is turned on or off
 * -bool onSkin: whether the devices is on someones skin or off
 * -bool deviceError: A flag is used to characterize whether the device has internal damage (current exceeds 700uA)
 *
 * -TreatDataType treat: A structure for storing treatment information
 * -TreatRecordType rec: A structure for keeping treatment records = TreatDataType add StartTime
 *
 * -int currentTimerCount: the current count of whatever timer is running
 * -double PowerLevel:  device battery value
 *
 *
 * Class Functions:
 * void sysInit(): Device startup function, used to display screen UI and initialize related variables
 * void sysHalt(): Device shutdown function, which simulates the operations required to shut down the device
 * void drainBattery(): Contains an equation of how much to drain the battery by then calls another function to change it
 * void flushBatIcon(): Update the icon of the device to simulate the LCD screen
 * void updateTableView(): Refresh the history display interface
 *
 * void applyToSkin(int): Starts/stops the therapy timer and update related UI
 * void powerChange(bool): Handle device switch button press event
 * void flushTreatTimer(): Updates the timer displayed on the screen
 * void flushSysTimer(): Close sysTimer and reset therapy
 * void flushAutoHaltTimer(): Turn off the device and turn off AutoHaltTimer
 * void on_pushButton_clk_clicked():  Slot to change the treatment timing
 * void on_pushButton_up_clicked(): Slot to change the current up
 * void on_pushButton_down_clicked(): Slot to change the current down
 * void on_pushButton_errorTrigger_clicked(): Slot to trigger internal damage (current exceeds 700uA)
 * void on_spinBox_batteryLevel_valueChanged(double arg1): Slot to change the battery level from the admin panel
 * void on_comboBox_waveType_currentIndexChanged(const QString &arg1): Slot to change the treatment output waveform
 * void on_comboBox_freSel_currentIndexChanged(const QString &arg1): Slot to change the treatment output current
 * void on_pushButton_record_clicked(): Slot to keep records of this treatment(Only available in treatment)
 * void on_pushButton_clearHistory_clicked(): Slot to clear treatment records

 */


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
    QStandardItemModel *model;

    bool powerStatus;
    bool onSkin;
    bool deviceError;

    double PowerLevel;
    int currentTimerCount;

    TreatDataType treat;
    TreatRecordType rec;
    Therapy* tP;
    HistoryDBManager* db;

    void sysInit();
    void sysHalt();
    void drainBattery();
    void flushBatIcon();
    void updateTableView();

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
    void on_pushButton_record_clicked();

    void on_pushButton_clearHistory_clicked();
};
#endif // MAINWINDOW_H
