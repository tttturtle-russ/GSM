#include "QuadTree.h"
#include "util.h"
using namespace std;

int main() {
    initUtil();
    auto q = new QuadTree(XMIN, YMIN, XMAX - XMIN, YMAX - YMIN);
    cout << q->init() << endl;
    cout << initUtil() << endl;
    int choice;
    // ���û�����
    while(true){
        showChoice();
        cin >> choice;
        system("cls");
        switch (choice) {
            case 0:
                exit(0);
            case 1:{
                    auto r1 = q->findFirstNorthWestPoints();
                    cout << "�����ǵ�һ������ڵ���" << r1.size() << "��" << endl;
                    for (const auto &item: r1)
                        cout << "x : " << item->x << "    y : "<< item->y << "    id : "<< item->value->id << endl;
                system("pause");
                system("cls");
            }
                break;
            case 2:{
                    auto r2 = q->findFirstSouthEastPoints();
                    cout << "���Ͻǵ�һ������ڵ���" << r2.size() << "��" << endl;
                    for (const auto &item: r2)
                        cout << "x : " << item->x << "    y : "<< item->y << "    id : "<< item->value->id << endl;
                system("pause");
                system("cls");
            }
                break;
            case 3:{
                    auto r3 = q->findNorthWestPointsOnEast();
                    cout << "�����ǵ�һ������Ķ���ڵ���" << r3.size() << "��" << endl;
                    for (const auto &item: r3)
                        cout << "x : " << item->x << "    y : "<< item->y << "    id : "<< item->value->id << endl;
                system("pause");
                system("cls");
            }
                break;
            case 4:{
                    auto r4 = q->findNorthWestPointsOnSouth();
                    cout << "�����ǵ�һ��������ϲ�ڵ���" << r4.size() << "��" << endl;
                    for (const auto &item: r4)
                        cout << "x : " << item->x << "    y : "<< item->y << "    id : "<< item->value->id << endl;
                system("pause");
                system("cls");
            }
                break;
            case 5:{
                    auto r5 = q->findSouthEastPointsOnNorthWest();
                    cout << "���Ͻǵ�һ�������������ڵ���" << r5.size() << "��" << endl;
                    for (const auto &item: r5)
                        cout << "x : " << item->x << "    y : "<< item->y << "    id : "<< item->value->id << endl;
                system("pause");
                system("cls");
            }
                break;
            case 6:{
                    auto r6 = q->findSouthEastPointsOnNorthWestOnNorth();
                    cout << "���Ͻǵ�һ�������������ı���ڵ���" << r6.size() << "��" << endl;
                    for (const auto &item: r6)
                        cout << "x : " << item->x << "    y : "<< item->y << "    id : "<< item->value->id << endl;
                system("pause");
                system("cls");
            }
                break;
            case 7:{
                auto p = new Point();
                for (int i = 0; i < 3; ++i) {
                    cout << "���������:" << endl;
                    cin >> p->x >> p->y;
                    cout << "��ǿ��վ" << q->findMostPowerfulPoint(*p) << endl;
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
                cout << "��Ч��ѡ��" << endl;
                system("pause");
                system("cls");
            }
            system("pause");
        }
    }
}
