#ifndef _ROUTER_H_
#define _ROUTER_H_

#include <iostream>
#include <map>
#include <vector>
using std::vector;

class router {
 private:
  int id;
  vector<int> connections;
  vector<int> costs;  // connections[i] must correlate directly to costs[i]

 public:
  router(int a) {
    id = a;
    vector<int> connections;
    vector<int> costs;
  }

  // returns connected routers, but not costs like map version
  vector<int> getConnections() { return this->connections; }

  // returns costs, correlated directly by index to a connection in the
  // connections vector
  vector<int> getCosts() { return this->costs; }

  // returns id of router
  int getID() { return this->id; }

  // adds a connection
  void addrouter(int otherid, int cost) {
    connections.push_back(otherid);
    costs.push_back(cost);
  }
};
#endif