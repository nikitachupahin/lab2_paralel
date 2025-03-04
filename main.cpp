#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;

void fillArray(int* arr, long long size, int min_val = 1, int max_val = 1000000) {
    for (long long i = 0; i < size; ++i) {
        arr[i] = rand() % (max_val - min_val + 1) + min_val;
    }
}

void findMinMax(const int* arr, long long size, int &min_num, int &max_num) {
    min_num = numeric_limits<int>::max();
    max_num = numeric_limits<int>::min();

    for (long long i = 0; i < size; ++i) {
        if (arr[i] < min_num) {
            min_num = arr[i];
        }
        if (arr[i] > max_num) {
            max_num = arr[i];
        }
    }
}

int main() {
    srand(time(0));

    long long SIZE;
    cout << "Введіть розмір масиву : ";
    cin >> SIZE;

    int* task_array = new (nothrow) int[SIZE];
    if (!task_array) {
        cerr << "Помилка виділення пам'яті!" << endl;
        return 1;
    }

    fillArray(task_array, SIZE);

    auto start = chrono::high_resolution_clock::now();

    int min_num, max_num;
    findMinMax(task_array, SIZE, min_num, max_num);

    int sum = min_num + max_num;

    auto end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Мінімальний елемент: " << min_num << endl;
    cout << "Максимальний елемент: " << max_num << endl;
    cout << "Сума мінімального та максимального елементів: " << sum << endl;
    cout << "Час виконання: " << duration.count() << " мс" << endl;

    delete[] task_array;

    return 0;
}
