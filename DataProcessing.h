#ifndef DATAPROCESSING_H
#define DATAPROCESSING_H

#include <QFile>
#include <QRandomGenerator>
#include "DatabaseData.h"
#include <QProgressBar>
#include <QMessageBox>

QVector<int> ConvertDatabaseDataToArray(QString &databaseElement);

bool WriteDataInDatabase(QFile *database, QVector<DatabaseData> *allArrays);

void RandomFillingOfArray(QVector<int> *array);

bool OpenDatabase(QFile *database, QVector<DatabaseData> *allArrays);

QString SortingArray(DatabaseData *array, QProgressBar *progressBar);


#endif // DATAPROCESSING_H
