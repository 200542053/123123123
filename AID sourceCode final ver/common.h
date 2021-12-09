#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QDateTime>

/* struct Purpose: Create a data structure for saving treatment information
 * Data Members:
 * + QString treatFreq: Save treatment frequency
 * + int treatCurrent: Save treatment current
 * + QString treatWaveType: Save treatment output waveform
 * + int treatTime: Save treatment time
 *
 */


struct TreatDataType
{
    QString treatFreq;
    int treatCurrent;
    QString treatWaveType;
    int treatTime;
};


/* struct Purpose: Create a data structure for saving treatment records
 * Data Members:
 * + TreatDataType treatData: Save treatment info
 * + QDateTime startTime: Save treatment start time
 *
 */

struct TreatRecordType
{
    TreatDataType treatData;
    QDateTime startTime;
};



#endif // COMMON_H
