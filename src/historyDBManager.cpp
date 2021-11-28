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
    query.exec("CREATE TABLE IF NOT EXISTS profiles ( pid INTEGER NOT NULL UNIQUE PRIMARY KEY AUTOINCREMENT, battery_level REAL NOT NULL, power_level INTEGER NOT NULL);");
    query.exec("CREATE TABLE IF NOT EXISTS frequencies ( name TEXT NOT NULL UNIQUE PRIMARY KEY);");
    query.exec("CREATE TABLE IF NOT EXISTS therapies  ( name TEXT NOT NULL UNIQUE PRIMARY KEY,  frequency TEXT NOT NULL REFERENCES frequencies, duration INTEGER NOT NULL);");
    query.exec("CREATE TABLE IF NOT EXISTS records ( rid INTEGER NOT NULL UNIQUE PRIMARY KEY AUTOINCREMENT,date TEXT NOT NULL,power_level INTEGER NOT NULL,duration INTEGER NOT NULL);");
    query.exec("CREATE TABLE IF NOT EXISTS therapy_records(name TEXT NOT NULL REFERENCES therapies,tid INTEGER NOT NULL REFERENCES records(rid) ON DELETE CASCADE, PRIMARY KEY (name, tid));");
    query.exec("CREATE TABLE IF NOT EXISTS frequency_records( name TEXT NOT NULL REFERENCES frequencies,fid INTEGER NOT NULL REFERENCES records(rid) ON DELETE CASCADE,PRIMARY KEY (name, fid));");

    // initialize device
    query.exec("INSERT OR REPLACE INTO frequencies VALUES ('10Hz'),('20Hz'),('60Hz'),('200Hz'),('7710');");
    query.exec("INSERT OR REPLACE INTO therapies VALUES('PAIN', '10Hz', 900),('GYNECOLOGIC PAIN', '7710', 900),('POTENCY', '60Hz', 900),('HEAD', '60Hz', 300);");

    return denasDB.commit();
}


QVector<TreatRecordType> HistoryDBManager::getTreatHistoy(int start, int maxLen) {

    QSqlQuery query;
    QVector<TreatRecordType> qvr;
//    denasDB.transaction();

//    query.prepare("SELECT name as treatment,date,power_level,duration FROM ( SELECT name as name,tid as rid FROM therapy_records UNION SELECT name as name,fid as rid FROM frequency_records ) NATURAL JOIN records ORDER BY rid;");
//    query.exec();

//    while (query.next()) {
//        QString name = query.value(0).toString();
//        QDateTime start = QDateTime::fromString(query.value(1).toString(), DATE_FORMAT);
//        int power = query.value(2).toString().toInt();
//        int duration = query.value(3).toString().toInt();
//        Record* r = new Record(name, start, power, duration);
//        qvr.push_back(r);
//    }
    return qvr;
}


/*
 * Type: Private
 * Validates the given parameters.
 *
 * Parameters:
 *  recordType - The type of record, used only for debugging
 *  time - the time the treatment started in QDateTime format
 *  powerLevel - the maximum power level observed during a treatment
 *  duration - the amount of time in milliseconds that the treatment was in use
 *
 * Returns:
 *  True - If the parameters are acceptable
 *  False - Otherwise
 *
 * See Also:
 *  DBManager::addTherapyRecord
 *  DBManager::addFrequencyRecord
 */
bool isValidRecord(const QString& recordType, const QDateTime& time, int powerLevel, int duration) {

    bool valid = true;
    if (!time.isValid()) {
        qDebug() << "Error: Cannot add " << recordType << " record, time is not valid";
        valid = false;
    }
    else if (powerLevel < 0 || powerLevel > 100) {
        qDebug() << "Error: Cannot add " << recordType << " record, power level is not valid";
        valid = false;
    }
    else if (duration < 0) {
        qDebug() << "Error: Cannot add " << recordType << " record, duration is not valid";
        valid = false;
    }
    return valid;
}


/*
 * Type: Private
 * Adds a treatment record to the database.
 *
 * Parameters:
 *  tableName - The database table to insert this record into
 *  name - The name of the frequency or therapy
 *  time - the time the treatment started in QDateTime format
 *  powerLevel - the maximum power level observed during a treatment
 *  duration - the amount of time in milliseconds that the treatment was in use
 *
 * Returns:
 *  True - If the record was added successfully
 *  False - Otherwise
 *
 * See Also:
 *  DBManager::addTherapyRecord
 *  DBManager::addFrequencyRecord
 */
bool HistoryDBManager::addTreatHistory(TreatRecordType) {

    denasDB.transaction();

//    QSqlQuery query;
//    query.prepare("INSERT INTO records (date, power_level, duration) VALUES (:date, :power_level, :duration);");
//    query.bindValue(":date", time.toString(DATE_FORMAT));
//    query.bindValue(":power_level", powerLevel);
//    query.bindValue(":duration", duration);
//    query.exec();

//    int rowid = query.lastInsertId().toInt();
//    query.prepare("INSERT INTO " + tableName + "_records VALUES (:name, :id);");
//    query.bindValue(":name", name);
//    query.bindValue(":id", rowid);
//    query.exec();

    return denasDB.commit();
}

bool HistoryDBManager::clearTreatHistory(){

}
