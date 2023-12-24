#include "mainwindow.h"
#include "Tests.h"
#include "ui_mainwindow.h"
#include <QResizeEvent>
#include "TableCellDelegate.h"
#include <QFileDialog>
#include <QStandardPaths>
#include "AddArray.h"
#include <QRandomGenerator>
#include <QCloseEvent>
#include "DataProcessing.h"

/*
    Конструктор класса MainWindow
    Загрузка интерфейса, данных из БД и свзяывание функций с элементами интерфейса
*/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SetupStatusBar();
    progressBar->setValue(0);
    LoadArraysFromDatabase();
    UpdateTables();
    isDatabaseSaved = true;

    //Подключение функций к сигналам, посылаемым элементами интерфейса
    connect(ui->arrayList, SIGNAL(itemSelectionChanged()),this, SLOT(UpdateTables()));
    connect(ui->actionSaveChanges, SIGNAL(triggered(bool)), this, SLOT(SaveChangesForDatabase()));
    connect(ui->addArrayBtn, SIGNAL(clicked(bool)), this, SLOT(AddNewArray()));
    connect(ui->deleteArrayBtn, SIGNAL(clicked(bool)), this, SLOT(DeleteArrayFromDatabase()));
    connect(ui->originalArrayTable, &QTableWidget::itemChanged, this, &MainWindow::UpdateArrayElementOnChange);
    connect(ui->sortArrayBtn, SIGNAL(clicked(bool)), this, SLOT(SortArray()));
    connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(ui->actionLaunch_tests, SIGNAL(triggered(bool)), this, SLOT(StartTesting()));
}

/*
    Функция, устанавливающая форматирование для таблицы.
    Установка делегата, отвечающего за ввод и изменение элементов массива,
    а также форматирование отображения строк и столбцов
*/
void MainWindow::SetupTable(QTableWidget *table){
    //Установка делегата и прототипа элемента таблицы
    table->setItemDelegate(new TableCellDelagate);
    QTableWidgetItem *prototype = new QTableWidgetItem;
    table->setItemPrototype(prototype);

    //Установка прототипа элемента и значения для номеров колонок
    for(int index = 0; index < table->columnCount(); index++){
        prototype = new QTableWidgetItem;
        prototype->setTextAlignment(Qt::AlignCenter);
        prototype->setText(QString::number(index));
        prototype->setFlags(Qt::NoItemFlags);
        table->setHorizontalHeaderItem(index, prototype);
    }

    //Установка прототипа элемента и значения для номеров строк
    for(int index = 0; index < table->rowCount(); index++){
        prototype = new QTableWidgetItem;
        prototype->setTextAlignment(Qt::AlignCenter);
        prototype->setText(QString::number(index));
        prototype->setFlags(Qt::NoItemFlags);
        table->setVerticalHeaderItem(index, prototype);
    }
}

