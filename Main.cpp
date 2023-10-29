#include "Input.h"
#include "Output.h"
#include "InsertionSort.h"

//вывод в консоль выполнявших работу
void Creators() {
	cout << "Laboratory work #2 : Insertion sort\n" <<
		"Made by:\nEfremov Ivan\nRunina Anastasia\nShaimardanova Sofia\n" <<
		"Group #423\n\n";
}

//вывод в консоль пунктов меню
void InputMenuOptions() {
	cout << "[1] - Console Input\n[2] - File Input\n" <<
		"[3] - Random Input\n[4] - Exit\n\n";
}


int main() {

	int menuOption{}; //выбранный пункт меню
	bool isCorrect{}; //корректен ли ввод с файла
	Input fillArray{}; //объект класса ввода
	Output writeArray{}; //объект класса вывода
	InsertionSort sortArray{}; //объект класса сортировки
	vector<int> array; //массив

	do {
		Creators();
		InputMenuOptions();

		//выбор пункта меню
		menuOption = GetMenuOption("Select menu option\n\n>>");

		//заполнение массива в зависимости от пункта меню
		switch (menuOption)
		{
		case InputMenu::consoleInput:
			//заполнение массива с консоли и предложение сохранить массив в файл
			fillArray.ConsoleInput(array);
			if (fillArray.SaveResults("Do you want to save current array in the file? ") == 'y')
				writeArray.WriteArrayInFile(array);
			break;

		case InputMenu::fileInput:
			//заполнение массива из файла
			do {
				isCorrect = fillArray.FileInput(array);
			} while (!isCorrect);
			break;

		case InputMenu::randomInput:
			//заполнение массива случайными значениями и предложение сохранить массив в файл
			fillArray.RandomInput(array);
			if (fillArray.SaveResults("Do you want to save current array in the file? ") == 'y')
				writeArray.WriteArrayInFile(array);
			break;

		case InputMenu::exitApp:

			cout << "Programm finished it's work!\n";
			exit(EXIT_SUCCESS);

		default:

			cout << "Incorrect option!\n";
			break;

		}

		//сортировка массива
		sortArray.Sort(array);

		//вывод сообщения о сортировке массива, отсортированного массива и количества сравнений и перестановок
		cout << "\nArray was successfully sorted!\n";
		fillArray.PrintCurrentArray(array);
		cout << "Total comparisons: " << sortArray.GetComparisons() << '\n'
			<< "Total permutations: " << sortArray.GetPermutations() << "\n\n";

		//предложить пользователю сохранить отсортированный массив
		if (writeArray.SaveResults("Do you want to write sorted array in the file? ") == 'y') {
			writeArray.WriteArrayInFile(array);
			cout << "Sorted array was successfully written in the file!\n\n";
		}

		system("pause");
		system("cls");

		//очистка массива для последующих вводов данных
		array.clear();

	} while (menuOption != InputMenu::exitApp);

	return EXIT_SUCCESS;
}