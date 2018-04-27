//
// Copyright (c) 2018 Tuan Q. Vu (tuan.vu58@yahoo.com). All rights reserved.
// Licensed under the MIT license. See LICENSE file in project root for details.
//
// Full problem description available at:
//  https://engineeringnotepad.wordpress.com/2018/04/21/algorithm-nuts-bolts/
//

#include <iostream>
#include <algorithm>
#include <random>
#include <vector>

using namespace std;

// Worker functions
void sortNutsBolts(vector<int>& nuts, vector<int>& bolts, int start, int end);
int  partition(vector<int>& A, int start, int end, int pivot);
void swap(vector<int>& A, int from, int to);

// Main sorting function
void sortNutsBolts(vector<int>& nuts, vector<int>& bolts) {
  if (nuts.size() != bolts.size()) {
    throw "Missing nuts and bolts";
  }

  // Guarantee quick sort performance
  random_shuffle(bolts.begin(), bolts.end());

  sortNutsBolts(nuts, bolts, 0, nuts.size()-1);
}

// Main worker function:
//  - Sort nuts using a random bolt into too-small and too-large partitions
//  - Using nut found, repeat previous step for bolts
//  - Recursively sort nuts and bolts
void sortNutsBolts(vector<int>& nuts, vector<int>& bolts, int start, int end) {
  // Base cases of 0 and 1 element
  if (end - start + 1 < 2) return;

  // Should have the same number of nuts/bolts smaller and larger than the pivot
  // else, either duplicates or missing nuts/bolts
  int n = partition(nuts , start, end, bolts[(start+end)/2]);
  int b = partition(bolts, start, end, nuts[n]);
  if (n < 0 || n != b) {
    throw "Nuts and bolts not unique or missing";
  }

  // Recursively sort the too-small and too-large piles
  sortNutsBolts(nuts, bolts, start, n-1);
  sortNutsBolts(nuts, bolts, n+1  , end);
}

// Modified quick-sort partition:
//  - Don't know where pivot is or even if it exists
//  - Pivot is inside the array so will have unnecessary swaps but will save time overall
int partition(vector<int>& A, int start, int end, int pivot) {
  int p = -1;
  while (start < end) {
    while (start < end && A[start] < pivot) { start++; }
    while (start < end && A[end]   > pivot) { end--;   }
    if (start <= end) {
      swap(A, start, end);
      p = (A[start] == pivot) ? start : (A[end] == pivot) ? end : p;
    }
  }
  return (p < 0) ? -1 : end;
}

void swap(vector<int>& A, int from, int to) {
  int temp = A[to];
  A[to]    = A[from];
  A[from]  = temp;
}

void print(vector<int>& A) {
  size_t N = A.size();
  for (size_t i = 0; i < N; i++) {
    cout << A[i] << " ";
  }
  cout << endl;
}

// Main program entry point
int main() {
  vector<int> nuts;
  vector<int> bolts;

  int N = 10;
  for (int i = 0; i < N; i++) {
    nuts.push_back(i);
    bolts.push_back(i);
  }

  random_shuffle(nuts.begin() , nuts.end());
  random_shuffle(bolts.begin(), bolts.end());

  cout << "Before sorting:" << endl;
  print(nuts); print(bolts);

  sortNutsBolts(nuts, bolts);

  cout << "After sorting:" << endl;
  print(nuts); print(bolts);

  return 0;
}