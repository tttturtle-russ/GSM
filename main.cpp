#include "QuadTree.h"
#include "util.h"
using namespace std;

int main() {
    initUtil();
    auto q = new QuadTree(XMIN, YMIN, XMAX - XMIN, YMAX - YMIN);
    cout << q->init() << endl;
    cout << initUtil() << endl;
    int choice;
    // 与用户交互
    while(true){
        showChoice();
        cin >> choice;
        system("cls");
        switch (choice) {
            case 0:
                exit(0);
            case 1:{
                    auto r1 = q->findFirstNorthWestPoints();
                    cout << "西北角第一个区域节点有" << r1.size() << "个" << endl;
                    for (const auto &item: r1)
                        cout << "x : " << item->x << "    y : "<< item->y << "    id : "<< item->value->id << endl;
                system("pause");
                system("cls");
            }
                break;
            case 2:{
                    auto r2 = q->findFirstSouthEastPoints();
                    cout << "东南角第一个区域节点有" << r2.size() << "个" << endl;
                    for (const auto &item: r2)
                        cout << "x : " << item->x << "    y : "<< item->y << "    id : "<< item->value->id << endl;
                system("pause");
                system("cls");
            }
                break;
            case 3:{
                    auto r3 = q->findNorthWestPointsOnEast();
                    cout << "西北角第一个区域的东侧节点有" << r3.size() << "个" << endl;
                    for (const auto &item: r3)
                        cout << "x : " << item->x << "    y : "<< item->y << "    id : "<< item->value->id << endl;
                system("pause");
                system("cls");
            }
                break;
            case 4:{
                    auto r4 = q->findNorthWestPointsOnSouth();
                    cout << "西北角第一个区域的南侧节点有" << r4.size() << "个" << endl;
                    for (const auto &item: r4)
                        cout << "x : " << item->x << "    y : "<< item->y << "    id : "<< item->value->id << endl;
                system("pause");
                system("cls");
            }
                break;
            case 5:{
                    auto r5 = q->findSouthEastPointsOnNorthWest();
                    cout << "东南角第一个区域的西北侧节点有" << r5.size() << "个" << endl;
                    for (const auto &item: r5)
                        cout << "x : " << item->x << "    y : "<< item->y << "    id : "<< item->value->id << endl;
                system("pause");
                system("cls");
            }
                break;
            case 6:{
                    auto r6 = q->findSouthEastPointsOnNorthWestOnNorth();
                    cout << "东南角第一个区域的西北侧的北侧节点有" << r6.size() << "个" << endl;
                    for (const auto &item: r6)
                        cout << "x : " << item->x << "    y : "<< item->y << "    id : "<< item->value->id << endl;
                system("pause");
                system("cls");
            }
                break;
            case 7:{
                auto p = new Point();
                for (int i = 0; i < 3; ++i) {
                    cout << "输入点坐标:" << endl;
                    cin >> p->x >> p->y;
                    cout << "最强基站" << q->findMostPowerfulPoint(*p) << endl;
                }
                system("pause");
                system("cls");
            }
                break;
            case 8:{
                q->showResult();
                system("pause");
                system("cls");
            }
                break;
            case 9:{
                q->extendTask2();
                system("pause");
                system("cls");
            }
                break;
            default:{
                cout << "无效的选项" << endl;
                system("pause");
                system("cls");
            }
            system("pause");
        }
    }
}
