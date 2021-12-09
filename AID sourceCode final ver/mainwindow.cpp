#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

//Construct LCD_NUM display string, add leading 0 before 1 digit
#define ITO2(n) QString("%1").arg(n, 2, 10, QChar('0'))

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    QFile file(":/qss/res/ManjaroMix.qss");
//    if (!file.open(QIODevice::ReadOnly))
//        exit(0);

//    QTextStream in(&file);
//    QString css = in.readAll();
//    qApp->setStyleSheet(css);

    // Create system timer
    sysTimer = new QTimer(this);
    autoHaltTimer = new QTimer(this);

    // Create the therapies
    tP = new Therapy;

    // Create database connection
    db = new HistoryDBManager;

    /*In order to quickly close the virtual LCD screen,
     * choose to place an opaque mask with the same color as the background color of
     * the screen on the existing screen position. When the mask is turned on,
     * the screen that the user observes is completely black.
     * When the mask is closed, the user interface can be displayed normally.
    */

    // Display the screen mask, turn off the equipment damage sign display
    ui->graphicsView_mask->setVisible(true);
    ui->label_error->setVisible(false);
    deviceError = false;

    // Connect the system power button
    //sysInit();
    connect(ui->checkBox_power, &QCheckBox::clicked, this, &MainWindow::powerChange);
}

MainWindow::~MainWindow()
{
    delete tP;
    delete db;
    delete ui;
}

/* Purpose:
 * Non-blocking millisecond delay, used for icon flash timing
 *
 * Parameters:
 * msec: How many milliseconds to delay
 *
 * Return:
 * None
 *
*/
void Delay_MSec(unsigned int msec)
{
    QEventLoop loop;
    QTimer::singleShot(msec, &loop, SLOT(quit()));
    loop.exec();
}

void MainWindow::sysInit(){
    // Set Initial system status
    onSkin = false;
    powerStatus = false;
    currentTimerCount = 0;

    // Set Initial treat args
    treat.treatCurrent = 0;
    treat.treatFreq = "100Hz";
    treat.treatTime = 0;
    treat.treatCurrent = 0;

    // Initialize the therapy class
    tP->therapyInit(treat);

    //  Initialize the virtual LCD screen icon
    ui->label_T20->setHidden(true);
    ui->label_T40->setHidden(true);
    ui->label_T60->setHidden(true);
    ui->label_LB5->setHidden(true);
    ui->label_LB2->setHidden(true);

    ui->label_current->setText("    1   2   3   4   5");
    ui->progressBar_current->setValue(0);

    if(!deviceError){
        ui->label_error->setHidden(true);
    }

    ui->lcdNumber_M->setDigitCount(2);
    ui->lcdNumber_M->display(ITO2(0));
    ui->lcdNumber_S->display(QString("%1").arg(0, 2, 10, QChar('0')));

    ui->checkBox_applySkin->setCheckable(true);
    ui->pushButton_record->setEnabled(true);

    // Initialize the treatment history display table
    model = new QStandardItemModel();
    model->setColumnCount(5);

    model->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("date"));
    model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("req"));
    model->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("waveType"));
    model->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("current"));
    model->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("theatTime"));
    ui->tableView->setModel(model);
    ui->tableView->setColumnWidth(0, 180);

    // device interface signal connections
    connect(ui->checkBox_power, &QCheckBox::clicked, this, &MainWindow::powerChange);
    connect(tP->getTimer(), &QTimer::timeout, this, &MainWindow::flushTreatTimer);
    connect(ui->checkBox_applySkin, &QCheckBox::stateChanged, this, &MainWindow::applyToSkin);
    connect(sysTimer,&QTimer::timeout, this, &MainWindow::flushSysTimer);
    connect(autoHaltTimer,&QTimer::timeout, this, &MainWindow::flushAutoHaltTimer);

    // Initialize the battery value
    PowerLevel = ui->spinBox_batteryLevel->value();

    // Update all icon display
    flushBatIcon();

    // Update history record display
    updateTableView();

    // Automatic shutdown for 30s without operation, timing on
    autoHaltTimer->start(30000);

}

