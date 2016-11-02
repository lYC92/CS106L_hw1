#include <iostream>
#include <ctime>
#include <cmath>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <iomanip>
#include <time.h>
#include "force.h"
#include "SimpleGraph.h"


int main() {
    SimpleGraph graph;
    node_info node;
    int totalTime;

    cout << "Please input running time: ";
    cin >> totalTime;

    string dummy;
    getline(cin,dummy);

    Initial(node);

    Edge edgex;
    Node nodex;

//    initial edge
    for (set<pair<int,int>>::iterator itr = node.edge.begin();
         itr != node.edge.end(); itr++) {
        edgex.start = itr->first;
        edgex.end = itr->second;
        graph.edges.push_back(edgex);
    }

//    initial coord
    for (map<int,pair<double, double>>::iterator itr = node.coord.begin();
         itr != node.coord.end(); itr++) {
        nodex.x = itr->second.first;
        nodex.y = itr->second.second;
        graph.nodes.push_back(nodex);
    }

    time_t startT = time(NULL);
    time_t elapseT = 0;
    InitGraphVisualizer(graph);



    int step = 0;
    while (elapseT < totalTime) {

        GetRepel(node);
        GetAttract(node);
        MovePts(node);

        int i = 0;
        for (map<int,pair<double, double>>::iterator itr = node.coord.begin();
             itr != node.coord.end(); itr++) {
            nodex.x = itr->second.first;
            nodex.y = itr->second.second;
            graph.nodes[i] = nodex;
            i++;
        }
//        InitGraphVisualizer(graph);
        DrawGraph(graph);
        elapseT = difftime(time(NULL),startT);
        cout << elapseT << endl;

        step++;
    }
    return 0;
}
