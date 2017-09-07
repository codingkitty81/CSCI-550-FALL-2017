/*
 * @file proj1.cpp This file takes input using to cin, to construct a graph,
 * then using DFS and lows to construct a tree. Once the tree is constructed
 * the leaves are counted and divided by 2, take the ceiling of that result
 * to get the number of edges to be added so any two vertices have two paths
 * of travel.
 *
 * @author Katherine Jouzapaitis
 * @date 07.09.2017
 */
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cmath>

class Graph {
   private:
      class City {
         public:
            int name;
            int d;
            int f;
            int l;
            char color;
            City *parent;
            std::vector<City*> adj;
            int id;
      };
      int cities;
      void DFS_Visit(City *curCity, int *ID);
   public:
      int time;
      Graph(int cities = 1);
      ~Graph() { free(cityList); }
      std::vector<City> *cityList;
      void addRoads(int from, int to);
      void addBridges(int from, int to);
      void DFS(int *ID);
      void print();
};

Graph::Graph(int cities) {
   this->cities = cities;
   cityList = new std::vector<City>(cities);
   for(int i = 0; i < cities; i++) { // looks at each vertex, so O(V)
      (*cityList)[i].name = i;
   }
}

void Graph::addRoads(int from, int to) {
   (*cityList)[from].adj.push_back(&(*cityList)[to]);
   (*cityList)[to].adj.push_back(&(*cityList)[from]);
}

void Graph::addBridges(int from, int to) {
   (*cityList)[from].adj.push_back(&(*cityList)[to]);
}

void Graph::DFS(int *ID) {
   for(int i = 0; i < cities; i++) {
      (*cityList)[i].color = 'W';
      (*cityList)[i].parent = &(*cityList)[i];
      (*cityList)[i].id = 0;
   }
   time = 0;
   for(int i = 0; i < cities; i++) { // looks at each vertex, so O(V)
      if((*cityList)[i].color == 'W') {
         *ID += 1;
         DFS_Visit(&(*cityList)[i], ID);
      }
   }
}

void Graph::DFS_Visit(City *curCity, int *ID) {
   time += 1;
   curCity->d = time;
   curCity->l = time;
   curCity->id = *ID;
   curCity->color = 'G';
   for(std::vector<City*>::iterator it = curCity->adj.begin(); it != curCity->adj.end(); ++it) { // will hit all edges twice since undirected, so O(E)
      if((*it)->color == 'W') {
         (*it)->parent = curCity;
         (*it)->id = *ID;
         DFS_Visit(*it, ID);
      }
      if((*it)->l < curCity->l && curCity->parent != (*it)) {
         curCity->l = (*it)->l;
      }
   }
   curCity->color = 'B';
   time += 1;
   curCity->f = time;
}

int main() {
   int N, M, from, to;
   int ID = -1;
   int leaves = 0;
   
   std::cin >> N >> M; // constant runtime for assignments
   
   if(N <= 0) {
      std::cerr << "ERROR: graph is empty." << std::endl;
      exit(1);
   }
   
   Graph roads(N);
   Graph gBCC(N);
   Graph gBridge(N);
   int *ids = new int[N];
   
   while(std::cin >> from >> to) { // will stop after M number of edges are read in, so O(E)
      roads.addRoads(from, to);
   }
   
   // This DFS run will calculate the low for each vertex in order to identify biconnected components
   roads.DFS(&ID);
  
   // Iterate through Graph roads, and place bridges in gBridge and other tree edges into gBCC
   for(std::vector<Graph::City>::iterator it = roads.cityList->begin()+1; it != roads.cityList->end(); ++it) { // looks at each vertex in graph roads, so O(V)
      if((*it).d == (*it).l && (*it).parent->name != (*it).name) {
         gBridge.addRoads((*it).parent->name, (*it).name);
      } else {
         gBCC.addRoads((*it).parent->name, (*it).name);
      }
   }
   
   ID = -1;
   
   // This run through DFS will assign the IDs to each vertex
   gBCC.DFS(&ID);

   Graph collapsed(ID + 1);
   
   for(std::vector<Graph::City>::iterator it = gBCC.cityList->begin(); it != gBCC.cityList->end(); ++it) { // looks at each vertex in gBCC, so O(V)
      ids[(*it).name] = (*it).id;
   }
   
   for(std::vector<Graph::City>::iterator it = gBridge.cityList->begin(); it != gBridge.cityList->end(); ++it) { // looks at each vertex in gBridge, O(V)
      if((*it).adj.size() > 0) { // this condition causes the adjacency list to be looked at once, so O(E)
         for(std::vector<Graph::City*>::iterator iter =(*it).adj.begin(); iter != (*it).adj.end(); ++iter) {
            collapsed.addBridges(ids[(*it).name], ids[(*iter)->name]);
         }
      }
   }

   for(std::vector<Graph::City>::iterator it = collapsed.cityList->begin(); it != collapsed.cityList->end(); ++it) { // looks at each vertex in collapsed, so O(V)
      if((*it).adj.size() == 1) {
         leaves++;
      }
   }
   
   std::cout << ceil(leaves/2.0) << std::endl;
   
   free(ids);
   
   return(0);
}