#include "Tests.h"
#include "ui_Tests.h"
#include <QMouseEvent>

/*
    Конструктор класса Tests
    Инициализация интерфейса, контрольных значений и таймера
    Связывание интерфейса с функциями
*/
Tests::Tests(QWidget *parent) ://parent указатель на базовый класс в qt
    QWidget(parent),
    ui(new Ui::Tests)
{
    //Инициализация интерфейса и контрольных значений
    ui->setupUi(this);//setupUi инициализация интерфейса
    testIndex = 0;
    numberOfArrays = 100;

    timer = new QElapsedTimer;

    //Заполнение начальных значений интерфейса шаблон для всех тестов
    ui->testProgressBar->setValue(0);
    ui->testDescription->setText("Test adding " + QString::number(numberOfArrays) + " arrays in database");
    ui->averageTestTime->setText("Average time: ");
    ui->testLabel->setText("Test #" + QString::number(testIndex + 1));
    ui->totalTestTime->setText("Total time: ");

    //Связывание кнопки запуска теста с функцией
    connect(ui->launchTestButton, SIGNAL(clicked(bool)), this, SLOT(LaunchTest()));
}

/*
    Тест добавления n массивов в БД
*///обращаемся к файлу QFile файловый поток
bool Tests::AddArraysTest(){
    database = new QFile("Databases/databaseForTests.txt");

    //Заполнение n массивов случайными значениями
    for(int i = 0; i < numberOfArrays; i++){
        //Значение для отображения в ProgressBar, зависящее от размера массива
        int progressBarPercentage = i * 100 / numberOfArrays;
        ui->testProgressBar->setValue(progressBarPercentage);
        // global- возвращет указатель на рандом генератор-> bounded-границы в рaндоме qt
        QVector<int> temporaryArray(QRandomGenerator::global()->bounded(1, 1000));
        RandomFillingOfArray(&temporaryArray);
        allArrays.push_back(DatabaseData(i + 1, temporaryArray, {}));
    }

    //Запись массивов в БД
    bool isCorrect = WriteDataInDatabase(database, &allArrays);

    allArrays.clear();

    //При успешной записи(записывавем 100% в прогрессбар)
    if(isCorrect){
        ui->testProgressBar->setValue(100);
        return true;
    }
    ui->testProgressBar->setValue(0);
    ui->status->setText("Unable to write data in test database!");
    return false;
}

/*
    Тест выгрузки и сортировки n массивов из БД
*/
bool Tests::SortArraysTest(){
    database = new QFile("Databases/databaseForTests.txt");

    //Открытие базы данных. При неудаче выведется сообщение
    if(!OpenDatabase(database, &allArrays)){
        ui->status->setText("Unable to open database!");
        ui->testProgressBar->setValue(0);
        return false;
    }

    //
    for(int i = 0; i < numberOfArrays; i++){
        //Значение для отображения в ProgressBar, зависящее от размера массива
        int progressBarPercentage = i * 100 / numberOfArrays;
        ui->testProgressBar->setValue(progressBarPercentage);

        //Если массив пуст (ошибка выгрузки)
        if(allArrays[i].originalArray.empty()){
            ui->status->setText("Invalid data in test database!");
            ui->testProgressBar->setValue(0);
            allArrays.clear();
            return false;
        }

        //Сортировка массива
        SortingArray(&allArrays[i], new QProgressBar);//QProgressBar заглушка 
    }

    //Запись отсортированных массивов в БД
    if(!WriteDataInDatabase(database, &allArrays)){// не удалось записать сорт массив
        ui->status->setText("Unable to write data in test database!");
        ui->testProgressBar->setValue(0);
        allArrays.clear();
        return false;
    }

    allArrays.clear();
    ui->testProgressBar->setValue(100);
    return true;
}

