#ifndef HISTORYDBMANAGER_H
#define HISTORYDBMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QDateTime>
#include <QDebug>
#include <QSqlQuery>
#include <QList>
#include <QApplication>

#include "common.h"
#include "record.h"
#include "therapy.h"


/* Class Purpose: Manages interactions between application and database
 *
 * Data Members:
 * - QSqlDatabase denasDB: the database object
 * + QString DATE_FORMAT: formats of dates going in/out the database
 * + QString DATABASE_PATH: path of the database
 *
 * Class Functions:
 * Getters for frequencies, profile, recordings, therapies
 *
 * bool addTreatHistory(TreatRecordType): adds a therapy record to the database, returns true if it was added properly, false otherwise
 * bool clearTreatHistory(): deletes all the records
 * QVector<TreatRecordType>  getTreatHistoy(): Return treatment history in all databases
 *
 * bool DBInit(): Initalizes the database
 */

class HistoryDBManager {

public:
    const QString DATE_FORMAT = "yyyy-MM-dd hh:mm:ss";
    static const QString DATABASE_PATH;

    HistoryDBManager();
    bool addTreatHistory(TreatRecordType);
    bool clearTreatHistory();
    QVector<TreatRecordType>  getTreatHistoy();


private:
    QSqlDatabase denasDB;
    bool DBInit();

};

#endif // HISTORYDBMANAGER_H
