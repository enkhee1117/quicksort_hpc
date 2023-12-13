#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <mpi.h>
#include <algorithm>

using namespace std;
using namespace std::chrono;

void quickSort(vector<int>& arr, int low, int high);
int partition(vector<int>& arr, int low, int high);
void printArray(const vector<int>& arr);

int main(int argc, char* argv[]) {

//    MPI initial:
    int world_rank, world_size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);


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
//    Global DATA:
    vector<int> global_data;
    if(world_rank == 0){
        vector<int> pattern = {3, 1, 4, 90, 5, 9, 200, 600, 500, 3, 5};

        global_data.reserve(array_length);
        while (global_data.size() < array_length) {
            for (int val : pattern) {
                if (global_data.size() < array_length) {
                    global_data.push_back(val);
                } else {
                    break;
                }
            }
        }
    }

    int local_n = array_length / world_size;
    vector<int> local_data(local_n);

//  Measure the time taken by QuickSort:
    auto start = high_resolution_clock::now();

    MPI_Scatter(global_data.data(), local_n, MPI_INT, local_data.data(), local_n, MPI_INT, 0, MPI_COMM_WORLD);
    // Perform local quicksort
    quickSort(local_data, 0, local_n - 1);
    // Gather the sorted subarrays into the global_data array
    MPI_Gather(local_data.data(), local_n, MPI_INT, global_data.data(), local_n, MPI_INT, 0, MPI_COMM_WORLD);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

//    Sorted array:
    cout << "Sorted array: \n";
    printArray(arr);
//    Duration:
    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl;

    MPI_Finalize();
    return 0;
}

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        {
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        };
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

