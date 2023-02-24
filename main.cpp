#include "QuadTree.h"

using namespace std;

int main() {
    auto * q = new Quad(Point(MINX,MINY),Point(MAXX,MAXY));
    q->init();
    cout << InitResult << endl;
    vector<Node*> result;
    q->searchNearbyNodes(Point(2986,15024),1000,result);
    for (const auto &item: result)
        cout << item->data->id << endl;
}
