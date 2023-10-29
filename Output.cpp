#include "Output.h"

//деструктор
Output::~Output()
{
	file.close();
}

//открытие файла для записи
void Output::OpenFile()
{
	string name{};

	do {
		name = GetLink("\nEnter the name of file where array will be stored.\nExample: array.txt\n\n");
		//проверить доступ к файлу по адресу name.
		//вернет 0, если к файлу можно обратиться
		//иначе смотрим по коду ошибки, по какой причине к файлу нельзя обратиться
		if (_access(name.c_str(), 0) == 0) {

			if (filesystem::exists(name)) {
				if (SaveResults("\nFile exists. Do you want to overwrite current data in the file") == 'n') continue;
			}

			file.open(name.c_str(), ios::out);
		}
		else {
			//если файл не существует
			if (errno == ENOENT) {
				file.open(name.c_str(), ios::out);
			}
			//если в пути к файлу содержатся зарезервированные системой имена
			else {
				cout << "\nAdress contains forbidden value!\n";
				continue;
			}
		}

		return;
	} while (true);
}

const void Output::WriteArrayInFile(vector<int>& array)
{
	OpenFile();

	for (auto element : array) file << element << ' ';

	file.close();
}
