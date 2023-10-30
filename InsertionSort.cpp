#include "InsertionSort.h"

InsertionSort::InsertionSort()
{
	comparisons = 0;
	permutations = 0;
}

void InsertionSort::Sort(std::vector<int>& array)
{
	int perms{};

	//делим массив на две части (отсортированная и нет)
	for (int index = 1; index < array.size(); index++) {
		int elem = array[index];
		//tmpIndex - индекс содержащийся в отсортированной части
		int tmpIndex = index - 1;

		//сдвигаем отсортированную часть, чтобы вставить elem на его место
		while (tmpIndex >= 0 && Compare(elem, array[tmpIndex])) {
			array[tmpIndex + 1] = array[tmpIndex];
			tmpIndex--;
			perms++;
		}
		array[tmpIndex + 1] = elem;
	}
	this->permutations = perms;

	//int comps{}, perms{};
	//for (int i = 1; i < static_cast<int>(array.size()); i++) {
	//	int j = i - 1;
	//	int element = array[i];
	//	int position = [&comps](std::vector<int> array, int element, int low, int high) {

	//		while (low <= high) {
	//			int mid = low + (high - low) / 2;
	//			comps++;
	//			if (element == array[mid]) return mid;
	//			else if (element > array[mid]) low = mid + 1;
	//			else high = mid - 1;
	//		}
	//		return low;
	//	}(array, element, 0, j);

	//	/*while (j >= position) {
	//		array[j + 1] = array[j];
	//		j--;
	//		perms++;
	//	}
	//	array[j + 1] = element;*/

	//	for (int k = j; k > position; k--) {
	//		Permutate(array, k, k - 1);
	//	}

	//}
	////this->permutations = perms
	//this->comparisons = comps--;
}

int InsertionSort::GetPermutations()
{
	return permutations;
}

int InsertionSort::GetComparisons()
{
	return comparisons;
}

bool InsertionSort::Compare(int first, int second)
{
	comparisons++;
	return second > first;
}

void InsertionSort::Permutate(std::vector<int>& array, int posFirst, int posSecond)
{
	permutations++;
	std::swap(array[posFirst], array[posSecond]);
}
