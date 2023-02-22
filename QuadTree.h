//
// Created by liuhy on 2023/2/21.
//

#ifndef GSM_QUADTREE_H
#define GSM_QUADTREE_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#define MAXX 109996
#define MAXY 39946
#define MINX (-26)
#define MINY (-82)
using namespace std;
extern string InitResult;
typedef struct base_station{
    int x,y,id;
    float power;
    string type;
    base_station(int _x,int _y, int _id,float _power,string _type){
        x = _x;
        y = _y;
        id = _id;
        power = _power;
        type = _type;
    }
}base;

typedef struct fake_station{
    int xs,ys,xe,ye,speed,id;
    int start,duration;
    string type;
    fake_station(int _xs,int _ys,int _xe,int _ye,int _speed,int _id,int _start,int _duration,string _type){
        xs = _xs;
        ys = _ys;
        xe = _xe;
        ye = _ye;
        speed = _speed;
        id = _id;
        start = _start;
        duration = _duration;
        type = _type;
    }
}fake;

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

struct Node
{
    Point pos;
    base *data;
    Node(Point _pos, base *_data)
    {
        pos = _pos;
        data = _data;
    }
    Node()
    {
        data = nullptr;
    }

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
public:
    Quad(Point _botLeft, Point _topRight);
    Quad();
    //插入
    void insert(Node*);
    //搜索
    Node* search(Point);
    //碰撞
    bool inBoundary(Point) const;
    bool init();
    void searchTopLeft(vector<Node*> &result);

    vector<Node *> searchNearbyNodes(Point p, double radius);
};

#endif //GSM_QUADTREE_H
