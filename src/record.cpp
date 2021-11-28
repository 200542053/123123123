#include "record.h"

Record::Record() {

}

//QString Record::toString() {
//    QString newString =
//            startTime.toString("ddd h:mm ap") + "\n"
//            + "   Therapy: " + treatment + "\n"
//            + "   Power Level: " + QString::number(powerLevel) + "\n"
//            + "   Duration: " + QString::number(duration/60) + ((duration%60 < 10) ? + ":0" + QString::number(duration%60) : + ":" + QString::number(duration%60));

//    return newString;
//}

bool Record::addRecord(TreatRecordType t){
    treatRecordList.push_back(t);
    return true;
}

int Record::getRecordLen(){
    return treatRecordList.length();
}

TreatRecordType Record::getRecord(int index){
    return treatRecordList[index];
}
