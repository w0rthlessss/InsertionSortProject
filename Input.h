#pragma once
#include "File.h"
class Input : public File
{
public:
	virtual ~Input();
	//определение функции. открытие файла для чтения
	virtual void OpenFile() override;

	//заполнение массива случайными значениями
	void RandomInput(vector<int>& array);

	//заполнение массива данными из файла. возвращает булево значение корректности ввода
	bool FileInput(vector<int>& array);

	//заполнение массива данными из консоли
	void ConsoleInput(vector <int>& array);

	//вывод текущего массива
	const void PrintCurrentArray(vector<int>& array);

};

