#pragma once
#include "File.h"
class Output : public File
{
public:
	virtual ~Output();
	//определение функции. открытие файла для чтения
	virtual void OpenFile() override;

	//записать текущий массив в файл
	const void WriteArrayInFile(vector<int>& array);
};

