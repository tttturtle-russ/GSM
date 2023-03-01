#include "QuadTree.h"

using namespace std;

int main() {
    auto q = new QuadTree(XMIN, YMIN, XMAX - XMIN, YMAX - YMIN);
    cout << q->init() << endl;
//    auto r1 = q->findFirstNorthWestPoints();
//    auto r2 = q->findFirstSouthEastPoints();
//    cout << "西北角第一个区域节点有" << r1.size() << "个" << endl;
//    for (const auto &item: r1)
//        cout << "x : " << item->x << "    y : "<< item->y << "    id : "<< item->value->id << endl;
//    cout << "东南角第一个区域节点有" << r2.size() << "个" << endl;
//    for (const auto &item: r2)
//        cout << "x : " << item->x << "    y : "<< item->y << "    id : "<< item->value->id << endl;
//    auto r3 = q->findNorthWestPointsOnEast();
//    cout << "西北角第一个区域的东侧节点有" << r3.size() << "个" << endl;
//    for (const auto &item: r3)
//        cout << "x : " << item->x << "    y : "<< item->y << "    id : "<< item->value->id << endl;
//    auto r4 = q->findNorthWestPointsOnSouth();
//    cout << "西北角第一个区域的南侧节点有" << r4.size() << "个" << endl;
//    for (const auto &item: r4)
//        cout << "x : " << item->x << "    y : "<< item->y << "    id : "<< item->value->id << endl;
//    auto r5 = q->findSouthEastPointsOnNorthWest();
//    cout << "东南角第一个区域的西北侧节点有" << r5.size() << "个" << endl;
//    for (const auto &item: r5)
//        cout << "x : " << item->x << "    y : "<< item->y << "    id : "<< item->value->id << endl;
//    auto r6 = q->findSouthEastPointsOnNorthWestOnNorth();
//    cout << "东南角第一个区域的西北侧的北侧节点有" << r6.size() << "个" << endl;
//    for (const auto &item: r6)
//        cout << "x : " << item->x << "    y : "<< item->y << "    id : "<< item->value->id << endl;
    //q->showResult();
    q->advancedTask1();

}
