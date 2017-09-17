
#include <cstdlib>
#include <iostream>
#include <vector>

class Corporation { // graph
    public:
        class Station;
        class WormHole { // edges
            public:
                WormHole(Station* dest, int weight, int cost);
                Station* dest;
                int weight;
        };
        class Station { // vertices
            public:
                Station();
                ~Station();
                int name;
                int id;
                char color;
                int weightFromParent;
                Station *parent;
                std::vector<WormHole> *wormList;
                void print() { std::cout << "corp" << " " << name << std::endl; }
        };
        int stations;
        Corporation(int stations);
        ~Corporation();
        std::vector<Station> *stationList;
        void addWormHole(int src, int dest, int weight, int cost = 0);
        void DFS(int *removeEdge);
        void DFS_Visit(Station *curStation, int *removeEdge);
        void runBackTrace(Station *curStation, Station *src, int *removeEdge);
};

Corporation::Corporation(int stations) {
    this->stations = stations;
    stationList = new std::vector<Station>(stations);
}

Corporation::~Corporation() {
    stationList->clear();
    delete stationList;
}

Corporation::WormHole::WormHole(Station* dest, int weight, int cost) {
    this->dest = dest;
    this->weight = weight;
}

Corporation::Station::Station() {
    wormList = new std::vector<WormHole>;
}

Corporation::Station::~Station() {
    wormList->clear();
    delete wormList;
}

void Corporation::addWormHole(int src, int dest, int weight, int cost) {
    Station *s = &(*stationList)[dest];
    WormHole *w = new WormHole(s, weight, cost);
    (*stationList)[src].wormList->push_back(*w);
}

void Corporation::DFS(int *removeEdge) { // used to identify the cycle, so doesn't need all aspects
   for(int i = 0; i < stations; i++) {
      (*stationList)[i].color = 'W';
      (*stationList)[i].parent = &(*stationList)[i];
   }
   for(int i = 0; i < stations; i++) {
      if((*stationList)[i].color == 'W') {
         DFS_Visit(&(*stationList)[i], removeEdge);
      }
   }
}

void Corporation::DFS_Visit(Station *curStation, int *removeEdge) {
   curStation->color = 'G';
   for(std::vector<WormHole>::iterator it = curStation->wormList->begin(); it != curStation->wormList->end(); ++it) {
      if((*it).dest == curStation->parent) continue;
      if((*it).dest->color == 'W') {
         (*it).dest->parent = curStation;
         (*it).dest->weightFromParent = (*it).weight;
         DFS_Visit((*it).dest, removeEdge);
      } else if((*it).dest->color == 'G') {
          Station *src = (*it).dest;
          removeEdge[0] = curStation->name;
          removeEdge[1] = (*it).dest->name;
          removeEdge[2] = (*it).weight;
          removeEdge[3] = curStation->id;
          removeEdge[4] = (*it).dest->id;
          
          runBackTrace(curStation, src, removeEdge);
      }
   }
   curStation->color = 'B';
}

void Corporation::runBackTrace(Station *curStation, Station *src, int *removeEdge) {
    if(curStation != src) {
        if(removeEdge[2] < curStation->weightFromParent) {
            removeEdge[0] = curStation->parent->name;
            removeEdge[1] = curStation->name;
            removeEdge[2] = curStation->weightFromParent;
            removeEdge[3] = curStation->parent->id;
            removeEdge[4] = curStation->id;
        }
        runBackTrace(curStation->parent, src, removeEdge);
    }
}

int main() {
    int RN, ST, from, from2, to, to2, wgt, cost1, cost2;
    int totWeight1 = 0;
    int totWeight2 = 0;
    int totWeight3 = 0;
    
    std::cin >> RN;

    int *rFrom = new int[RN];
    int *rTo = new int[RN];
    int *rWgt = new int[RN];
    
    for(int i = 0; i < RN - 1; i++) {
        std::cin >> from >> to >> wgt;
        rFrom[i] = from;
        rTo[i] = to;
        rWgt[i] = wgt;
        totWeight1 += wgt * 10;
        totWeight2 += wgt * 10;
        totWeight3 += wgt * 10;
    }
    
    std::cin >> ST;
    
    Corporation redNova(RN + ST); // make Red Nova
    
    for(int i = 0; i < RN - 1; i++) { // add the worm holes
        redNova.addWormHole(rFrom[i], rTo[i], rWgt[i]);
        redNova.addWormHole(rTo[i], rFrom[i], rWgt[i]);
        (*redNova.stationList)[rFrom[i]].name = rFrom[i];
        (*redNova.stationList)[rTo[i]].name = rTo[i];
        (*redNova.stationList)[rFrom[i]].id = 1;
        (*redNova.stationList)[rTo[i]].id = 1;
    }
    
    for(int i = 0; i < ST - 1; i++) { // add the worm holes
        std::cin >> from >> to >> wgt;
        redNova.addWormHole(from + RN, to + RN, wgt);
        redNova.addWormHole(to + RN, from + RN, wgt);
        totWeight1 += wgt * 10;
        totWeight2 += wgt * 10;
        totWeight3 += wgt * 10;
        (*redNova.stationList)[from+RN].name = from;
        (*redNova.stationList)[to+RN].name = to;
        (*redNova.stationList)[from+RN].id = 2;
        (*redNova.stationList)[to+RN].id = 2;
    }

    std::cin >> from >> to >> wgt >> cost1;
    redNova.addWormHole(from, to + RN, wgt);
    redNova.addWormHole(to + RN, from, wgt);
    totWeight1 += cost1 + wgt * 10;
    totWeight3 += cost1 + wgt * 10;
    
    std::cin >> from2 >> to2 >> wgt >> cost2;
    redNova.addWormHole(from2, to2 + RN, wgt);
    redNova.addWormHole(to2 + RN, from2, wgt);
    totWeight2 += cost2 + wgt * 10;
    totWeight3 += cost2 + wgt * 10;

    int *removeEdge = new int[5];
    // index 0 = from name
    // index 1 = to name
    // index 2 = weight of edge
    // index 3 = from rank
    // index 4 = to rank

    redNova.DFS(removeEdge);
    
    /*
    for(std::vector<Corporation::Station>::iterator it = redNova.stationList->begin(); it != redNova.stationList->end(); ++it) {
        std::cout << (*it).name << " ";
        for(unsigned int i = 0; i < (*it).wormList->size(); i++) {
            std::cout << (*(*it).wormList)[i].dest->name << " ";
        }
        std::cout << std::endl;
    }
    
    for(int i = 0; i < 5; i++) {
        std::cout << removeEdge[i] << ", ";
    }
    std::cout << std::endl;
    
    std::cout << totWeight1 << " " << totWeight2 << std::endl;
    */
    
    totWeight3 -= removeEdge[2] * 10;
    
    if(totWeight3 < totWeight1 && totWeight3 < totWeight2) {
        if(removeEdge[0] < removeEdge[1]) {
            std::cout << removeEdge[0] << " " << removeEdge[1] << " " << removeEdge[3] << " " << totWeight3 << std::endl;
        } else {
            std::cout << removeEdge[1] << " " << removeEdge[0] << " " << removeEdge[3] << " " << totWeight3 << std::endl;
        }
    } else {
        if(totWeight1 < totWeight2) {
            std::cout << from << " " << to << " " << totWeight1 << std::endl; 
        } else {
            std::cout << from2 << " " << to2 << " " << totWeight2 << std::endl;
        }
    }
    
    delete [] rFrom;
    delete [] rTo;
    delete [] rWgt;
    delete [] removeEdge;
    
    return 0;
}