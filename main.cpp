#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

mutex mtx;

void fillArray(int* arr, long long size, int min_val = 1, int max_val = 1000000) {
    for (long long i = 0; i < size; ++i) {
        arr[i] = rand() % (max_val - min_val + 1) + min_val;
    }
}

void findMinMaxThread(const int* arr, long long start, long long end, int &min_num, int &max_num) {
    for (long long i = start; i < end; ++i) {
        mtx.lock();

        if (arr[i] < min_num) {
            min_num = arr[i];
        }
        if (arr[i] > max_num) {
            max_num = arr[i];
        }

        mtx.unlock();
    }
}

void findMinMaxParallel(const int* arr, long long size, int &min_num, int &max_num) {
    min_num = numeric_limits<int>::max();
    max_num = numeric_limits<int>::min();

    int num_threads = 4;
    vector<thread> threads;
    long long segment_size = size / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        long long start = i * segment_size;
        long long end = (i == num_threads - 1) ? size : (i + 1) * segment_size;
        threads.emplace_back(findMinMaxThread, arr, start, end, ref(min_num), ref(max_num));
    }

    for (auto &t : threads) {
        t.join();
    }
}

int main() {
    srand(time(0));

    long long SIZE;
    cout << "Введіть розмір масиву: ";
    cin >> SIZE;

    int* task_array = new int[SIZE];

    fillArray(task_array, SIZE);

    auto start = chrono::high_resolution_clock::now();

    int min_num, max_num;
    findMinMaxParallel(task_array, SIZE, min_num, max_num);

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
