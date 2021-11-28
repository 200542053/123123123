#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#define ITO2(n) QString("%1").arg(n, 2, 10, QChar('0'))

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    sysTimer = new QTimer(this);
    autoHaltTimer = new QTimer(this);

    tP = new Therapy;
    ui->graphicsView_mask->setVisible(true);
    ui->label_error->setVisible(false);
    deviceError = false;
    sysInit();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void Delay_MSec(unsigned int msec)
{
    QEventLoop loop;
    QTimer::singleShot(msec, &loop, SLOT(quit()));
    loop.exec();
}

void MainWindow::sysInit(){
    onSkin = false;
    powerStatus = false;
    currentTimerCount = 0;

    treat.treatCurrent = 0;
    treat.treatFreq = "100Hz";
    treat.treatTime = 0;
    treat.treatCurrent = 0;



    tP->therapyInit(treat);

    //ui->toolButton_power->setChecked(true);
    //ui->spinBox_batteryLevel->setValue(PowerLevel);

    ui->label_T20->setHidden(true);
    ui->label_T40->setHidden(true);
    ui->label_T60->setHidden(true);

    ui->label_current->setText("    1   2   3   4   5");
    ui->progressBar_current->setValue(0);

    if(!deviceError){
        ui->label_error->setHidden(true);
    }

    ui->label_LB5->setHidden(true);
    ui->label_LB2->setHidden(true);

    ui->lcdNumber_M->setDigitCount(2);
    ui->lcdNumber_M->display(ITO2(0));
    ui->lcdNumber_S->display(QString("%1").arg(0, 2, 10, QChar('0')));
//    ui->lcdNumber_mH->display(QString("%1").arg(0, 2, 10, QChar('0')));
//    ui->lcdNumber_mM->display(QString("%1").arg(0, 2, 10, QChar('0')));
//    ui->lcdNumber_mM->display("Alpha");
//    ui->lcdNumber_mH->display("0.5Hz");

    ui->checkBox_applySkin->setCheckable(true);

    connect(ui->checkBox_power, &QCheckBox::clicked, this, &MainWindow::powerChange);
    connect(tP->getTimer(), &QTimer::timeout, this, &MainWindow::flushTreatTimer);
    connect(ui->checkBox_applySkin, &QCheckBox::stateChanged, this, &MainWindow::applyToSkin);
    connect(sysTimer,&QTimer::timeout, this, &MainWindow::flushSysTimer);
    connect(autoHaltTimer,&QTimer::timeout, this, &MainWindow::flushAutoHaltTimer);

    PowerLevel = ui->spinBox_batteryLevel->value();
    flushBatIcon();

    autoHaltTimer->start(30000);

}

void MainWindow::sysHalt(){

    sysTimer->stop();
    tP->getTimer()->stop();
    ui->checkBox_applySkin->setChecked(false);
    ui->graphicsView_mask->setVisible(true);
    ui->checkBox_power->setChecked(false);
    ui->checkBox_applySkin->setCheckable(false);
    disconnect(tP->getTimer(), &QTimer::timeout, this, &MainWindow::flushTreatTimer);
    disconnect(ui->checkBox_applySkin, &QCheckBox::stateChanged, this, &MainWindow::applyToSkin);
}


void MainWindow::applyToSkin(int arg1){
    if(tP->getTime()!=0){
        tP->setCurrent(100);
        if(arg1 == Qt::Checked){
            sysTimer->stop();
            autoHaltTimer->stop();
            tP->setCurrent(100);
            ui->comboBox_waveType->setDisabled(true);
            ui->comboBox_freSel->setDisabled(true);
            tP->getTimer()->start(50);
        }else {
            tP->setCurrent(0);
            tP->getTimer()->stop();
            ui->comboBox_waveType->setDisabled(false);
            ui->comboBox_freSel->setDisabled(false);
            sysTimer->start(5000);
        }
        ui->progressBar_current->setValue(tP->getCurrent());
    }
}

void MainWindow::powerChange(bool value){
    if(ui->checkBox_power->isChecked() && !deviceError){
        sysInit();
        ui->graphicsView_mask->setVisible(false);
    }else{
        sysHalt();
        ui->graphicsView_mask->setVisible(true);
    }
}

