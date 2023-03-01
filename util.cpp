//
// Created by liuhy on 2023/2/22.
//
#include <cmath>
#include "QuadTree.h"
#include "util.h"
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

double Point::calculateEquivalentIntensity(const Point p) {
    double result = 0;
    if (p.value->type == "城区"){
        double dis = this->distance(p);
        result = p.value->power * pow((CITY_DISTANCE/dis),2);
    }else if (p.value->type == "乡镇"){
        double dis = this->distance(p);
        result = p.value->power * pow((VALLEY_DISTANCE/dis),2);
    }else if (p.value->type == "高速") {
        double dis = this->distance(p);
        result = p.value->power * pow((HIGHWAY_DISTANCE/dis),2);
    }
    return result;
}

vector<mobile *> readMobileInfo() {
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

vector<fake *> readFakeInfo(){
    vector<fake *> fake;
    ifstream f("./data/wz001.txt",ios::in);
    if (!f.is_open()){
        cout << "假基站文件打开失败" << endl;
        return {};
    }
    string line;
    getline(f,line); // 跳过第一行
    // 读到 -1，-1 时结束
    while(getline(f,line)){
        if (line == "-1,-1")
            break;
        int xs,ys,xe,ye,id,speed,start_hour,start_min;
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
        getline(ss,tmp,',');
        id = stoi(tmp);
        fake.push_back(new fake_station(xs,ys,xe,ye,speed,id,start_hour,start_min));
    }
    f.close();
    return fake;
}

// 迭代求值 精度0.1m
void Point::enterIterateCalculate(Point* cur,Point* pre,double cos,double sin,int step,double precision){
    double _x = this->x;
    double _y = this->y;
    double pre_x = _x - step * cos;
    double pre_y = _y - step * sin;
    double mid_x = (_x + pre_x) / 2;
    double mid_y = (_y + pre_y) / 2;
    Point* mid = new Point(mid_x, mid_y);
    while(fabs(_x - pre_x) > precision || fabs(_y - pre_y) > precision){
        if (mid->calculateEquivalentIntensity(*pre) > 0.999999 && mid->calculateEquivalentIntensity(*cur) > 0.999999){
            _x = mid->x;
            _y = mid->y;
        }else{
            pre_x = mid->x;
            pre_y = mid->y;
        }
        mid->x = (_x + pre_x) / 2;
        mid->y = (_y + pre_y) / 2;
    }
    this->x = mid->x;
    this->y = mid->y;
}

void Point::outIterateCalculate(Point* cur,Point* pre,double cos,double sin,int step,double precision){
    double _x = this->x;
    double _y = this->y;
    double pre_x = _x - step * cos;
    double pre_y = _y - step * sin;
    double mid_x = (_x + pre_x) / 2;
    double mid_y = (_y + pre_y) / 2;
    Point* mid = new Point(mid_x, mid_y);
    while(fabs(_x - pre_x) > precision || fabs(_y - pre_y) > precision){
        if (mid->calculateEquivalentIntensity(*cur) <= 0.999999 && mid->calculateEquivalentIntensity(*pre) <= 0.999999){
            _x = mid->x;
            _y = mid->y;
        }else{
            pre_x = mid->x;
            pre_y = mid->y;
        }
        mid->x = (_x + pre_x) / 2;
        mid->y = (_y + pre_y) / 2;
    }
    this->x = mid->x;
    this->y = mid->y;
}

Point * fake::getPosition(double time,int curHour,int curMin){
    Point * p = new Point();
    if(this->start_hour > curHour){
        p->x = this->xs;
        p->y = this->ys;
        p->id = this->id;
        return p;
    }
    if (this->start_hour == curHour && this->start_min >= curMin){
        p->x = this->xs;
        p->y = this->ys;
        p->id = this->id;
        return p;
    }
    if (this->start_hour < curHour  && this->start_min + time / 60.0 < curMin){
        p->x = this->xe;
        p->y = this->ye;
        p->id = this->id;
        return p;
    }
    p->id = this->id;
    double cos = this->getCos();
    double sin = this->getSin();
    double distance = double(this->speed) * 1000.0 / 3600.0 * time;
    p->x = this->xs + distance * cos;
    p->y = this->ys + distance * sin;
    return p;
}

double fake_station::getCos() {
    double x = this->xe - this->xs;
    double y = this->ye - this->ys;
    double dis = sqrt(x*x + y*y);
    return x / dis;
}

double fake_station::getSin() {
    double x = this->xe - this->xs;
    double y = this->ye - this->ys;
    double dis = sqrt(x*x + y*y);
    return y / dis;
};