#include "QuadTree.h"

using namespace std;

int main() {
    auto * q = new Quad(Point(MINX,MINY),Point(MAXX,MAXY));
    q->init();
    cout << InitResult << endl;
    q->showBotRight();
    q->showTopLeft();
    q->showEast();
    q->showSouth();
}
