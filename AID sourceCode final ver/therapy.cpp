#include "therapy.h"

Therapy::Therapy() {
    // initialize the timer
    timer = new QTimer(this);
}


Therapy::~Therapy() {
    delete timer;
}

void Therapy::therapyInit(TreatDataType& t){
    treatData = t;
}

// getters
int Therapy::getCurrent() { return treatData.treatCurrent;}
int Therapy::getTime() { return treatData.treatTime;}
QString Therapy::getWaveType() { return treatData.treatWaveType; }
QString Therapy::getFreq() { return treatData.treatFreq; }
QTimer* Therapy:: getTimer() { return timer; }

// setters
void Therapy::setTime(int t){ treatData.treatTime = t; }
void Therapy::setWaveType(QString w) { treatData.treatWaveType =  w;}
void Therapy::setFreq(QString f) {treatData.treatFreq = f;}
void Therapy::setCurrent(int c) {treatData.treatCurrent = c;}
