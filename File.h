#pragma once
#include <fstream>
#include "InputMethods.h"
#include <vector>
#include <io.h>
#include <cerrno>
#include <filesystem>

class File {
protected:
	fstream file;
public:
	File();
	virtual ~File();
	//функция предлагающая сохранить введённые / отсортированные данные в файла
	char SaveResults(string message);
	//чистая виртуальная фунция для открытия файла
	virtual void OpenFile() = 0;
};
