#include "Input.h"


//минимальное и максимальное значения для генерации случайных чисел
const int lowerRandomRange = -999;
const int upperRandomRange = 999;


//деструктор
Input::~Input()
{
	file.close();
}

//открытие файла для чтения
void Input::OpenFile()
{
	string name{};
	do {
		name = GetLink("\nEnter the name of file with data. Example: ArrayValues.txt\n");

		//проверить доступ к файлу по адресу name.
		//вернет 0, если к файлу можно обратиться
		//иначе смотрим по коду ошибки, по какой причине к файлу нельзя обратиться
		if (_access(name.c_str(), 0) == 0) {
			file.open(name.c_str(), ios::in);
		}
		else {
			//если файл не существует
			if (errno == ENOENT) {
				cout << "\nError opening file. Make sure, that file exists!\n";
				continue;
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



void Input::RandomInput(vector<int>& array)
{
	srand(static_cast<unsigned int>(time(NULL)));
	//ввод размера массива
	
	int arraySize = GetMenuOption("\nEnter number of elements in array\n\n>>");
	array.resize(arraySize);
	
	//генерация случайных чисел и заполнение ими массива
	for (auto index = 0; index < arraySize; index++)
		array[index] = lowerRandomRange + rand() %
		(upperRandomRange + 1 - lowerRandomRange);

	//вывод текущего массива
	PrintCurrentArray(array);
}

//если ввод успешен - возвращаем true, если нет - false
bool Input::FileInput(vector<int>& array)
{
	//переменная, куда будет записываться считанное значение
	int tmpValue{};

	//открытие файла для чтения
	OpenFile();

	//считывание значений до достижения конца файла
	do {
		file >> tmpValue;

		//если встретили конец файла завершаем цикл
		if (file.eof()) break;

		//если считывание не удалось
		if (file.fail()) {
			cout << "\nError! Invalid data in the file! Check array values!\n";
			array.clear();
			file.close();
			return false;
		}

		//добавление элемета в конец массива
		array.push_back(tmpValue);

	} while (!file.eof());

	file.close();

	return true;
}

void Input::ConsoleInput(vector<int>& array)
{
	//ввод размера массива
	int arraySize = GetMenuOption("\nEnter number of elements in array\n\n>>");
	array.resize(arraySize);

	//заполнение массива значениями из консоли
	cout << "\nEnter " << arraySize << " elements:\n";
	for (auto index = 0; index < arraySize; index++) array[index] = GetInt(">>");

	//вывод текущего массива
	PrintCurrentArray(array);
}

const void Input::PrintCurrentArray(vector<int>& array)
{
	cout << "\nCurrent array is:\n\n";
	for (auto element : array) cout << element << ' ';
	cout << "\n\n";
}
