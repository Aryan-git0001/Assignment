#include <iostream>
#include <cstdlib>   
#include <cctype> 
using namespace std;

const int SIZE = 100; 


struct Record {
    int id;
    int value;
};


void fillRecords(Record records[], int size) {
    for (int i = 0; i < size; i++) {
        records[i].id = i + 1;                 
        records[i].value = rand() % 1000 + 1;  
    }
}


void displayRecords(Record records[], int size) {
    cout << "\n----- All Records -----\n";
    for (int i = 0; i < size; i++) {
        cout << "ID: " << records[i].id << "\tValue: " << records[i].value << endl;
    }
}


void selectionSort(Record records[], int size) {
    for (int i = 0; i < size - 1; i++) {
        int smallestIndex = i;
        for (int j = i + 1; j < size; j++) {
            if (records[j].value < records[smallestIndex].value) {
                smallestIndex = j;
            }
        }
        
        Record temp = records[i];
        records[i] = records[smallestIndex];
        records[smallestIndex] = temp;
    }
}


int partition(Record records[], int low, int high) {
    int pivotValue = records[high].value; 
    int i = low - 1;                      

    for (int j = low; j < high; j++) {
        if (records[j].value <= pivotValue) {
            i++;
            
            Record temp = records[i];
            records[i] = records[j];
            records[j] = temp;
        }
    }

    
    Record temp = records[i + 1];
    records[i + 1] = records[high];
    records[high] = temp;

    return i + 1; 
}


void quickSort(Record records[], int low, int high) {
    if (low < high) {
        int pivotIndex = partition(records, low, high);
        quickSort(records, low, pivotIndex - 1);   
        quickSort(records, pivotIndex + 1, high);  
    }
}


void merge(Record records[], int left, int mid, int right) {
    int leftSize = mid - left + 1;
    int rightSize = right - mid;

    
    Record leftArray[leftSize];
    Record rightArray[rightSize];

    for (int i = 0; i < leftSize; i++) {
        leftArray[i] = records[left + i];
    }
    for (int j = 0; j < rightSize; j++) {
        rightArray[j] = records[mid + 1 + j];
    }

    
    int i = 0, j = 0, k = left;

    while (i < leftSize && j < rightSize) {
        if (leftArray[i].value <= rightArray[j].value) {
            records[k] = leftArray[i];
            i++;
        } else {
            records[k] = rightArray[j];
            j++;
        }
        k++;
    }

    
    while (i < leftSize) {
        records[k] = leftArray[i];
        i++;
        k++;
    }
    while (j < rightSize) {
        records[k] = rightArray[j];
        j++;
        k++;
    }
}


void mergeSort(Record records[], int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSort(records, left, mid);
        mergeSort(records, mid + 1, right);
        merge(records, left, mid, right);
    }
}




bool isSorted(Record records[], int size) {
    for (int i = 1; i < size; i++) {
        if (records[i - 1].value > records[i].value) {
            return false;
        }
    }
    return true;
}


int binarySearch(Record records[], int size, int target) {
    int low = 0;
    int high = size - 1;

    while (low <= high) {
        int mid = (low + high) / 2;

        if (records[mid].value == target) {
            return mid;               
        } else if (records[mid].value < target) {
            low = mid + 1;            
        } else {
            high = mid - 1;           
        }
    }
    return -1; 
}


int interpolationSearch(Record records[], int size, int target) {
    int low = 0;
    int high = size - 1;

    while (low <= high && target >= records[low].value && target <= records[high].value) {

        
        if (records[high].value == records[low].value) {
            if (records[low].value == target) {
                return low;
            }
            return -1;
        }

        
        int pos = low + (long long)(target - records[low].value) *
                  (high - low) / (records[high].value - records[low].value);

        if (records[pos].value == target) {
            return pos;                 
        } else if (records[pos].value < target) {
            low = pos + 1;              
        } else {
            high = pos - 1;             
        }
    }
    return -1; 
}


void sortMenu(Record records[], int size) {
    cout << "\n----- Sort Records (Ascending Order) -----\n";
    cout << "1. Selection Sort\n";
    cout << "2. Quick Sort\n";
    cout << "3. Merge Sort\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;

    if (choice == 1) {
        selectionSort(records, size);
        cout << "Records sorted using Selection Sort.\n";
    } else if (choice == 2) {
        quickSort(records, 0, size - 1);
        cout << "Records sorted using Quick Sort.\n";
    } else if (choice == 3) {
        mergeSort(records, 0, size - 1);
        cout << "Records sorted using Merge Sort.\n";
    } else {
        cout << "Invalid choice.\n";
    }
}


void searchMenu(Record records[], int size) {
    cout << "\n----- Search for a Number -----\n";
    cout << "1. Binary Search\n";
    cout << "2. Interpolation Search\n";
    cout << "(Both require the records to be sorted first - Menu option B)\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;

    if (choice != 1 && choice != 2) {
        cout << "Invalid choice.\n";
        return;
    }

    if (!isSorted(records, size)) {
        cout << "The records are not sorted yet.\n";
        cout << "Please sort them first (Menu option B) before searching.\n";
        return;
    }

    cout << "Enter the number you want to search for: ";
    int target;
    cin >> target;

    int index = -1;

    if (choice == 1) {
        index = binarySearch(records, size, target);
    } else {
        index = interpolationSearch(records, size, target);
    }

    if (index != -1) {
        cout << "Found! Value " << target << " belongs to Record ID "
             << records[index].id << " (position " << index << ").\n";
    } else {
        cout << "Value " << target << " was not found in the records.\n";
    }
}


int main() {
    Record records[SIZE];
    fillRecords(records, SIZE);

    cout << "100 dummy records have been generated.\n";

    char letter;

    do {
        cout << "\n===== MAIN MENU =====\n";
        cout << "A. Display All Records\n";
        cout << "B. Sort Records\n";
        cout << "C. Search for Number\n";
        cout << "D. Exit Application\n";
        cout << "Enter your choice: ";
        cin >> letter;
        letter = toupper(letter);

        switch (letter) {
            case 'A':
                displayRecords(records, SIZE);
                break;
            case 'B':
                sortMenu(records, SIZE);
                break;
            case 'C':
                searchMenu(records, SIZE);
                break;
            case 'D':
                cout << "Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please enter A, B, C, or D.\n";
        }

    } while (letter != 'D');

    return 0;
}
