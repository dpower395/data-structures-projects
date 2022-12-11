#include "Sorting.h"

/*
  Please refer to sorting.h for documentation on each function.

   + ------ N O T E: -- Big scary warning! -----------------------------\
   |                                                                    |
   |    We clearly can't use unit testing to ensure that you're         |
   |    actually implementing the correct algorithm. I suppose we could |
   |    write some complicated static analysis checker. But it is       |
   |    easier to just look at it with our eyeballs.                    |
   |                                                                    |
   |    After the assignment is due, we will look at the most recent    |
   |    version of your implementation that got the highest score and   |
   |    make sure you're not doing something sneaky like using a        |
   |    standard library call to sort a vector.                         |
   |                                                                    |
   |    Anyone caught doing this (even though it is kinda funny) will   |
   |    be given 0 points on the assignment.                            |
   |                                                                    |
   `--------------------------------------------------------------------/ 
*/

// constructor, initialize class variables and pointers here if you need.
Sorting::Sorting(){
  // Your code here
}

//deconstructor,
Sorting::~Sorting(){
}

void Sorting::quicksort(vector<int>& data, int low_idx, int high_idx){
    if (high_idx - low_idx <= 0) {
        return;
    }
    int mid = quicksort_partition(data, low_idx, high_idx);
    quicksort(data, low_idx, mid-1);
    quicksort(data, mid+1, high_idx);
}

int Sorting::quicksort_partition(vector<int>& data, int low_idx, int high_idx){
    int i = low_idx;
    int j = low_idx;
    int l = high_idx;
    int temp;
    while (j < l) {
        if (data[j] < data[l]) {
            temp = data[i];
            data[i] = data[j];
            data[j] = temp;
            i++;
            j++;
        }
        else {
            j++;
        }
    }
    temp = data[i];
    data[i] = data[l];
    data[l] = temp;
    return i;
}

void Sorting::bubblesort(vector<int>& data){
    int i;
    int j;
    int temp;
    for (i = 0; i < data.size(); i++) {
        for (j = 0; j < data.size() - i - 1; j++) {
            if (data[j] > data[j+1]) {
                temp = data[j];
                data[j] = data[j+1];
                data[j+1] = temp;
            }
        }
    }
}

void Sorting::mergesort(vector<int>& data){
    int stop = data.size() - 1;
    merge_recurse(data, 0, stop);
}

void Sorting::merge_recurse(vector<int>& data, int start, int stop) {
    if (start == stop) {
        return;
    }
    int mid = (start + stop) / 2;
    merge_recurse(data, start, mid);
    merge_recurse(data, mid+1, stop);
    // create the left vector
    vector<int> left;
    for (int i = start; i <= mid; i++) {
        left.push_back(data[i]);
    }
    // create the right vector
    vector<int> right;
    for (int j = mid + 1; j <= stop; j++) {
        right.push_back(data[j]);
    }
    // merge the two and copy to data
    vector<int> result;
    merge(left, right, result);
    int count = 0;
    for (int k = start; k <= stop; k++) {
        data[k] = result[count];
        count++;
    }
}

void Sorting::merge(vector<int>& left, vector<int>& right, vector<int>& result){
    int i = 0;
    int j = 0;
    int n = left.size();
    int m = right.size();
    while (i < n and j < m) {
        if (left[i] < right[j]) {
            result.push_back(left[i]);
            i++;
        }
        else {
            result.push_back(right[j]);
            j++;
        }
    }
    while (i < n) {
        result.push_back(left[i]);
        i++;
    }
    while (j < m) {
        result.push_back(right[j]);
        j++;
    }
}

void Sorting::mystery_sort(vector<int>& data){
    // Combsort, a variation of bubblesort
    int gap = data.size();
    bool sorted = false;
    int i;
    int temp;
    while (not sorted) {
        gap = (gap * 10) / 13;
            if (gap <= 1) {
                gap = 1;
                sorted = true;
            }
        for (i = 0; i < data.size() - gap; i++) {
            if (data[i] > data[i+gap]) {
                temp = data[i];
                data[i] = data[i+gap];
                data[i+gap] = temp;
                sorted = false;
            }
        }
    }
    
}