/*
    Тест удаления n массивов из БД
*/
bool Tests::DeleteArraysTest(){
    //Открытие БД с очисткой данных
    bool isCorrect = WriteDataInDatabase(database, &allArrays);

    //Если не удается открыть БД для выгрузки
    if(!OpenDatabase(database, &allArrays) || !isCorrect){
        ui->status->setText("Unable to open database!");
        ui->testProgressBar->setValue(0);
        return false;
    }

    //Если файл не пуст
    if(!database->atEnd()){
        ui->status->setText("Unable to open database!");
        ui->testProgressBar->setValue(0);
        return false;
    }

    database->close();
    ui->testProgressBar->setValue(100);
    return true;
}

/*
    Функция для вызова тестов
    Вызывает функцию теста, соответствующую индексу текущего теста
    Подсчет времени выполнения каждого теста
    Вызов идет в формате (Добавление, Сортировка, Удаление) 100, 1000, 10000 массивов
*/
void Tests::LaunchTest(){
    //Изменение интерфейса при запуске теста
    ui->launchTestButton->setDisabled(true);
    ui->averageTestTime->setText("Average time: ");
    ui->testLabel->setText("Test #" + QString::number(testIndex + 1));
    ui->totalTestTime->setText("Total time: ");

    //При кажом 3 тесте увеличиваем количество массивов на 10
    if(testIndex % 3 == 0 && testIndex!=0) numberOfArrays *= 10;

    //Запуск таймера
    //qint64 - то же самое что и long long, но в qt
    qint64 elapsedTime;
    timer->start();

    switch(testIndex % 3){

    //Добавление массивов
    case 0:
        //Изменение описания теста
        ui->testDescription->setText("Test adding " + QString::number(numberOfArrays) + " arrays in database");
        //Установка флага выполнения теста и вызов самого теста
        ui->testList->currentItem()->setCheckState(AddArraysTest() ? Qt::Checked : Qt::PartiallyChecked);
        //Остановка таймера
        elapsedTime = timer->elapsed();
        //Запись времени выполнения: Итоговой и Средней
        ui->totalTestTime->setText("Total time: " + QString::number(elapsedTime) + " ms");
        ui->averageTestTime->setText("Average time: " + QString::number(elapsedTime / numberOfArrays) + " ms");
        break;

    //Сортировка массивов
    case 1:
        //Изменение описания теста
        ui->testDescription->setText("Test sorting " + QString::number(numberOfArrays) + " arrays from database");
        //Установка флага выполнения теста и вызов самого теста
        ui->testList->currentItem()->setCheckState(SortArraysTest() ? Qt::Checked : Qt::PartiallyChecked);
        //Остановка таймера
        elapsedTime = timer->elapsed();
        //Запись времени выполнения: Итоговой и Средней
        ui->totalTestTime->setText("Total time: " + QString::number(elapsedTime) + " ms");
        ui->averageTestTime->setText("Average time: " + QString::number(elapsedTime / numberOfArrays) + " ms");
        break;

    //Удаление массивов
    case 2:
        //Изменение описания теста
        ui->testDescription->setText("Test deleting " + QString::number(numberOfArrays) + " arrays from database");
        //Установка флага выполнения теста и вызов самого теста
        ui->testList->currentItem()->setCheckState(DeleteArraysTest() ? Qt::Checked : Qt::PartiallyChecked);
        //Остановка таймера
        elapsedTime = timer->elapsed();
        //Запись времени выполнения: Итоговой и Средней
        ui->totalTestTime->setText("Total time: " + QString::number(elapsedTime) + " ms");
        ui->averageTestTime->setText("Average time: " + QString::number(elapsedTime / numberOfArrays) + " ms");
        break;
    }

    //Увеличиваем номер теста, пока не дойдем до последнего
    if(testIndex<8){
        testIndex++;
        ui->testList->setCurrentRow(testIndex);
        ui->launchTestButton->setDisabled(false);
        return;
    }
}

/*
    Фунция игнорирующая события нажатия мыши, при выполнении теста
*/
void Tests::mousePressEvent(QMouseEvent *event){
    if(!ui->launchTestButton->isEnabled()) event->ignore();
}

/*
    Деструктор класса
    Удаляет указатель на интерфейс, на таймер и на файл БД
*/
Tests::~Tests()
{
    delete ui;
    delete timer;
    delete database;
}