void MainWindow::flushTreatTimer() {


    if (currentTimerCount <= 0) {
        if(tP->getTime()>0){
            tP->setTime(tP->getTime()-1);
            currentTimerCount = 59;
        }else{
            tP->getTimer()->stop();
            ui->checkBox_applySkin->setChecked(false);
            sysHalt();
        }

        ui->lcdNumber_M->display(ITO2(tP->getTime()));
    }
    else {
        currentTimerCount--;
        drainBattery();
    }

    ui->lcdNumber_S->display(ITO2(currentTimerCount));

}

void MainWindow::flushSysTimer(){
    sysTimer->stop();
    sysInit();
}

void MainWindow::flushAutoHaltTimer(){
    autoHaltTimer->stop();
    sysHalt();
}


void MainWindow::drainBattery(){
    PowerLevel -= tP->getCurrent()*100.0/(40*60*500);
    ui->spinBox_batteryLevel->setValue(PowerLevel);
    flushBatIcon();
}


void MainWindow::flushBatIcon(){
    qDebug() << PowerLevel;
    if(PowerLevel>80){
        ui->label_bat->setPixmap(QPixmap(":/icons/bat100"));
    }else if(PowerLevel>60){
        ui->label_bat->setPixmap(QPixmap(":/icons/bat75"));
    }else if(PowerLevel>40){
        ui->label_bat->setPixmap(QPixmap(":/icons/bat50"));
    }else if(PowerLevel>20){
        ui->label_bat->setPixmap(QPixmap(":/icons/bat25"));
    }else{
        ui->label_bat->setPixmap(QPixmap(":/icons/bat0"));
        if(PowerLevel<5){
            ui->label_LB5->setHidden(false);
        }else{
            ui->label_LB5->setHidden(true);
        }
        if(PowerLevel<2){
            ui->label_LB2->setHidden(false);
            tP->getTimer()->stop();
            for(int i=0;i<14;i++){
                Delay_MSec(500);
                ui->label_LB2->setHidden(!ui->label_LB2->isHidden());
            }
            sysHalt();

        }else{
            ui->label_LB2->setHidden(true);
        }
    }
}

void MainWindow::on_pushButton_clk_clicked()
{
    if(!ui->checkBox_applySkin->isChecked()){
        ui->label_T20->setHidden(true);
        ui->label_T40->setHidden(true);
        ui->label_T60->setHidden(true);

        switch (tP->getTime()) {
        case 0:
            tP->setTime(20);
            ui->label_T20->setHidden(false);
            break;
        case 20:
            tP->setTime(40);
            ui->label_T40->setHidden(false);
            break;
        case 40:
            tP->setTime(60);
            ui->label_T60->setHidden(false);
            break;
        case 60:
            tP->setTime(0);
            break;
        default:
            break;
        }
        ui->lcdNumber_M->display(ITO2(tP->getTime()));
    }
}


void MainWindow::on_pushButton_up_clicked()
{
    if(ui->checkBox_applySkin->isChecked()){
        tP->setCurrent(tP->getCurrent()+100);
        ui->progressBar_current->setValue(tP->getCurrent());
    }
}

void MainWindow::on_pushButton_down_clicked()
{
    if(ui->checkBox_applySkin->isChecked() && tP->getCurrent()>100){
        tP->setCurrent(tP->getCurrent()-100);
        ui->progressBar_current->setValue(tP->getCurrent());
    }
}

void MainWindow::on_pushButton_errorTrigger_clicked()
{
    sysHalt();
    deviceError = true;
    ui->label_error->setVisible(true);
}

void MainWindow::on_spinBox_batteryLevel_valueChanged(double arg1)
{
    PowerLevel = ui->spinBox_batteryLevel->value();
}

void MainWindow::on_comboBox_waveType_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "Alpha"){
        ui->label_waveType->setPixmap(QPixmap(":/icons/res/alpha.png"));
    }else if(arg1 == "Betta"){
        ui->label_waveType->setPixmap(QPixmap(":/icons/res/beta.png"));
    }else if(arg1 == "Gamma"){
        ui->label_waveType->setPixmap(QPixmap(":/icons/res/gamma.png"));
    }
}

void MainWindow::on_comboBox_freSel_currentIndexChanged(const QString &arg1)
{
    ui->label_Freq->setText(arg1);
}
