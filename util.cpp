//
// Created by liuhy on 2023/2/22.
//
#include "QuadTree.h"
#include "util.h"
using namespace std;
bool base::isPowerful(base *b,Point p) {
    static double mostPower = 0;
    double _power = 0;
    if (b->type == "城区"){
        _power = b->power * pow((CITY_DISTANCE / b->distance(p)), 2);
    }else if (b->type == "乡镇"){
        _power = b->power * pow((VALLEY_DISTANCE / b->distance(p)), 2);
    } else if (b->type == "高速"){
        _power = b->power * pow((HIGHWAY_DISTANCE / b->distance(p)), 2);
    }
    if (mostPower < _power){
        mostPower = _power;
        return false;
    }
    return true;
}

double base::distance(Point _p) {
    return sqrt(pow(p.x - _p.x, 2) + pow(p.y - _p.y, 2));
}

bool Node::inBoundary(double x1, double y1, double x2, double y2) const {
    return pos.x >= x1 && pos.x <= x2 && pos.y >= y2 && pos.y <= y1;
}
