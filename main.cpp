#include "QuadTree.h"

using namespace std;

int main() {
    auto * q = new Quad(Point(MINX,MINY),Point(MAXX,MAXY));
    q->init();
    cout << InitResult << endl;
    auto result = q->searchNearbyNodes(Point(13223,14988),1000000);
    for (const auto &item: result)
        cout << item->data->id << endl;
}
