#ifndef DATABASEDATA_H
#define DATABASEDATA_H

#include <QVector>

class DatabaseData
{
public:
    DatabaseData(int, QVector<int>, QVector<int>);
    int id;
    QVector<int> originalArray;
    QVector<int> sortedArray;
};


#endif // DATABASEDATA_H
