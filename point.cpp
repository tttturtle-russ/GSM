//
// Created by liuhy on 2023/2/22.
//
#include <cmath>
#include "QuadTree.h"

double Point::distance(const Point& p){
    return sqrt((pow((p.y - this->y),2)+pow((p.x - this->x),2)));
}

Point* Point::getMinDistancePoint(const vector<Point *>& p) const{
    int index = 0;
    double minD = 1 << 30;
    for (int i = 0;i < p.size();i++){
        double dis = p[i]->distance(*this);
        if (minD > dis){
            minD = dis;
            index = i;
        }
    }
    return p[index];
}

Point *Point::getMaxPowerPoint(const vector<Point *>& p){
    int index = 0;
    double ePower = 0;
    for (int i = 0; i < p.size(); ++i) {
        double tmp = this->calculateEquivalentIntensity(*p[i]);
        if (isinf(tmp)){
            return p[i];
        }
        if (ePower < tmp){
            ePower = tmp;
            index = i;
        }
    }
    if (ePower <= 0.999999){
        return nullptr;
    }
    return p[index];
}

double mobile::getCos(){
    return (xe - xs) / distance();
}

double mobile::getSin(){
    return (ye - ys) / distance();
}

vector<mobile *> getMobileInfo() {
    ifstream f("./data/yd001.txt",ios::in);
    vector<mobile *> result;
    if (!f.is_open()){
        cout << "移动终端文件打开失败" << endl;
        return {};
    }
    string line;
    getline(f,line); // 跳过第一行
    // 读到 -1，-1 时结束
    while(getline(f,line)){
        if (line == "-1,-1")
            break;
        int xs,ys,xe,ye,speed,start_hour,start_min;
        stringstream ss(line);
        string tmp;
        getline(ss,tmp,',');
        xs = stoi(tmp);
        getline(ss,tmp,',');
        ys = stoi(tmp);
        getline(ss,tmp,',');
        xe = stoi(tmp);
        getline(ss,tmp,',');
        ye = stoi(tmp);
        getline(ss,tmp,',');
        speed = stoi(tmp);
        getline(ss,tmp,',');
        start_hour = stoi(tmp);
        getline(ss,tmp,',');
        start_min = stoi(tmp);
        result.push_back(new mobile(xs,ys,xe,ye,speed,start_hour,start_min));
    }
    f.close();
    return result;
}
