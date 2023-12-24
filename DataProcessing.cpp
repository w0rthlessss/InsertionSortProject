#include "DataProcessing.h"
#include "InsertionSort.h"


/*
    Функция для преобразованиия, данных из БД в массив
    (в БД массивы хранятся как строки)
    Возвращает массив элементов, преобразованных в целые числа
*/
QVector<int> ConvertDatabaseDataToArray(QString &databaseElement){
    //Если строка пустая, то возвращаем пустой массив
    if(databaseElement.length()==0) return QVector<int>();

    //Разделение строки по символу ;
    QStringList arrayElements = databaseElement.split(';');
    QVector<int> intArray;
    bool isCorrect = true;

    for(const QString &element : arrayElements){
        int convertedElement = element.toInt(&isCorrect);
        //Если элемент строки невозможно преобразовать в число, то выводим сообщение об ошибке
        //и возвращаем пустой массив
        if(!isCorrect){
            QMessageBox msg(QMessageBox::Critical, "Error", "Invalid elements in database!");
            msg.exec();
            return QVector<int>();
        }

        intArray.push_back(convertedElement);
    }

    return intArray;
}

/*
    Функция, открывающая базу данных для чтения
    Заполняет временное хранилище данными из БД
    Возвращает флаг успешного / неудачного чтения информации
*/
bool OpenDatabase(QFile *database, QVector<DatabaseData> *allArrays){
    //Открытие БД для чтения c преобразованием символов переноса строки
    if(database->open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream input(database);
        while(!input.atEnd()){
            //Считывание элемента бд
            int id = input.readLine().toInt();
            QString originalArray = input.readLine();
            QString sortedArray = input.readLine();

            input.readLine();

            allArrays->push_back(DatabaseData(id, ConvertDatabaseDataToArray(originalArray), ConvertDatabaseDataToArray(sortedArray)));
        }
        database->close();
        return true;
    }
    return false;
}

/*
    Функция, открывающая базу данных для записи
    Очищает БД и заполняет ее элементами внутреннего хранилища
    Возвращает флаг успешной / неудачной попытки записи информации
*/
bool WriteDataInDatabase(QFile *database, QVector<DatabaseData> *allArrays){
    //Открытие БД для записи с очищением файла и c преобразованием символов переноса строки
    if(!database->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
        return false;
    }
    QTextStream output(database);
    for(const DatabaseData &array : *allArrays){
        output<<array.id<<'\n';

        //Обход массива до препоследнего элемента, чтобы в конце не стоял разделитель
        for(int index = 0; index < array.originalArray.size()-1; index++){
            output<<array.originalArray[index]<<';';
        }
        output<<array.originalArray.back()<<'\n';

        //Если массив не был отсортирован, то не выводим ничего
        if(array.sortedArray.size() != 0){

            //Обход массива до препоследнего элемента, чтобы в конце не стоял разделитель
            for(int index = 0; index < array.sortedArray.size()-1; index++){
                output<<array.sortedArray[index]<<';';
            }
            output<<array.sortedArray.back();
        }
        output<<"\n\n";
    }
    database->close();

    return true;
}

/*
    Функция для заполнения массива случайными числами
*/
void RandomFillingOfArray(QVector<int> *array){
    for(int index = 0; index < array->size(); index++){
        (*array)[index] = QRandomGenerator::global()->bounded(-10000, 10000);
    }
}

/*
    Функция, вызывающая метод сортировки из объекта InsertionSort
    Возвращает сообщение о количестве сравнений и перестановок
*/
QString SortingArray(DatabaseData *array, QProgressBar *progressBar){
    InsertionSort sortingMethod;
    array->sortedArray = array->originalArray;

    sortingMethod.Sort(array->sortedArray, progressBar);

    return "Array was successfully sorted!\n\nComparisons: "
                            + QString::number(sortingMethod.GetComparisons())
                            + "\n\nPermutations: " + QString::number(sortingMethod.GetPermutations());
}
