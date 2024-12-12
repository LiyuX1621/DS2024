#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;

// Bubble Sort
void bubbleSort(vector<int>& arr) {
    for (size_t i = 0; i < arr.size() - 1; ++i) {
        for (size_t j = 0; j < arr.size() - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Insertion Sort
void insertionSort(vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        int key = arr[i];
        size_t j = i - 1;
        while (j < arr.size() && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

// Selection Sort
void selectionSort(vector<int>& arr) {
    for (size_t i = 0; i < arr.size() - 1; ++i) {
        size_t minIndex = i;
        for (size_t j = i + 1; j < arr.size(); ++j) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        swap(arr[i], arr[minIndex]);
    }
}

// Merge Sort Helper
void merge(vector<int>& arr, int left, int mid, int right) {
    vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        }
        else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];

    for (i = left, k = 0; i <= right; ++i, ++k) {
        arr[i] = temp[k];
    }
}

// Iterative Merge Sort
void mergeSortIterative(vector<int>& arr) {
    int n = arr.size();
    vector<int> temp(arr.size());
    for (int width = 1; width < n; width *= 2) {
        for (int i = 0; i < n; i += 2 * width) {
            int left = i;
            int mid = min(i + width - 1, n - 1);
            int right = min(i + 2 * width - 1, n - 1);
            if (mid < right) {
                merge(arr, left, mid, right);
            }
        }
    }
}

// Quick Sort Partition
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j] < pivot) {
            swap(arr[++i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Quick Sort with Random Pivot
int randomPartition(vector<int>& arr, int low, int high) {
    int randomPivot = low + rand() % (high - low + 1);
    swap(arr[randomPivot], arr[high]);
    return partition(arr, low, high);
}

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = randomPartition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Heapify Helper
void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

// Heap Sort
void heapSort(vector<int>& arr) {
    for (int i = arr.size() / 2 - 1; i >= 0; --i) {
        heapify(arr, arr.size(), i);
    }

    for (int i = arr.size() - 1; i > 0; --i) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// Measure Execution Time
void measureSortPerformance(void (*sortFunc)(vector<int>&), vector<int>& arr, const string& name, vector<double>& results) {
    auto start = high_resolution_clock::now();
    sortFunc(arr);
    auto end = high_resolution_clock::now();
    double timeTaken = duration_cast<microseconds>(end - start).count() / 1000.0;
    results.push_back(timeTaken);
    cout << name << " took " << fixed << setprecision(2) << timeTaken << " milliseconds." << endl;
}

int main() {
    vector<int> original;
    for (int i = 0; i < 10000; ++i) {
        original.push_back(rand());
    }

    vector<int> arr;

    cout << "\nPerformance Comparison of Sorting Algorithms:\n";
    cout << "-------------------------------------------------\n";
    cout << "| Test Case      | Bubble | Insertion | Selection | Merge | Quick | Heap |\n";
    cout << "-------------------------------------------------\n";

    // Test on sorted, reverse sorted, and random data
    for (const string& testCase : { "Sorted", "Reverse Sorted", "Random" }) {
        vector<double> results;

        if (testCase == "Sorted") {
            sort(original.begin(), original.end());
        }
        else if (testCase == "Reverse Sorted") {
            sort(original.rbegin(), original.rend());
        }
        else {
            random_shuffle(original.begin(), original.end());
        }

        arr = original;
        measureSortPerformance(bubbleSort, arr, "Bubble Sort", results);

        arr = original;
        measureSortPerformance(insertionSort, arr, "Insertion Sort", results);

        arr = original;
        measureSortPerformance(selectionSort, arr, "Selection Sort", results);

        arr = original;
        measureSortPerformance(mergeSortIterative, arr, "Merge Sort", results);

        arr = original;
        measureSortPerformance([](vector<int>& a) { quickSort(a, 0, a.size() - 1); }, arr, "Quick Sort", results);

        arr = original;
        measureSortPerformance(heapSort, arr, "Heap Sort", results);

        cout << "| " << setw(15) << testCase << " | "
            << setw(6) << fixed << setprecision(2) << results[0] << " | "
            << setw(9) << results[1] << " | "
            << setw(9) << results[2] << " | "
            << setw(5) << results[3] << " | "
            << setw(5) << results[4] << " | "
            << setw(4) << results[5] << " |" << endl;
    }

    cout << "-------------------------------------------------\n";
    return 0;
}