/*
    Функция, обрабатывающая событие закрытия приложения.
    Предлагает сохранить изменения в БД, если они не были сохранены
*/
void MainWindow::closeEvent(QCloseEvent *event){
    //Если изменения не сохранены, выводим диалоговое окно с предложением сохранить данные
    if(!isDatabaseSaved){
        int reply = QMessageBox::question(this, MainWindow::windowTitle(),
                              tr("Changes in database were not saved!\nDo you want to save them?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if(reply == QMessageBox::Yes) SaveChangesForDatabase();
        if(reply == QMessageBox::Cancel) {event->ignore(); return;}
    }
    close();
}

/*
    Функция, устанавливающая форматирование для StatusBar
    В StatusBar находится полоска прогресса, которой тоже задаются геометрические параметры
*/
void MainWindow::SetupStatusBar(){
    progressBar = new QProgressBar;
    ui->statusbar->setFixedHeight(25);
    progressBar->setFixedWidth(150);
    ui->statusbar->addPermanentWidget(progressBar);
}

/*
    Функция, обрабатывающая событие изменение размера окна
    Делает так, чтобы таблица всегда полностью помещалась в окно по горизонтали
*/
void MainWindow::resizeEvent(QResizeEvent *event){
    QMainWindow::resizeEvent(event);
    ui->originalArrayTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->sortedArrayTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

/*
    Функция, загружающая массивы из БД
    и обновляющая интерфейс в соответствие с данными из БД
*/
void MainWindow::LoadArraysFromDatabase(){

    QFile database("Databases/database.txt");

    OpenDatabase(&database, &allArrays);

    UpdateArrayListUI();
}

/*
    Функция, изменяющая отображаемые данные массива в таблице,
    в соответствие с выбранным массивом из списка
*/
void MainWindow::UpdateTableValues(QTableWidget *table, QVector<int> &array){
    //Установка количества строк таблицы как 1 + размер массива / 10, с округлением вверх
    table->setRowCount(1 + (array.size()+9)/10);
    table->clear();
    for(int row = 0; row < table->rowCount(); row++){
        for(int col = 0; col < 10; col++){
            if(row * 10 + col == array.size()) {
                SetupTable(table);
                return;
            }
            //создание новой ячейки таблицы с текущим элементом массива
            table->setItem(row, col, new QTableWidgetItem(QString::number(array[row*10 + col])));
        }
    }
}

/*
    Функция, изменяющая отображаемые данные из БД,
    в соответствие с выбранным элементом из списка (изменяет всю вкладку,
    при выборе цельного элемента БД)
*/
void MainWindow::UpdateTables(){
    ui->arrayTabs->setCurrentIndex(0);

    if(allArrays.empty()) return;

    int arrayIndex = ui->arrayList->currentRow();
    QVector<int> originalArray = allArrays[arrayIndex].originalArray, sortedArray = allArrays[arrayIndex].sortedArray;

    UpdateTableValues(ui->originalArrayTable, originalArray);

    if(sortedArray.size() != originalArray.size() && sortedArray.size() != 0){
        QMessageBox msg(QMessageBox::Warning, MainWindow::windowTitle(), tr("Invalid data in the current sorted array! Sorted array will not be written!"));
        msg.exec();
        return;
    }

    UpdateTableValues(ui->sortedArrayTable, sortedArray);

    ui->arrayTabs->setTabVisible(1, sortedArray.size()!=0);
}

/*
    Функция изменяющая список массивов в интерфейсе,
    в соответствие с загруженными данными из БД,
    автоматически выбирает последний добавленный элемент
*/
void MainWindow::UpdateArrayListUI(){
    ui->arrayList->clear();
    for(DatabaseData &array : allArrays){
        ui->arrayList->addItem("Array #" + QString::number(array.id));
    }

    if(!allArrays.empty()) ui->arrayList->setCurrentRow(allArrays.size()-1);
}

/*
    Функция, добавляющая новый массив во временное хранилище программы,
    позже массив можно сохранить в БД, как новый элемент
    При вызове вызывается форма класса AddArray
*/
void MainWindow::AddNewArray(){
    AddArray dialog(this);

    if(dialog.exec() != QDialog::Accepted) return;

    int numberOfElements = dialog.GetNumberOfElements();
    int inputOption = dialog.GetInputOption();
    QVector<int> temporaryArray;
    temporaryArray.resize(numberOfElements);

    if(inputOption==1) RandomFillingOfArray(&temporaryArray);

    allArrays.push_back(DatabaseData(allArrays.empty() ? 1 : allArrays.back().id + 1, temporaryArray, QVector<int>()));
    UpdateArrayListUI();

    isDatabaseSaved = false;
}

/*
    Функция, сохраняющая изменения, примененные во временном хранилище,
    будь то изменение элемента массива, добавление нового массива или удаление элемента
*/
void MainWindow::SaveChangesForDatabase(){
    QFile database("Databases/database.txt");

    if(!WriteDataInDatabase(&database, &allArrays)){
        QMessageBox msg(QMessageBox::Critical, "Error!", "Unable to open database!");
        msg.exec();
        return;
    }

    UpdateArrayListUI();

    QMessageBox msg(QMessageBox::Information, MainWindow::windowTitle(), "Database was successfully updated!");
    msg.exec();

    isDatabaseSaved = true;
}

/*
    Функция удаляющая массив из временного хранилища / БД,
    с последующим сохранением изменений
*/
void MainWindow::DeleteArrayFromDatabase(){

    if(!isDatabaseSaved){
        QMessageBox msg(QMessageBox::Warning, MainWindow::windowTitle(), tr("Save changes to database first!"));
        msg.exec();
        return;
    }

    int arrayIndex = ui->arrayList->currentRow();
    QMessageBox msg(QMessageBox::Question, MainWindow::windowTitle(), tr("Are you sure you want to delete array?"), QMessageBox::Yes | QMessageBox::No);
    int userAnswer = msg.exec();

    if(userAnswer == QMessageBox::No) return;

    ui->arrayList->clear();
    allArrays.remove(arrayIndex);

    SaveChangesForDatabase();
}

/*
    Проверка, при добавлении нового элемента в массив, что он не будет
    добавлен в случайное место, а строго после последнего
*/
bool MainWindow::CheckIfPreviousCellIsNotEmpty(QTableWidgetItem *clickedElement){
    int row = clickedElement->row();
    int col = clickedElement->column();
    int arrayIndex = ui->arrayList->currentRow();

    if((row * 10 + col) - allArrays[arrayIndex].originalArray.size() > 0) return false;

    return true;
}

/*
    Функция для изменения данных массива из временного хранилища, при
    добавлении нового элемента или обновлении уже существующего
*/
void MainWindow::UpdateArrayElementOnChange(QTableWidgetItem *changedElement){
    int row = changedElement->row();
    int col = changedElement->column();
    int arrayIndex = ui->arrayList->currentRow();

    bool isValidInput = true;
    int newValue = changedElement->text().toInt(&isValidInput);

    if(!CheckIfPreviousCellIsNotEmpty(changedElement)){
        changedElement->setText("");
        return;
    }

    if(allArrays[arrayIndex].originalArray.size() == (row * 10 + col)){
        if(changedElement->text()=="") return;
        else {
            allArrays[arrayIndex].originalArray.resize(row * 10 + col + 1);
        }
    }

    if(newValue == allArrays[arrayIndex].originalArray[row * 10 + col] || !isValidInput) return;

    allArrays[arrayIndex].originalArray[row * 10 + col] = newValue;
    allArrays[arrayIndex].sortedArray.clear();
    ui->arrayTabs->setTabVisible(1, false);

    if(row + 1 == ui->originalArrayTable->rowCount() && col + 1 == ui->originalArrayTable->columnCount()){
        ui->originalArrayTable->setRowCount(row + 2);
        SetupTable(ui->originalArrayTable);
    }

    isDatabaseSaved = false;
}

/*
    Функция, сортирующая массив, и обновляющая интерфейс в соответствие с отсортированным массивом
    (Открытие вкладки для отсортированных массивов, и вывод сообщение о количестве сравнений и перестановок)
*/
void MainWindow::SortArray(){
    int arrayIndex = ui->arrayList->currentRow();
    QString resultMessage = SortingArray(&allArrays[arrayIndex], progressBar);
    SaveChangesForDatabase();
    UpdateTables();
    ui->arrayList->setCurrentRow(arrayIndex);
    ui->arrayTabs->setCurrentIndex(1);

    QMessageBox msg(QMessageBox::Information, MainWindow::windowTitle(), resultMessage);
    msg.exec();

}

/*
    Функция, открывающая окно с тестированием функционала программы
    При вызове, выводится форма класса Tests
*/
void MainWindow::StartTesting(){
    Tests *test = new Tests;
    MainWindow::setDisabled(true);
    test->show();
    MainWindow::setDisabled(false);
}

/*
    В деструкторе удаление указателя на объекты интерфейса и на полоску прогресса
*/
MainWindow::~MainWindow()
{
    delete ui;
    delete progressBar;
}

