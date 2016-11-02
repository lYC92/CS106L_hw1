#ifndef FORCE_H
#define FORCE_H


using namespace std;

const double kRepel = 1e-2;
const double kAttract = 1e-2;
const double PI = 3.14159265358979323;

struct node_info {
    int nodeNo;
    int step;
    map<int,pair<double, double>> coord;                // coordinate of node
    set<pair<int,int>> edge;                            // the edge is between pt0 & pt1
    map<pair<int,int>, pair<double, double>> repel;     // atom 1 x_force *  y_force  *
    map<pair<int,int>, pair<double, double>> attract;
};

//template <class T>
string Getline() {
    string foo;
    getline(cin,foo);
    return foo;
}

void Initial(node_info &node) {
    srand(time(NULL));
//    read edge file
    ifstream edgeFile;
    while (!edgeFile.is_open()){
        cout << "Please input file name: ";
        string file = Getline();
        edgeFile.open(file);
        if (!edgeFile.is_open()) {
            cout << "Edge File not opened" << endl;
        }
    }

    edgeFile >> node.nodeNo;
    double x,y;
    while(edgeFile >> x >> y) {
        node.edge.insert(make_pair(x,y));
    }

    for(int j = 0; j < node.nodeNo; ++j) {
        node.coord[j] = make_pair(((double) rand() / (RAND_MAX)),((double) rand() / (RAND_MAX)));
    }
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
}

void OutPut(node_info &node) {
    cout << "=========At the " << node.step << "th steps==========" << endl;
    cout << "Node" << setw(20) << "Coordinate" << endl;
    for (int i = 0; i < node.nodeNo; i++) {
        cout << i << setw(20) << node.coord[i].first << ", " << node.coord[i].second << endl;
    }
    cout << endl;
}


#endif // FORCE_H
