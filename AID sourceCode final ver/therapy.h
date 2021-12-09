#ifndef THERAPY_H
#define THERAPY_H

#include "common.h"
#include <QString>
#include <QTimer>
#include <QDateTime>
#include <QObject>


/* Purpose of class: Therapy object to hold information about a therapy
 *
 * Data Members:
 * -TreatDataType treatData: A structure for storing treatment information
 * -QTimer* timer: A QTimer object to keep track of how long a therapy should last
 *
 * Class functions:
 * void therapyInit(TreatDataType&): Initialize the class based on the incoming treatment parameters
 *
 * QString getFreq(): Return treatment frequency
 * int getTime(): Return treatment time
 * int getCurrent(): Return treatment current
 * QString getWaveType(): Return treatment output waveform
 * QTimer* getTimer(): Return treatment timer pointer
 *
 * void setTime(int t): Set treatment time
 * void setWaveType(QString w): Set treatment output waveform
 * void setFreq(QString f): Set treatment frequency
 * void setCurrent(int c): Set treatment current
 */


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