void MainWindow::sysHalt(){

    // Turn off the system timer and treatment
    sysTimer->stop();
    tP->getTimer()->stop();

    // Close the display interface
    ui->checkBox_applySkin->setChecked(false);
    ui->graphicsView_mask->setVisible(true);
    ui->checkBox_power->setChecked(false);
    ui->checkBox_applySkin->setCheckable(false);
    model->clear();

    // Disconnect non-power button response
    disconnect(tP->getTimer(), &QTimer::timeout, this, &MainWindow::flushTreatTimer);
    disconnect(ui->checkBox_applySkin, &QCheckBox::stateChanged, this, &MainWindow::applyToSkin);

}


void MainWindow::applyToSkin(int arg1){
    if(tP->getTime()!=0){

        //tP->setCurrent(100);

        if(arg1 == Qt::Checked){    // When connected to the skin
            sysTimer->stop();
            autoHaltTimer->stop();
            // Set the default output 100uA current
            tP->setCurrent(100);
            // Set treatment parameters
            tP->setWaveType(ui->comboBox_waveType->currentText());
            tP->setFreq(ui->comboBox_freSel->currentText());

            // Turn off current and waveform change options
            ui->comboBox_waveType->setDisabled(true);
            ui->comboBox_freSel->setDisabled(true);

            // Record the start time of treatment
            rec.startTime = QDateTime::currentDateTime();

            // Start treatment countdown
            tP->getTimer()->start(50);
        }else {                     // When disconnected to the skin
            // Turn off the current output and stop treatment countdown
            tP->setCurrent(0);
            tP->getTimer()->stop();
            // Run waveform and frequency selection
            ui->comboBox_waveType->setDisabled(false);
            ui->comboBox_freSel->setDisabled(false);
            // The timeout for disconnecting the skin starts
            sysTimer->start(5000);
        }

        // Update current display components
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
    //qDebug() << PowerLevel;
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
        rec.treatData.treatTime = tP->getTime();
    }
    autoHaltTimer->start(30000);
}

void MainWindow::updateTableView()
{
    QVector<TreatRecordType> ret = db->getTreatHistoy();
    int dbLen = ret.length();
    model->removeRows(0, model->rowCount());
    for(int i = 0; i<dbLen ; i++){
        this->model->setItem(i, 0, new QStandardItem(ret[i].startTime.toString("yyyy-MM-dd hh:mm:ss")));
        this->model->setItem(i, 1, new QStandardItem(ret[i].treatData.treatFreq));
        this->model->setItem(i, 2, new QStandardItem(ret[i].treatData.treatWaveType));
        this->model->setItem(i, 3, new QStandardItem(QString::number(ret[i].treatData.treatCurrent)));
        this->model->setItem(i, 4, new QStandardItem(QString::number(ret[i].treatData.treatTime)));
    }

    qDebug() << db->getTreatHistoy().length();
}


void MainWindow::on_pushButton_up_clicked()
{
    if(ui->checkBox_applySkin->isChecked()){
        tP->setCurrent(tP->getCurrent()+50);
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
    autoHaltTimer->start(30000);
}

void MainWindow::on_comboBox_freSel_currentIndexChanged(const QString &arg1)
{
    ui->label_Freq->setText(arg1);
    autoHaltTimer->start(30000);
}

void MainWindow::on_pushButton_record_clicked()
{
    rec.treatData.treatCurrent = tP->getCurrent();
    //rec.treatData.treatTime = tP->getTime();
    rec.treatData.treatFreq = tP->getFreq();
    rec.treatData.treatWaveType = tP->getWaveType();
    qDebug() << rec.startTime;
    db->addTreatHistory(rec);
    ui->pushButton_record->setEnabled(false);
    updateTableView();
}


void MainWindow::on_pushButton_clearHistory_clicked()
{
    db->clearTreatHistory();
    updateTableView();
}
