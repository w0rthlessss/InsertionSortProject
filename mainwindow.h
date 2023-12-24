#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QProgressBar>
#include "DatabaseData.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void UpdateTables();
    void AddNewArray();
    void DeleteArrayFromDatabase();
    void SaveChangesForDatabase();
    void UpdateArrayElementOnChange(QTableWidgetItem *changedElement);
    void SortArray();
    void StartTesting();
public:
    MainWindow(QWidget *parent = nullptr);
    void resizeEvent(QResizeEvent*);
    void closeEvent(QCloseEvent *event);
    void SetupStatusBar();
    void LoadArraysFromDatabase();
    void SetupTable(QTableWidget *table);
    bool CheckIfPreviousCellIsNotEmpty(QTableWidgetItem *clickedElement);
    void UpdateTableValues(QTableWidget *table, QVector<int> &array);
    void UpdateArrayListUI();
    void UpdateArrayFromTable();
    QVector<int> ConvertDatabaseDataToArray(QString &databaseElement);
    ~MainWindow();

private:
    bool isDatabaseSaved;
    QVector<DatabaseData> allArrays;
    bool isAnyDatabseOpen;
    QProgressBar *progressBar;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
