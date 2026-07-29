#include <iostream>
#include <chrono>
#include <cstdlib>
#include <iomanip>
using namespace std;
using namespace std::chrono;

struct Record { int id; int value; };

int sizes[] = {100, 500, 1000, 5000, 10000, 50000};
int numSizes = 6;


const int SEARCH_REPEATS = 200000;

void generateRandomData(Record arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i].id = i + 1;
        arr[i].value = rand() % 100000 + 1;
    }
}


void generateSortedData(Record arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i].id = i + 1;
        arr[i].value = i + 1; 
    }
}

void copyArr(Record src[], Record dst[], int n) {
    for (int i = 0; i < n; i++) dst[i] = src[i];
}


void selectionSort(Record arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j].value < arr[minIdx].value) minIdx = j;
        }
        Record t = arr[i]; arr[i] = arr[minIdx]; arr[minIdx] = t;
    }
}

int partition(Record arr[], int low, int high) {
    int pivot = arr[high].value;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j].value <= pivot) {
            i++;
            Record t = arr[i]; arr[i] = arr[j]; arr[j] = t;
        }
    }
    Record t = arr[i + 1]; arr[i + 1] = arr[high]; arr[high] = t;
    return i + 1;
}
void quickSort(Record arr[], int low, int high) {
    if (low < high) {
        int p = partition(arr, low, high);
        quickSort(arr, low, p - 1);
        quickSort(arr, p + 1, high);
    }
}

