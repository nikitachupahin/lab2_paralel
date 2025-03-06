#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <vector>
#include <atomic>

using namespace std;

void fillArray(int* arr, long long size, int min_val = 1, int max_val = 1000000) {
    for (long long i = 0; i < size; ++i) {
        arr[i] = rand() % (max_val - min_val + 1) + min_val;
    }
}

void findMinMaxThread(const int* arr, long long start, long long end, atomic<int>& amin_num, atomic<int>& amax_num) {
    for (long long i = start; i < end; ++i) {
        int expected, desired;

        if (amin_num.load() > arr[i]) {
            do {
                expected = amin_num.load();
                desired = arr[i];
                if (desired > expected) {
                    break;
                }
            } while (!amin_num.compare_exchange_weak(expected, desired));
        }

        if (amax_num.load() < arr[i]) {
            do {
                expected = amax_num.load();
                desired = arr[i];
                if (desired < expected) {
                    break;
                }
            } while (!amax_num.compare_exchange_weak(expected, desired));
        }
    }
}

void findMinMaxParallel(const int* arr, long long size, atomic<int>& amin_num, atomic<int>& amax_num) {
    amin_num = numeric_limits<int>::max();
    amax_num = numeric_limits<int>::min();

    int num_threads = 8;
    vector<thread> threads;
    long long segment_size = size / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        long long start = i * segment_size;
        long long end = (i == num_threads - 1) ? size : (i + 1) * segment_size;
        threads.emplace_back(findMinMaxThread, arr, start, end, ref(amin_num), ref(amax_num));
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

    atomic<int> amin_num, amax_num;
    findMinMaxParallel(task_array, SIZE, amin_num, amax_num);

    int sum = amin_num + amax_num;

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Мінімальний елемент: " << amin_num << endl;
    cout << "Максимальний елемент: " << amax_num << endl;
    cout << "Сума мінімального та максимального елементів: " << sum << endl;
    cout << "Час виконання: " << duration.count() << " мс" << endl;

    delete[] task_array;
    return 0;
}
