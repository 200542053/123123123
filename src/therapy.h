#ifndef THERAPY_H
#define THERAPY_H

#include "common.h"
#include <QString>
#include <QTimer>
#include <QDateTime>

#include <QObject>


class Therapy: public QObject {

    Q_OBJECT

    public:
        Therapy();
        ~Therapy();
        void therapyInit(TreatDataType&);
        QString getFreq();
        int getTime();
        int getCurrent();
        QString getWaveType();
        QTimer* getTimer();

        void setTime(int t);
        void setWaveType(QString w);
        void setFreq(QString f);
        void setCurrent(int c);

    private:
        TreatDataType treatData;
        QTimer* timer;

};

#endif // THERAPY_H