void merge(Record arr[], int left, int mid, int right) {
    int ls = mid - left + 1, rs = right - mid;
    Record* L = new Record[ls];
    Record* R = new Record[rs];
    for (int i = 0; i < ls; i++) L[i] = arr[left + i];
    for (int j = 0; j < rs; j++) R[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < ls && j < rs) {
        if (L[i].value <= R[j].value) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < ls) arr[k++] = L[i++];
    while (j < rs) arr[k++] = R[j++];
    delete[] L;
    delete[] R;
}
void mergeSort(Record arr[], int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

int binarySearch(Record arr[], int n, int target) {
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (arr[mid].value == target) return mid;
        else if (arr[mid].value < target) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

int interpolationSearch(Record arr[], int n, int target) {
    int low = 0, high = n - 1;
    while (low <= high && target >= arr[low].value && target <= arr[high].value) {
        if (arr[high].value == arr[low].value) {
            if (arr[low].value == target) return low;
            return -1;
        }
        int pos = low + (long long)(target - arr[low].value) * (high - low) / (arr[high].value - arr[low].value);
        if (arr[pos].value == target) return pos;
        else if (arr[pos].value < target) low = pos + 1;
        else high = pos - 1;
    }
    return -1;
}


double timeInMs(void (*fn)(Record*, int), Record arr[], int n) {
    auto start = high_resolution_clock::now();
    fn(arr, n);
    auto end = high_resolution_clock::now();
    return duration<double, milli>(end - start).count();
}

int main() {
    srand(42); 

    cout << fixed << setprecision(6);

    cout << "\n===== SORTING RESULTS\n";
    cout << "n,SelectionSort(ms),QuickSort_Average(ms),QuickSort_Worst(ms),MergeSort(ms)\n";

    for (int s = 0; s < numSizes; s++) {
        int n = sizes[s];

        
        Record* base = new Record[n];
        generateRandomData(base, n);
        Record* arr1 = new Record[n];
        copyArr(base, arr1, n);
        auto t1 = high_resolution_clock::now();
        selectionSort(arr1, n);
        auto t2 = high_resolution_clock::now();
        double selTime = duration<double, milli>(t2 - t1).count();

        
        Record* arr2 = new Record[n];
        copyArr(base, arr2, n);
        auto t3 = high_resolution_clock::now();
        quickSort(arr2, 0, n - 1);
        auto t4 = high_resolution_clock::now();
        double qsAvgTime = duration<double, milli>(t4 - t3).count();

        
        Record* sortedData = new Record[n];
        generateSortedData(sortedData, n);
        auto t5 = high_resolution_clock::now();
        quickSort(sortedData, 0, n - 1);
        auto t6 = high_resolution_clock::now();
        double qsWorstTime = duration<double, milli>(t6 - t5).count();

       
        Record* arr3 = new Record[n];
        copyArr(base, arr3, n);
        auto t7 = high_resolution_clock::now();
        mergeSort(arr3, 0, n - 1);
        auto t8 = high_resolution_clock::now();
        double msTime = duration<double, milli>(t8 - t7).count();

        cout << n << "," << selTime << "," << qsAvgTime << "," << qsWorstTime << "," << msTime << "\n";

        delete[] base; delete[] arr1; delete[] arr2; delete[] sortedData; delete[] arr3;
    }

    cout << "\nNote: Selection Sort and Merge Sort always do the same amount of\n";
    cout << "work no matter how the data is arranged, so their best, average,\n";
    cout << "and worst case times are effectively the same -- only one column\n";
    cout << "is shown for each. Quick Sort is different: its worst case (already\n";
    cout << "sorted data) is shown separately from its average case (random data).\n";

   
    cout << "\n\n===== SEARCHING RESULTS\n";
    cout << "n,Binary_Best(us),Binary_Avg(us),Binary_Worst(us),Interp_Best(us),Interp_Avg(us),Interp_Worst(us)\n";

    for (int s = 0; s < numSizes; s++) {
        int n = sizes[s];

        Record* sortedArr = new Record[n];
        generateSortedData(sortedArr, n); 

        int bestTarget = sortedArr[n / 2].value;      
        int avgTarget  = sortedArr[rand() % n].value; 
        int worstTarget = -1;                          

       
        volatile long long sink = 0;

        
        auto bStart = high_resolution_clock::now();
        for (int r = 0; r < SEARCH_REPEATS; r++) sink += binarySearch(sortedArr, n, bestTarget);
        auto bEnd = high_resolution_clock::now();
        double binBest = duration<double, micro>(bEnd - bStart).count() / SEARCH_REPEATS;

        bStart = high_resolution_clock::now();
        for (int r = 0; r < SEARCH_REPEATS; r++) sink += binarySearch(sortedArr, n, avgTarget);
        bEnd = high_resolution_clock::now();
        double binAvg = duration<double, micro>(bEnd - bStart).count() / SEARCH_REPEATS;

        bStart = high_resolution_clock::now();
        for (int r = 0; r < SEARCH_REPEATS; r++) sink += binarySearch(sortedArr, n, worstTarget);
        bEnd = high_resolution_clock::now();
        double binWorst = duration<double, micro>(bEnd - bStart).count() / SEARCH_REPEATS;

        
        auto iStart = high_resolution_clock::now();
        for (int r = 0; r < SEARCH_REPEATS; r++) sink += interpolationSearch(sortedArr, n, bestTarget);
        auto iEnd = high_resolution_clock::now();
        double interpBest = duration<double, micro>(iEnd - iStart).count() / SEARCH_REPEATS;

        iStart = high_resolution_clock::now();
        for (int r = 0; r < SEARCH_REPEATS; r++) sink += interpolationSearch(sortedArr, n, avgTarget);
        iEnd = high_resolution_clock::now();
        double interpAvg = duration<double, micro>(iEnd - iStart).count() / SEARCH_REPEATS;

        iStart = high_resolution_clock::now();
        for (int r = 0; r < SEARCH_REPEATS; r++) sink += interpolationSearch(sortedArr, n, worstTarget);
        iEnd = high_resolution_clock::now();
        double interpWorst = duration<double, micro>(iEnd - iStart).count() / SEARCH_REPEATS;

        cout << n << "," << binBest << "," << binAvg << "," << binWorst << ","
             << interpBest << "," << interpAvg << "," << interpWorst << "\n";

        delete[] sortedArr;
    }

    return 0;
}
