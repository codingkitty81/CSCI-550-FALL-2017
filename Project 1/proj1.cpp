
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
      std::vector<City> *cityList;
      void addRoads(int from, int to);
      void addBridges(int from, int to);
      void DFS(int *ID);
      void print();
};


void Graph::print() {
   for(int i = 0; i < cities; i++) {
      std::cout << i 
         << ", n:" << (*cityList)[i].name
         << ", d:" << (*cityList)[i].d
         << ", f:" << (*cityList)[i].f
         << ", l:" << (*cityList)[i].l
         << ", c:" << (*cityList)[i].color
         << ", id:" << (*cityList)[i].id
         << ", adj:";
      for(std::vector<City*>::iterator it = (*cityList)[i].adj.begin(); it != (*cityList)[i].adj.end(); ++it) {
         std::cout << (*it)->name << ", ";
      }
      std::cout << std::endl;
   }
}

Graph::Graph(int cities) {
   this->cities = cities;
   cityList = new std::vector<City>(cities);
   for(int i = 0; i < cities; i++) {
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
   for(int i = 0; i < cities; i++) {
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
   for(std::vector<City*>::iterator it = curCity->adj.begin(); it != curCity->adj.end(); ++it) {
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
   
   std::cin >> N >> M;
   
   if(N <= 0) {
      std::cout << "ERROR: graph is empty." << std::endl;
      exit(1);
   }
   
   Graph roads(N);
   Graph gBCC(N);
   Graph gBridge(N);
   int *ids = new int[N];
   
   while(std::cin >> from >> to) {
      roads.addRoads(from, to);
   }
   
   roads.DFS(&ID);
   
   for(std::vector<Graph::City>::iterator it = roads.cityList->begin(); it != roads.cityList->end(); ++it) {
      if((*it).d == (*it).l && (*it).parent->name != (*it).name) {
         gBridge.addRoads((*it).parent->name, (*it).name);
      } else {
         gBCC.addRoads((*it).parent->name, (*it).name);
      }
   }
   
   ID = -1;
   
   gBCC.DFS(&ID);

   Graph collapsed(ID + 1);
   
   for(std::vector<Graph::City>::iterator it = gBCC.cityList->begin(); it != gBCC.cityList->end(); ++it) {
      ids[(*it).name] = (*it).id;
   }
   
   for(std::vector<Graph::City>::iterator it = gBridge.cityList->begin(); it != gBridge.cityList->end(); ++it) {
      if((*it).adj.size() > 0) {
         for(std::vector<Graph::City*>::iterator iter =(*it).adj.begin(); iter != (*it).adj.end(); ++iter) {
            collapsed.addBridges(ids[(*it).name], ids[(*iter)->name]);
         }
      }
   }
   
   for(std::vector<Graph::City>::iterator it = collapsed.cityList->begin(); it != collapsed.cityList->end(); ++it) {
      if((*it).adj.size() == 1) {
         leaves++;
      }
   }
   
   std::cout << ceil(leaves/2.0) << std::endl;
   
   return(0);
}