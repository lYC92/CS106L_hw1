#include <iostream>
//#include <ctime>
#include <cmath>
#include <fstream>
#include <map>
#include <set>
#include <iomanip>
//#include "SimpleGraph.h"

#define EDGE "res/2line"
#define COORD "res/2line_coord"
using namespace std;

const double PI = 3.14159265358979323;
const double kRepel = 1e-3;
const double kAttract = 1e-3;

struct node_info {
    int nodeNo;
    int step;
    map<int,pair<double, double>> coord;                // coordinate of node
    set<pair<int,int>> edge;                            // the edge is between pt0 & pt1
    map<pair<int,int>, pair<double, double>> repel;     // atom 1 x_force *  y_force  *
    map<pair<int,int>, pair<double, double>> attract;
};


void Initial(node_info &node) {
//    read edge file
    ifstream edgeFile(EDGE);
    if (!edgeFile.is_open()) {
        cout << "Edge File not opened" << endl;
        return;
    }
    edgeFile >> node.nodeNo;
    double x,y;
    while(edgeFile >> x >> y) {
        node.edge.insert(make_pair(x,y));
    }
//    for(set<pair<int,int>>::iterator itr = node.edge.begin();
//        itr != node.edge.end(); itr ++) {
//        cout << "The edge file contain: " << endl;
//        cout << node.nodeNo << endl;
//        cout << itr->first << ", " << itr->second << endl << endl;
//    }

//    read coordinate file
    ifstream coordFile(COORD);
    if (!coordFile.is_open()) {
        cout << "Coordinate File not opened" << endl;
        return;
    }
    for(int j = 0; j < node.nodeNo; ++j) {
        coordFile >> x >> y;
        node.coord[j] = make_pair(x,y);
    }
//    for(map<int,pair<double,double>>::iterator itr = node.coord.begin();
//        itr != node.coord.end(); itr++) {
//        cout << "The coordinates are: " << endl;
//        cout << itr->second.first << ", " << itr->second.second << endl;
//    }

}

void GetRepel(node_info &node) {
    for (int i = 0; i < node.nodeNo; ++i) {
        for (int j = i+1; j < node.nodeNo; ++j) {
            double x0 = node.coord[i].first;
            double y0 = node.coord[i].second;
            double x1 = node.coord[j].first;
            double y1 = node.coord[j].second;
            double theta = atan2(y1 - y0, x1 - x0);

            double temp = pow((x1 - x0),2) + pow((y1 - y0),2);
            double f = kRepel / pow(temp,0.5);
            double fx = f*cos(theta);
            double fy = f*sin(theta);


            node.repel[make_pair(i,j)] = make_pair(-fx,-fy);
            node.repel[make_pair(j,i)] = make_pair(fx,fy);
        }
    }
//    for(map< pair<int,int>, pair<double, double>>::iterator itr2 = node.repel.begin();
//        itr2 != node.repel.end(); itr2 ++) {
//        cout << "The repel forces are: " << endl;
//        cout << itr2->first.first << " <= " << itr2->first.second << " : " <<
//                itr2->second.first << ", " << itr2->second.second << endl;
//    }
}

void GetAttract(node_info &node) {
    for (set<pair<int,int>>::iterator itr = node.edge.begin();
         itr != node.edge.end(); itr++) {
        int node0 = itr->first;
        int node1 = itr->second;

        double x0 = node.coord[node0].first;
        double y0 = node.coord[node0].second;
        double x1 = node.coord[node1].first;
        double y1 = node.coord[node1].second;

        double temp = pow((x0 - x1),2) + pow((y0 - y1),2);
        double f = kAttract * temp;
        double theta = atan2(y1 - y0, x1 - x0);
        double fx = f*cos(theta);
        double fy = f*sin(theta);

        node.attract[make_pair(node0,node1)] = make_pair(fx,fy);
        node.attract[make_pair(node1,node0)] = make_pair(-fx,-fy);

    }
//    for(map< pair<int,int>, pair<double, double>>::iterator itr2 = node.attract.begin();
//        itr2 != node.attract.end(); itr2 ++) {
//        cout << "The attract forces are: " << endl;
//        cout << itr2->first.first << " <= " << itr2->first.second << " : " <<
//                itr2->second.first << ", " << itr2->second.second << endl;
//    }
}

void MovePts(node_info &node) {
    for (int i = 0; i < node.nodeNo; i++) {
        double dx = 0;
        double dy = 0;
        for (map<pair<int,int>, pair<double, double>>::iterator itr = node.attract.begin();
             itr != node.attract.end(); itr++) {
            if (itr->first.first == i) {
                dx = dx + itr->second.first;
                dy = dy + itr->second.second;
            }
        }
        for (map<pair<int,int>, pair<double, double>>::iterator itr = node.repel.begin();
             itr != node.repel.end(); itr++) {
            if (itr->first.first == i) {
                dx = dx + itr->second.first;
                dy = dy + itr->second.second;
            }
        }
        node.coord[i].first = node.coord[i].first + dx;
        node.coord[i].second = node.coord[i].second + dy;
    }


//    for(map<int,pair<double,double>>::iterator itr = node.coord.begin();
//        itr != node.coord.end(); itr++) {
//        cout << "The coordinates are: " << endl;
//        cout << itr->second.first << ", " << itr->second.second << endl;
//    }
}

void OutPut(node_info &node) {
    cout << "=========At the " << node.step << "th steps==========" << endl;
    cout << "Node" << setw(20) << "Coordinate" << endl;
    for (int i = 0; i < node.nodeNo; i++) {
        cout << i << setw(20) << node.coord[i].first << ", " << node.coord[i].second << endl;
    }
    cout << endl;
}

int main() {
    node_info node;
    int step = 10;
    Initial(node);

    for (int i = 0; i < step; i++) {
        node.step = i;
        OutPut(node);
        GetRepel(node);
        GetAttract(node);
        MovePts(node);
    }



//    Draw(node);

//    time_t startTime = time(NULL);
//    time_t elapseTime = 0;
//    while(elapseTime < 10) {

//        state = Run(state);

//        elapseTime = difftime(time(NULL), startTime);
//    }

    return 0;
}

