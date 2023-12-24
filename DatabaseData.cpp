#include "DatabaseData.h"

/*
    Конструктор класса DatabaseData
    Инициализация id объекта, исходного массива и отсортированного массива
*/
DatabaseData::DatabaseData(int id, QVector<int> originalArray, QVector<int> sortedArray){
    this->id = id;
    this->originalArray = originalArray;
    this->sortedArray = sortedArray;
}
