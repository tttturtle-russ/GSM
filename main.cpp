#include "QuadTree.h"

using namespace std;

int main() {
    auto q = new QuadTree(XMIN, YMIN, XMAX - XMIN, YMAX - YMIN);
    cout << q->init() << endl;
//    auto r1 = q->findFirstNorthWestPoints();
//    auto r2 = q->findFirstSouthEastPoints();
//    cout << "�����ǵ�һ������ڵ���" << r1.size() << "��" << endl;
//    for (const auto &item: r1)
//        cout << "x : " << item->x << "    y : "<< item->y << "    id : "<< item->value->id << endl;
//    cout << "���Ͻǵ�һ������ڵ���" << r2.size() << "��" << endl;
//    for (const auto &item: r2)
//        cout << "x : " << item->x << "    y : "<< item->y << "    id : "<< item->value->id << endl;
//    auto r3 = q->findNorthWestPointsOnEast();
//    cout << "�����ǵ�һ������Ķ���ڵ���" << r3.size() << "��" << endl;
//    for (const auto &item: r3)
//        cout << "x : " << item->x << "    y : "<< item->y << "    id : "<< item->value->id << endl;
//    auto r4 = q->findNorthWestPointsOnSouth();
//    cout << "�����ǵ�һ��������ϲ�ڵ���" << r4.size() << "��" << endl;
//    for (const auto &item: r4)
//        cout << "x : " << item->x << "    y : "<< item->y << "    id : "<< item->value->id << endl;
//    auto r5 = q->findSouthEastPointsOnNorthWest();
//    cout << "���Ͻǵ�һ�������������ڵ���" << r5.size() << "��" << endl;
//    for (const auto &item: r5)
//        cout << "x : " << item->x << "    y : "<< item->y << "    id : "<< item->value->id << endl;
//    auto r6 = q->findSouthEastPointsOnNorthWestOnNorth();
//    cout << "���Ͻǵ�һ�������������ı���ڵ���" << r6.size() << "��" << endl;
//    for (const auto &item: r6)
//        cout << "x : " << item->x << "    y : "<< item->y << "    id : "<< item->value->id << endl;
    //q->showResult();
    q->advancedTask1();

}
