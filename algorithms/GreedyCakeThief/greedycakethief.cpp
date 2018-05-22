//
// Copyright (c) 2018 Tuan Q. Vu (tuan.vu58@yahoo.com). All rights reserved.
// Licensed under the MIT license. See LICENSE file in project root for details.
//
// Full problem description available at:
//  https://engineeringnotepad.wordpress.com/2018/05/22/algorithm-greedy-cake-thief/
//

#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

// Cake type structure
struct Cake {
  int weight;
  int value;

  Cake(int weight=0, int value=0) : weight(weight), value(value) {}
};

// Dynamic Programming bottom-up solution
// capacity := N, cakes.size() := M
// Time O(M*N), Space O(N)
int maxValueIterative(int capacity, vector<Cake> cakes) {

  // Auxillary array to keep maximum value at capacity k
  // Space : O(N)
  auto max_value = unique_ptr<int[]>(new int[capacity+1]());

  // Iteratively update max_value array for every type of cake
  // Time : O(M) iterations, takes O(N) each
  for (auto it = cakes.begin(); it != cakes.end(); it++) {
    int weight = (*it).weight;
    for (int k = 0; (k + weight) <= capacity; k++) {
      max_value[k + weight] = max(max_value[k + weight], max_value[k] + (*it).value);
    }
  }

  return max_value[capacity];
}

// Utility routine to calculate cost/weight ratio
static inline float getRatio(const Cake& c) {
  return c.value / static_cast<float>(c.weight);
}

// Greedy solution implemented with priority queue
// capacity := N, cakes.size() := M
// Time O(M^2*lgM), Space O(M)
int maxValueGreedyPQueue(int capacity, vector<Cake> cakes) {

  // Cake comparator object for priority queue
  // Select cake based on value/cost ratio; if ratios are equaled, pick the smaller weight
  class cakeComparator {
  public:
    bool operator() (const Cake& A, const Cake& B) {
      float ratioA = getRatio(A);
      float ratioB = getRatio(B);
      return (ratioA > ratioB) ? false : (ratioA == ratioB) ? A.weight > B.weight : true;
    }
  };

  // Building the queue
  // Time O(M), Space O(M)
  priority_queue<Cake, vector<Cake>, cakeComparator> queue;
  for (auto it = cakes.begin(); it != cakes.end(); it++) {
    queue.push(*it);
  }

  // Find maximum value possible
  // Main loop - O(M)
  // Inside loop:
  //   copy constructor      - O(M)
  //   while loop w/ heapify - O(M * lgM)
  // Total time - O(M * (M + M*lgM)) = O(M^2 + M^2*lgM) = O(M^2*lgM)
  int max_value = 0;
  while (!queue.empty()) {
    bool  repeat        = false;
    int   temp_value    = 0;
    auto  temp_queue    = queue;  // Time O(M) copy constructor
    int   temp_capacity = capacity;
    float prev_ratio    = -1.0;

    while (temp_capacity > 0 && !temp_queue.empty()) {
      Cake c = temp_queue.top();
      temp_queue.pop(); // Heapify takes time O(lgM)

      // If we have repeated ratios, then have to repeat until we don't
      if (prev_ratio == getRatio(c)) {
        repeat = true;
      }
      prev_ratio = getRatio(c);

      temp_value    += (temp_capacity / c.weight) * c.value;
      temp_capacity  = temp_capacity % c.weight;
    }
    max_value = max(max_value, temp_value);

    // If don't have to repeat, exit loop early
    if (!repeat) {
      break;
    }

    // Start with next cake type
    queue.pop();
  }

  return max_value;
}

// Greedy solution using in-place sorting
// capacity := N, cakes.size() := M
// Time O(M^2), Space O(1)
int maxValueGreedySort(int capacity, vector<Cake> cakes) {

  // In-place sort cake types based on cost/weight ratio
  // If ratios are equaled, prioritize the smaller weight
  // Time O(M*lgM)
  sort(cakes.begin(), cakes.end(), [](const Cake& A, const Cake& B) -> bool {
    float ratioA = getRatio(A);
    float ratioB = getRatio(B);
    return (ratioA > ratioB) ? false : (ratioA == ratioB) ? A.weight > B.weight : true;
  });

  // Find maximum value possible
  // Main loop - average case ??, worst case O(M)
  // Inside loop:
  //   for loop - O(M)
  // Total time - O(M * M) = O(M^2)
  int i         = 0;
  int max_value = 0;
  while (i < cakes.size()) {
    bool  repeat        = false;
    int   temp_value    = 0;
    int   temp_capacity = capacity;
    float prev_ratio    = -1.0;

    for (int j = i; temp_capacity > 0 && j < cakes.size(); j++) {
      // If we have repeated ratios, then have to repeat until we don't
      float current_ratio = getRatio(cakes[i]);
      if (prev_ratio == current_ratio) {
        repeat = true;
      }
      prev_ratio     = current_ratio;
      temp_value    += (temp_capacity / cakes[i].weight) * cakes[i].value;
      temp_capacity  = temp_capacity % cakes[i].weight;
    }
    max_value = max(max_value, temp_value);

    // If don't have to repeat, exit loop early
    if (!repeat) {
      break;
    }

    // Start with next cake type
    i++;
  }

  return max_value;
}

// Main program entry point
int main() {
  // Big capacity
  int capacity = 350000000;

  // Initialize cake types
  vector<Cake> cakes;
  cakes.push_back(Cake(2,2));
  cakes.push_back(Cake(3,3));
  cakes.push_back(Cake(5,5));
  cakes.push_back(Cake(6,6));

  clock_t start = clock();
  int max_value = maxValueIterative(capacity, cakes);
  clock_t end = clock();
  cout << "Iterative solution takes " << double(end - start) << " ticks - result " << max_value << endl;

  start = clock();
  max_value = maxValueGreedyPQueue(capacity, cakes);
  end = clock();
  cout << "Greedy p-queue solution takes " << double(end - start) << " ticks - result " << max_value << endl;

  start = clock();
  max_value = maxValueGreedySort(capacity, cakes);
  end = clock();
  cout << "Greedy sorting solution takes " << double(end - start) << " ticks - result " << max_value << endl;

  return 0;
}