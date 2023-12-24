#ifndef INSERTIONSORT_H
#define INSERTIONSORT_H

#include <QVector>
#include <QProgressBar>

class InsertionSort
{
private:
    int comparisons;
    int permutations;
public:
    InsertionSort();
    ~InsertionSort() = default;
    void Sort(QVector<int> &array, QProgressBar *progressBar);
    int GetPermutations();
    int GetComparisons();
    bool Compare(int first, int second);
    void Permutate(QVector<int> &array, int posFirst, int posSecond);

};



#endif // INSERTIONSORT_H
