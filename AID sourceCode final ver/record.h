#ifndef RECORD_H
#define RECORD_H

#include <QString>
#include <QDateTime>
#include <QTime>
#include <QVector>
#include "common.h"

/* Purpose of Class: To store information about a treatment in the database and in mainwindow
 *
 * Data Members:
 * - QString treatment: a QString representation of a treatment
 * - QDateTime startTime: an object to keep track of what time a threatment started at
 * - int powerLevel: integer representation of the power level of the therapy (max power level)
 * - int duration: a integer representation of how long the treatment lasted (in seconds)
 *
 * - Class Functions:
 * - Getters and Setters
 *
 */

class Record {

public:
    Record();
    bool addRecord(TreatRecordType);
    int getRecordLen();
    TreatRecordType getRecord(int);

private:
    QVector<TreatRecordType> treatRecordList;

};

#endif // RECORD_H
