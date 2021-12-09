#include "historyDBManager.h"



const QString HistoryDBManager::DATABASE_PATH = "/database/denas.db";


/*
 * Create a new DBManager to a existing denas database, defined by DBManager::DATABASE_PATH
 *
 * Throws:
 *  If the database could not be opened
 */
HistoryDBManager::HistoryDBManager() {

    denasDB = QSqlDatabase::addDatabase("QSQLITE");
    denasDB.setDatabaseName("denas.db");

    if (!denasDB.open()) {
        throw "Error: Database could not be opened";
    }

    if (!DBInit()) {
        throw "Error: Database could not be initialized";
    }
}


bool HistoryDBManager::DBInit() {

    denasDB.transaction();

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS therapies  ( date TEXT NOT NULL UNIQUE PRIMARY KEY,  freq TEXT NOT NULL, waveType TEXT NOT NULL, current INTEGER NOT NULL, time INTEGER NOT NULL);");
    return denasDB.commit();
}


QVector<TreatRecordType> HistoryDBManager::getTreatHistoy() {

    QSqlQuery query;
    QVector<TreatRecordType> qvr;
    denasDB.transaction();

    query.prepare("SELECT * FROM therapies");
    query.exec();

    while (query.next()) {
        TreatRecordType r;
        r.startTime = QDateTime::fromString(query.value(0).toString(), DATE_FORMAT);;
        r.treatData.treatFreq = query.value(1).toString();;
        r.treatData.treatWaveType = query.value(2).toString();
        r.treatData.treatCurrent = query.value(3).toString().toInt();
        r.treatData.treatTime = query.value(4).toString().toInt();
        qvr.push_back(r);
    }
    return qvr;
}


bool HistoryDBManager::addTreatHistory(TreatRecordType t) {

    denasDB.transaction();

    QSqlQuery query;
    query.prepare("INSERT INTO therapies (date, freq, waveType, current, time) VALUES (:date, :freq, :waveType, :current, :time);");
    query.bindValue(":date", t.startTime.toString(DATE_FORMAT));
    query.bindValue(":freq", t.treatData.treatFreq);
    query.bindValue(":waveType", t.treatData.treatWaveType);
    query.bindValue(":current", t.treatData.treatCurrent);
    query.bindValue(":time", t.treatData.treatTime);

    query.exec();


    return denasDB.commit();
}

bool HistoryDBManager::clearTreatHistory(){
    denasDB.transaction();
    QSqlQuery query;
    query.prepare("DELETE FROM therapies;");
    query.exec();
    return denasDB.commit();
}
