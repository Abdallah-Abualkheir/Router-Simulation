// Authors: Kaelan Kramer, Abdallah Abualkheir
#include <iostream>
#include <thread>
#include <vector>

#include "..\headers\routerv.h"
using std::cout;
using std::endl;
using std::thread;
using std::vector;
using std::cin;
int MAX_INT = 0x7FFFFFFF;
vector<vector<int>> ara;

// A function for printing a 2d array(in this case vector of vectors)
void print2darray(vector<vector<int>> ara) {
  for (int i = 0; i < ara.size(); i++) {
    for (int j = 0; j < ara[i].size(); j++) {
      int t = ara[i][j];
      cout << t << ' ';
    }
    cout << endl;
  }
}

// A function for setting the two d table to infite so routers can be set on it (Technically no longer need this function)
vector<vector<int>> setup(vector<vector<int>> ara) {
  for (int i = 0; i < ara.size(); i++) {
    for (int j = 0; j < ara[i].size(); j++) {
      ara[i][j] = MAX_INT;
    }
  }
  return ara;
}

// A utility function to find the vertex with minimum distance value, from
// the set of vertices not yet included in shortest path tree
int minDistance(vector<int> optimal, vector<bool> bestSet) {
  // Initialize min value
  int min = MAX_INT, min_index;

  for (int v = 0; v < ara.size(); v++)
    if (bestSet[v] == false && optimal[v] <= min)
      min = optimal[v], min_index = v;

  // return index of min value, from start router to "min_index" router
  return min_index;
}

// Find shoretest paths for routers
// Resource:
// https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/
void Dijkstra(router r) {
  // graph is ara, set to known connections
  // starting vertext/router
  int rt = r.getID();
  vector<int> optimal(ara.size()); // vector of current shortest distances
  vector<bool> bestSet(ara.size());  // vectors of bools, set to true if shortest path is set
  // Set all optimal known values to infinity, and booleans to false
  for (int i = 0; i < ara.size(); i++) {
    optimal[i] = MAX_INT;
    bestSet[i] = false;
  }
  optimal[r.getID()] = 0;

  // Find shortest path for all routers
  for (int count = 0; count < ara.size() - 1; count++) {
    // Pick the minimum distance vertex from the set of vertices not
    // yet processed. u is always equal to src in the first iteration.
    int u = minDistance(optimal, bestSet);

    // Mark the picked vertex as processed
    bestSet[u] = true;

    // Update optimal value of the adjacent vertices of the picked vertex.
    for (int v = 0; v < ara.size(); v++) {
      // Update optimal[v] only if is not in bestSet, there is an edge from
      // u to v, and total weight of path from src to v through u is
      // smaller than current value of optimal[v]
      if (!bestSet[v] && ara[u][v] && optimal[u] != MAX_INT && optimal[u] + ara[u][v] < optimal[v])
        optimal[v] = optimal[u] + ara[u][v];
    }
  }
  for (int i = 0; i < optimal.size(); i++) {
    cout << "Shortest distance from " << rt << " to router " << i << " is " << optimal[i] << endl;
  }
}

// implimentation has to manipulate global without setting global i.e. void
void Dijkthread(router r) {
  // Set table based off router connections
  int begin = r.getID();
  for (int i = 0; i < r.getConnections().size(); i++) {
    int end = r.getConnections()[i];
    int cost = r.getCosts()[i];
    if (ara[begin][end] > cost) {
      ara[begin][end] = cost;
      ara[end][begin] = cost;
    }
  }
}

// Checks vector of vectors for any values that haven't been set and sets them to 0 so Dijkstra's can run properly
void format() {
  for(int i = 0; i < ara.size(); i++) {
    for (int j = 0; j < ara[i].size(); j++) {
      if (ara[i][j] == MAX_INT) {
        ara[i][j] = 0;
      }
    }
  }
}

int main() {
  //Create routers
  router zero = router(0);
  zero.addrouter(0, 0);
  zero.addrouter(1, 6);
  zero.addrouter(5, 7);
  router one = router(1);
  one.addrouter(1, 0);
  one.addrouter(0, 6);
  one.addrouter(2, 3);
  one.addrouter(3, 5);
  one.addrouter(5, 4);
  router two = router(2);
  two.addrouter(2, 0);
  two.addrouter(1, 3);
  two.addrouter(3, 9);
  two.addrouter(4, 2);
  router three = router(3);
  three.addrouter(3, 0);
  three.addrouter(1, 5);
  three.addrouter(2, 9);
  router four = router(4);
  four.addrouter(4, 0);
  four.addrouter(2, 2);
  four.addrouter(5, 7);
  four.addrouter(6, 6);
  router five = router(5);
  five.addrouter(5, 0);
  five.addrouter(0, 7);
  five.addrouter(1, 4);
  five.addrouter(4, 7);
  five.addrouter(6, 3);
  router six = router(6);
  six.addrouter(6, 0);
  six.addrouter(4, 6);
  six.addrouter(5, 3);
  vector<router> routers;
  routers.push_back(zero);
  routers.push_back(one);
  routers.push_back(two);
  routers.push_back(three);
  routers.push_back(four);
  routers.push_back(five);
  routers.push_back(six);

  // Create a 2D array Resource:
  // https://www.dcs.bbk.ac.uk/~roger/cpp/week13.htm#:~:text=To%20initialize%20a%20two%2Ddimensional,vector(5))%3B

  // push back vectors of a certain size to ara to create vector of vectors (2D array)
  for (int i = 0; i < 7; i++) {
    ara.push_back(vector<int>(7));
  }

// set all values to infinite, technically no longer viable
  ara = setup(ara);

// a few comments to help in implementation, check array, know how array works, and know how threads work
  // print2darray(ara);
  // cout << ara[0][0]; proving 0, 0 which is top left
  // thread first(foo, zero);  // spawn new thread that calls foo()

  //Routers will add their conenctions to the vector of vectors 
  // Spawn Threads to create router map
  thread t_zero(Dijkthread, zero);
  thread t_one(Dijkthread, one);
  thread t_two(Dijkthread, two);
  thread t_three(Dijkthread, three);
  thread t_four(Dijkthread, four);
  thread t_five(Dijkthread, five);
  thread t_six(Dijkthread, six);
  // Join Threads
  t_zero.join();
  t_one.join();
  t_two.join();
  t_three.join();
  t_four.join();
  t_five.join();
  t_six.join();
  format();
  //Choose router 
  int rnum;
  cout << "Please choose which router to have as your source(0-6): ";
  std::cin >> rnum;
  cout << endl;
  for (int i = 0; i < routers.size(); i++) {
    if (rnum == routers[i].getID())
    //Run Dijkstras on specified router
      Dijkstra(routers[i]);
  }

  return 0;
}