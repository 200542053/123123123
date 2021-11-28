#ifndef RECORD_H
#define RECORD_H

#include <QString>
#include <QDateTime>
#include <QTime>
#include <QVector>
#include "common.h"


class Record {

public:
    Record();
//    bool recordInit(TreatRecordType);
    bool addRecord(TreatRecordType);
//    bool deleteRecord(int);
//    bool deleteRecord(QDateTime);
    int getRecordLen();
    TreatRecordType getRecord(int);

private:
    QVector<TreatRecordType> treatRecordList;

};

#endif // RECORD_H
