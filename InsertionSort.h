#pragma once
#include <vector>
using namespace std;
class InsertionSort
{
private:
	int comparisons;
	int permutations;
public:
	InsertionSort();
	~InsertionSort() = default;
	//сортировка массива методом вставок
	void Sort(vector<int>& array);

	//возвращение значения перестановок
	int GetPermutations();

	//возвращение значения сравнений
	int GetComparisons();

	//сравнение двух чисел
	bool Compare(int first, int second);

	//перестановка двух чисел в массиве
	void Permutate(vector<int>& array, int posFirst, int posSecond);

};

