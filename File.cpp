#include "File.h"


//конструктор по умолчанию
File::File()
{
	this->file = fstream{};
}

//деструктор
File::~File()
{
	file.close();
}

//предложить пользователю сохранить данные массива в файл
char File::SaveResults(string message)
{
	cout << "\n" << message << " (y / n)\n\n";
	char res = 'n';
	//пока не будет корректного ответа
	do {
		res = GetAnswer(">>");
		if (res != 'y' && res != 'n') {
			cout << "Incorrect input. Type 'y' or 'n' only!\n\n";
		}
	} while (res != 'y' && res != 'n');

	return res;
}
