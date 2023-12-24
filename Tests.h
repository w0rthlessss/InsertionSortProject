#ifndef TESTS_H
#define TESTS_H

#include <QWidget>
#include "DataProcessing.h"
#include <QElapsedTimer>

namespace Ui {
class Tests;
}

class Tests : public QWidget
{
    Q_OBJECT

public slots:
    void LaunchTest();
public:
    explicit Tests(QWidget *parent = nullptr);  
    ~Tests();
private:
    void mousePressEvent(QMouseEvent *event);
    bool AddArraysTest();
    bool SortArraysTest();
    bool DeleteArraysTest();
    Ui::Tests *ui;
    int testIndex;
    int numberOfArrays;
    QVector<DatabaseData> allArrays;
    QFile *database;
    QElapsedTimer *timer;
};

#endif // TESTS_H
