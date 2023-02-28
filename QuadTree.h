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
using namespace std;

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
    mobile_station(int _xs,int _ys,int _xe,int _ye,int _speed,int _start_hour,int _start_min){
        xs = _xs;
        ys = _ys;
        xe = _xe;
        ye = _ye;
        speed = _speed;
        start_hour = _start_hour;
        start_min = _start_min;
    }

    double distance() {
        return sqrt(pow((xe - xs),2) + pow((ye - ys),2));
    }
    double getSin();
    double getCos();
}mobile;

vector<mobile_station *> getMobileInfo();

struct Point {
    double x,y;
    base* value;
    Point(double _x,double _y) : x(_x),y(_y){}
    Point(double _x,double _y,base* v) : x(_x),y(_y),value(v){}
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
    double calculateEquivalentIntensity(const Point p) {
        double result = 0;
        if (p.value->type == "����"){
            double dis = this->distance(p);
            result = p.value->power * pow((300.0/dis),2);
        }else if (p.value->type == "����"){
            double dis = this->distance(p);
            result = p.value->power * pow((1000.0/dis),2);
        }else if (p.value->type == "����") {
            double dis = this->distance(p);
            result = p.value->power * pow((5000.0/dis),2);
        }
        return result;
    };

};

class QuadTree {
    const int POINT_CAPACITY = 10;
    float x, y; // ��ǰ�ڵ�����ʾ�ľ�����������½�����
    float w, h; // ��ǰ�ڵ�����ʾ�ľ�������Ŀ�Ⱥ͸߶�
    vector<Point*> value; // ��ǰ�ڵ�洢��ֵ
    QuadTree* nw; // �����ӽڵ�
    QuadTree* ne; // �����ӽڵ�
    QuadTree* sw; // �����ӽڵ�
    QuadTree* se; // �����ӽڵ�

    bool isLeaf() const {
        return nw == nullptr && ne == nullptr && sw == nullptr && se == nullptr;
    }

    void subdivide() {
        nw = new QuadTree(x, y + h / 2, w / 2, h / 2);
        ne = new QuadTree(x + w / 2, y + h / 2, w / 2, h / 2);
        sw = new QuadTree(x, y, w / 2, h / 2);
        se = new QuadTree(x + w / 2, y, w / 2, h / 2);
    }

    void checkMove(mobile m) const;
public:
    QuadTree(int x, int y, int w, int h) : x(x), y(y), w(w), h(h), value(0), nw(nullptr), ne(nullptr), sw(nullptr), se(nullptr){}

    ~QuadTree() {
        delete nw;
        delete ne;
        delete sw;
        delete se;
    }

    void insert(Point*p) ;

    [[nodiscard]] Point* query(const Point& p) const;

    string init();

    bool intersects(double rx, double ry, double rw, double rh) const;

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

    vector<Point *> searchNearbyPoints(const Point& p,double r = 10000) const;

    Point* findNearestPoint(const Point& p) const;

    Point *findMostPowerfulPoint(Point& p) const;

    void showResult() const;
};

#endif //GSM_QUADTREE_H
