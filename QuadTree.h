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

struct Point {
    double x,y;
    base* value;
    Point(double _x,double _y) : x(_x),y(_y){}
    Point(double _x,double _y,base* v) : x(_x),y(_y),value(v){}
    bool operator == (const Point& p) const{
        return this->x == p.x && this->y == p.y;
    }
};

class QuadTree {
    const int POINT_CAPACITY = 10;
    int x, y; // 当前节点所表示的矩形区域的左下角坐标
    int w, h; // 当前节点所表示的矩形区域的宽度和高度
    vector<Point*> value; // 当前节点存储的值
    QuadTree* nw; // 左上子节点
    QuadTree* ne; // 右上子节点
    QuadTree* sw; // 左下子节点
    QuadTree* se; // 右下子节点

    bool isLeaf() const {
        return nw == nullptr && ne == nullptr && sw == nullptr && se == nullptr;
    }

    void subdivide() {
        nw = new QuadTree(x, y + h / 2, w / 2, h / 2);
        ne = new QuadTree(x + w / 2, y + h / 2, w / 2, h / 2);
        sw = new QuadTree(x, y, w / 2, h / 2);
        se = new QuadTree(x + w / 2, y, w / 2, h / 2);
    }

public:
    QuadTree(int x, int y, int w, int h) : x(x), y(y), w(w), h(h), value(0), nw(nullptr), ne(nullptr), sw(nullptr), se(nullptr){}

    ~QuadTree() {
        delete nw;
        delete ne;
        delete sw;
        delete se;
    }

    void insert(Point*p) ;

    Point* query(const Point& p) const;
    string init();

    bool intersects(double rx, double ry, double rw, double rh) const;

    // 查询第一个有节点的西北角区域 (task 1)
    vector<Point *> findFirstNorthWestPoints() const;
    // 查询第一个有节点的东南角区域 (task 1)
    vector<Point *> findFirstSouthEastPoints() const;
    // 查询第一个有节点的西北角区域的东边区域 (task 2)
    vector<Point *> findNorthWestPointsOnEast() const;
    // 查询第一个有节点的西北角区域的南边区域 (task 2)
    vector<Point *> findNorthWestPointsOnSouth() const;
    // 查询第一个有节点的东南角区域的西北边区域 (task 2)
    vector<Point *> findSouthEastPointsOnNorthWest() const;
    // 查询第一个有节点的东南角区域的西北边区域的北侧区域 (task 2)
    vector<Point *> findSouthEastPointsOnNorthWestOnNorth() const;
};

#endif //GSM_QUADTREE_H
