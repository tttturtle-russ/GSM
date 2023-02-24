//
// Created by liuhy on 2023/2/21.
//

#ifndef GSM_QUADTREE_H
#define GSM_QUADTREE_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <cstdlib>
#define MAXX 109996
#define MAXY 39946
#define MINX (-26)
#define MINY (-82)
using namespace std;
extern string InitResult;
struct Point
{
    int x;
    int y;
    Point(int _x, int _y)
    {
        x = _x;
        y = _y;
    }
    Point()
    {
        x = 0;
        y = 0;
    }
    // 重载比较运算符
    bool operator==(const Point& p) const{
        return x == p.x && y == p.y;
    }

    double distance(Point point) const {
        return sqrt(pow(x - point.x, 2) + pow(y - point.y, 2));
    }
};
typedef struct base_station{
    Point p;
    int id;
    float power;
    string type;
    base_station(Point _p, int _id,float _power,string _type){
        p = _p;
        id = _id;
        power = _power;
        type = _type;
    }
    double distance(Point _p);
    bool isPowerful(base_station *b,Point p);
}base;

typedef struct fake_station{
    Point s_point,e_point;
    int speed,id;
    int start,duration;
    string type;
    fake_station(Point s_p,Point e_p,int _speed,int _id,int _start,int _duration,string _type){
        s_point = s_p;
        e_point = e_p;
        speed = _speed;
        id = _id;
        start = _start;
        duration = _duration;
        type = _type;
    }
}fake;

typedef struct mobile {
    int xs,ys,xe,ye;
    int speed;
    int start_hour,start_min;
}mobile;


struct Node
{
    Point pos;
    base *data;
    Node(Point _pos, base *_data)
    {
        pos = _pos;
        data = _data;
    }
    Node() {
        data = nullptr;
    }
    bool inBoundary(double x1, double y1, double x2, double y2) const;
};


class Quad
{
    // 范围
    Point botLeft;
    Point topRight;
    // 根节点
    Node *n;
    // 子结点
    Quad *topLeftTree;
    Quad *topRightTree;
    Quad *botLeftTree;
    Quad *botRightTree;
    void subdivide() {
        Point mid = Point((botLeft.x + topRight.x)/2, (topRight.y + botLeft.y)/2);
        topLeftTree = new Quad(botLeft, mid);
        topRightTree = new Quad(Point(mid.x, botLeft.y), Point(topRight.x, mid.y));
        botLeftTree = new Quad(Point(botLeft.x, mid.y), Point(mid.x, topRight.y));
        botRightTree = new Quad(mid, topRight);
    }
public:
    Quad(Point _botLeft, Point _topRight);
    Quad();
    ~Quad();
    //插入
    void insert(Node*);
    //搜索
    Node* search(Point);
    //碰撞
    bool inBoundary(Point) const;
    bool inBoundary(Point p, double radius) const;
    bool inBoundary(double x1,double y1,double x2,double y2) const;
    // 初始化
    bool init();
    // 查找给定点周围半径的所有节点
    void searchNearbyNodes(Point p, double radius, vector<Node *> &vector1);
    void clear();
    // 查找最强基站
    base *findMostPowerfulBase(Point,double radius = 10000);
    // 显示西北角和东南角区域中的基站数据
    void showBotRight() const;
    void getNodesInBoundaryHelper(vector<Node *> &nodes, double x1, double y1, double x2, double y2) const;
    vector<Node *> getNodesInBoundary(double d, double d1, double d2, double d3) const;
    void showTopLeft() const;
    void showEast() const;
    void showSouth() const;
};

#endif //GSM_QUADTREE_H
