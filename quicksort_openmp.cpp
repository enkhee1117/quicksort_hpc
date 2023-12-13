#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#ifdef _OPENMP
#include <omp.h>
#endif

using namespace std;
using namespace std::chrono;

void quickSort(vector<int>& arr, int low, int high, int depth);
int partition(vector<int>& arr, int low, int high);
void printArray(const vector<int>& arr);

int main(int argc, char* argv[]) {

//    Limiting number of threads:
    omp_set_num_threads(8);

//    #pragma omp parallel
//    {
//        int thread_id = omp_get_thread_num();
//        int total_threads = omp_get_num_threads();
//        std::cout << "Thread " << thread_id << " of " << total_threads << std::endl;
//    }

//    Default array length
    int array_length = 1000;
//    Check the argument for array length:
    if(argc >= 2){
        array_length = atoi(argv[1]);
        if (array_length <= 0){
            cerr << "Array length should be a positive integer" << endl;
            return 1;
        }
    }

//    Creating an array and filling it by repeated pattern:
    vector<int> pattern = {3, 1, 4, 90, 5, 9, 200, 600, 500, 3, 5};
    vector<int> arr;
    arr.reserve(array_length);
    while (arr.size() < array_length){
        for(int val : pattern){
            if(arr.size() < array_length){
                arr.push_back(val);
            } else {
                break;
            }
        }
    }
//    Array when unsorted:
//    cout << "Unsorted version of the array: \n";
//    printArray(arr);

//  Measure the time taken by QuickSort:
    auto start = high_resolution_clock::now();
    quickSort(arr, 0, arr.size() - 1, 0);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

//    Sorted array:
    cout << "Sorted array: \n";
    printArray(arr);
//    Duration:
    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl;

    return 0;
}

void quickSort(vector<int>& arr, int low, int high, int depth = 0) {
    if (low < high) {
        int pi = partition(arr, low, high);

        if(depth < 9){
            #pragma omp parallel sections
            {
                #pragma omp section
                quickSort(arr, low, pi - 1, depth + 1);
                #pragma omp section
                quickSort(arr, pi + 1, high, depth + 1);
            };
            } else {
            quickSort(arr, low, pi - 1, depth + 1);
            quickSort(arr, pi + 1, high, depth + 1);
        }
    }

}

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void printArray(const vector<int>& arr) {
    if(arr.empty()) return;

    int prev = arr[0];
    cout << prev << " ";

    for (size_t i = 1 ; i < arr.size(); ++i) {
        if(arr[i] != prev){
            cout << arr[i] << " ";
            prev = arr[i];
        }
    }

    cout << endl;
}
