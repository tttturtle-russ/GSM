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

// 最大矩形边界
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
    // 重载==
    bool operator == (const Point& p) const{
        return this->x == p.x && this->y == p.y;
    }
    // 重载<<
    friend ostream& operator << (ostream& os, const Point* p){
        os << p->x << "," << p->y << "," << p->value->id << "," << p->value->power << "," << p->value->type;
        return os;
    }
    double distance(const Point &p);
    Point *getMinDistancePoint(const vector<Point *> &p) const;
    Point *getMaxPowerPoint(const vector<Point *>& vector1) ;
    Point *getOnlyPoint(const vector<Point *>& p);
    // 计算等效强度
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
    double x, y; // 当前节点所表示的矩形区域的左下角坐标
    double w, h; // 当前节点所表示的矩形区域的宽度和高度
    vector<Point*> value; // 当前节点存储的值
    QuadTree* nw; // 左上子节点
    QuadTree* ne; // 右上子节点
    QuadTree* sw; // 左下子节点
    QuadTree* se; // 右下子节点

    bool isLeaf() const {
        return nw == nullptr && ne == nullptr && sw == nullptr && se == nullptr;
    }
    // 子节点四分
    void subdivide() {
        nw = new QuadTree(x, y + h / 2, w / 2, h / 2);
        ne = new QuadTree(x + w / 2, y + h / 2, w / 2, h / 2);
        sw = new QuadTree(x, y, w / 2, h / 2);
        se = new QuadTree(x + w / 2, y, w / 2, h / 2);
    }
    // 检查移动轨迹
    void checkMove(mobile m) const;
    // 检查重叠区域
    void checkOverlap(mobile m) const;
    // 查找给定点附近的点
    [[nodiscard]] vector<Point *> searchNearbyPoints(const Point& p,double r = 10000) const;
public:
    QuadTree(double x, double y, double w, double h) : x(x), y(y), w(w), h(h), value(0), nw(nullptr), ne(nullptr), sw(nullptr), se(nullptr){}

    ~QuadTree() {
        delete nw;
        delete ne;
        delete sw;
        delete se;
    }
    // 插入一个点
    void insert(Point*p) ;
    // 查询一个点
    [[nodiscard]] Point* query(const Point& p) const;
    // 初始化，建树，读入数据
    string init();

    // 查询第一个有节点的西北角区域 (task 1)
    [[nodiscard]] vector<Point *> findFirstNorthWestPoints() const;
    // 查询第一个有节点的东南角区域 (task 1)
    [[nodiscard]] vector<Point *> findFirstSouthEastPoints() const;
    // 查询第一个有节点的西北角区域的东边区域 (task 2)
    [[nodiscard]] vector<Point *> findNorthWestPointsOnEast() const;
    // 查询第一个有节点的西北角区域的南边区域 (task 2)
    [[nodiscard]] vector<Point *> findNorthWestPointsOnSouth() const;
    // 查询第一个有节点的东南角区域的西北边区域 (task 2)
    [[nodiscard]] vector<Point *> findSouthEastPointsOnNorthWest() const;
    // 查询第一个有节点的东南角区域的西北边区域的北侧区域 (task 2)
    [[nodiscard]] vector<Point *> findSouthEastPointsOnNorthWestOnNorth() const;
    // 查询最近的点
    Point* findNearestPoint(const Point& p) const;
    // 查询最强的点
    Point *findMostPowerfulPoint(Point& p) const;
    // 显示路线上的基站连接情况（task 5,extendTask 1）
    void showResult() const;
    // 显示第三段和第六段的重叠区域情况 (extendTask 2)
    void extendTask2() const;
};

#endif //GSM_QUADTREE_H
