//
// Created by liuhy on 2023/2/24.
//

#ifndef GSM_QUADTREE_H
#define GSM_QUADTREE_H

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <memory>
using namespace std;

// �����α߽�
const double XMIN = -26;
const double XMAX = 109996;
const double YMIN = -82;
const double YMAX = 39946;

typedef struct base_station{
    int id;
    float power;
    string type;
    base_station(int _id,float _power,string _type){
        id = _id;
        power = _power;
        type = _type;
    }
}base;

typedef struct mobile_station{
    int xs,ys,xe,ye;
    int speed;
    int start_hour,start_min;
    int end_hour,end_min;
    mobile_station(int _xs,int _ys,int _xe,int _ye,int _speed,int _start_hour,int _start_min){
        xs = _xs;
        ys = _ys;
        xe = _xe;
        ye = _ye;
        speed = _speed;
        start_hour = _start_hour;
        start_min = _start_min;
        int duration = sqrt(pow((xe - xs),2) + pow((ye - ys),2)) / (double(speed) * 50.0 / 3.0);
        end_hour = start_hour + (start_min + int(duration)) / 60;
        end_min = (start_min + int(duration)) % 60;
    }

    double distance() {
        return sqrt(pow((xe - xs),2) + pow((ye - ys),2));
    }
    double distance(int x,int y) {
        return sqrt(pow((x - xs),2) + pow((y - ys),2));
    }
    double getSin();
    double getCos();
}mobile;

struct Point {
    int id;
    double x,y;
    base* value;
    Point() : x(0),y(0),value(nullptr),id(0){}
    Point(double _x,double _y) : x(_x),y(_y),value(nullptr),id(0){}
    Point(double _x,double _y,base* v) : x(_x),y(_y),value(v),id(0){}
    // ����==
    bool operator == (const Point& p) const{
        return this->x == p.x && this->y == p.y;
    }
    // ����<<
    friend ostream& operator << (ostream& os, const Point* p){
        os << p->x << "," << p->y << "," << p->value->id << "," << p->value->power << "," << p->value->type;
        return os;
    }
    double distance(const Point &p);
    Point *getMinDistancePoint(const vector<Point *> &p) const;
    Point *getMaxPowerPoint(const vector<Point *>& vector1) ;
    Point *getOnlyPoint(const vector<Point *>& p);
    // �����Чǿ��
    double calculateEquivalentIntensity(const Point p);
    bool isValid(Point p);
    void enterIterateCalculate(Point* cur,Point* pre,double cos,double sin,int step = 10,double precision = 0.000001);
    void outIterateCalculate(Point *cur,Point* pre, double cos, double sin, int step = 10, double precision= 0.000001);

    void outIterateCalculate(Point* pre,double cos,double sin,int step = 10,double precision = 0.1);
};

typedef struct fake_station{
    int xs,ys,xe,ye;
    int speed;
    int id;
    int start_hour,start_min;
    int end_hour,end_min;
    double duration;
    fake_station(int _xs,int _ys,int _xe,int _ye,int _speed,int _id,int _start_hour,int _start_min){
        xs = _xs;
        ys = _ys;
        xe = _xe;
        ye = _ye;
        speed = _speed;
        id = _id;
        start_hour = _start_hour;
        start_min = _start_min;
        duration = sqrt(pow((xe - xs),2) + pow((ye - ys),2)) / (double(speed) * 50.0 / 3.0);
        end_hour = start_hour + (start_min + int(duration)) / 60;
        end_min = (start_min + int(duration)) % 60;
    }
    double distance();

    double getCos();

    double getSin();

    Point *getPosition(double time, int curHour, int curMin);
}fake;

class QuadTree {
    const int POINT_CAPACITY = 10;
    double x, y; // ��ǰ�ڵ�����ʾ�ľ�����������½�����
    double w, h; // ��ǰ�ڵ�����ʾ�ľ�������Ŀ�Ⱥ͸߶�
    vector<Point*> value; // ��ǰ�ڵ�洢��ֵ
    QuadTree* nw; // �����ӽڵ�
    QuadTree* ne; // �����ӽڵ�
    QuadTree* sw; // �����ӽڵ�
    QuadTree* se; // �����ӽڵ�

    bool isLeaf() const {
        return nw == nullptr && ne == nullptr && sw == nullptr && se == nullptr;
    }
    // �ӽڵ��ķ�
    void subdivide() {
        nw = new QuadTree(x, y + h / 2, w / 2, h / 2);
        ne = new QuadTree(x + w / 2, y + h / 2, w / 2, h / 2);
        sw = new QuadTree(x, y, w / 2, h / 2);
        se = new QuadTree(x + w / 2, y, w / 2, h / 2);
    }
    // ����ƶ��켣
    void checkMove(mobile m) const;
    // ����ص�����
    void checkOverlap(mobile m) const;
    // ���Ҹ����㸽���ĵ�
    [[nodiscard]] vector<Point *> searchNearbyPoints(const Point& p,double r = 10000) const;
public:
    QuadTree(double x, double y, double w, double h) : x(x), y(y), w(w), h(h), value(0), nw(nullptr), ne(nullptr), sw(nullptr), se(nullptr){}

    ~QuadTree() {
        delete nw;
        delete ne;
        delete sw;
        delete se;
    }
    // ����һ����
    void insert(Point*p) ;
    // ��ѯһ����
    [[nodiscard]] Point* query(const Point& p) const;
    // ��ʼ������������������
    string init();

    // ��ѯ��һ���нڵ������������ (task 1)
    [[nodiscard]] vector<Point *> findFirstNorthWestPoints() const;
    // ��ѯ��һ���нڵ�Ķ��Ͻ����� (task 1)
    [[nodiscard]] vector<Point *> findFirstSouthEastPoints() const;
    // ��ѯ��һ���нڵ������������Ķ������� (task 2)
    [[nodiscard]] vector<Point *> findNorthWestPointsOnEast() const;
    // ��ѯ��һ���нڵ��������������ϱ����� (task 2)
    [[nodiscard]] vector<Point *> findNorthWestPointsOnSouth() const;
    // ��ѯ��һ���нڵ�Ķ��Ͻ���������������� (task 2)
    [[nodiscard]] vector<Point *> findSouthEastPointsOnNorthWest() const;
    // ��ѯ��һ���нڵ�Ķ��Ͻ����������������ı������� (task 2)
    [[nodiscard]] vector<Point *> findSouthEastPointsOnNorthWestOnNorth() const;
    // ��ѯ����ĵ�
    Point* findNearestPoint(const Point& p) const;
    // ��ѯ��ǿ�ĵ�
    Point *findMostPowerfulPoint(Point& p) const;
    // ��ʾ·���ϵĻ�վ���������task 5,extendTask 1��
    void showResult() const;
    // ��ʾ�����κ͵����ε��ص�������� (extendTask 2)
    void extendTask2() const;
};

#endif //GSM_QUADTREE_H
