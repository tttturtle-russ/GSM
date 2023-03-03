//
// Created by liuhy on 2023/2/22.
//
#include <cmath>
#include "QuadTree.h"
#include "util.h"

vector<mobile *> mobileInfo;
vector<fake *> fakeInfo;

void showChoice(){
    cout << "0.退出程序" << endl;
    cout << "1.显示首个西北角的区域" << endl;
    cout << "2.显示首个东南角的区域" << endl;
    cout << "3.显示西北角第一个区域的东侧区域" << endl;
    cout << "4.显示西北角第一个区域的南侧区域" << endl;
    cout << "5.显示东南角第一个区域的西北侧区域" << endl;
    cout << "6.显示东南角第一个区域的西北侧的北侧区域" << endl;
    cout << "7.查找指定点坐标的要求基站" << endl;
    cout << "8.显示移动终端的轨迹和基站连接情况" << endl;
    cout << "9.计算重叠区域的时间" << endl;
}

string initUtil(){
    clock_t start = clock();
    fakeInfo = readFakeInfo();
    mobileInfo = readMobileInfo();
    clock_t end = clock();
    if (fakeInfo.empty() || mobileInfo.empty()){
        return "Init Util Failed";
    }
    string result(50,'\0');
    sprintf_s(&result[0],result.size(),"Init Util Successfully in %d ms",end-start);
    return result;
}

double Point::distance(const Point& p){
    return sqrt((pow((p.y - this->y),2)+pow((p.x - this->x),2)));
}

// 从给定的Points里面获取距离最小的基站
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

// 从给定的Points里面求相对强度最大的基站
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

// 计算相对有效强度
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

// 从文件中读取移动终端信息
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

// 从文件中读取伪基站信息
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

// 进入基站有效范围迭代求值
void Point::enterIterateCalculate(Point* cur,Point* pre,double cos,double sin,int step,double precision){
    double _x = this->x;
    double _y = this->y;
    double pre_x = _x - step * cos;
    double pre_y = _y - step * sin;
    double mid_x = (_x + pre_x) / 2;
    double mid_y = (_y + pre_y) / 2;
    auto* mid = new Point(mid_x, mid_y);
    while(fabs(_x - pre_x) >= precision || fabs(_y - pre_y) >= precision){
        if (mid->isValid(*cur) && mid->isValid(*pre)){
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
    delete mid;
}


void Point::outIterateCalculate(Point* cur,Point* pre,double cos,double sin,int step,double precision){
    double _x = this->x;
    double _y = this->y;
    double pre_x = _x - step * cos;
    double pre_y = _y - step * sin;
    double mid_x = (_x + pre_x) / 2;
    double mid_y = (_y + pre_y) / 2;
    auto* mid = new Point(mid_x, mid_y);
    while(fabs(_x - pre_x) >= precision || fabs(_y - pre_y) >= precision){
        // mid不在重叠区则对_x,_y改变
        if (!mid->isValid(*cur) || !mid->isValid(*pre)){
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
    delete mid;
}

// 出重叠区域时的迭代 (针对单个基站的迭代)
void Point::outIterateCalculate(Point *pre, double cos, double sin, int step, double precision) {
    double _x = this->x;
    double _y = this->y;
    double pre_x = _x - step * cos;
    double pre_y = _y - step * sin;
    double mid_x = (_x + pre_x) / 2;
    double mid_y = (_y + pre_y) / 2;
    auto* mid = new Point(mid_x, mid_y);
    while(fabs(_x - pre_x) >= precision || fabs(_y - pre_y) >= precision){
        if (!mid->isValid(*pre)){
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
    delete mid;
}

// 判断this与p之间的是否有效
bool Point::isValid(Point p) {
    return this->calculateEquivalentIntensity(p) > 0.999999;
}


Point * fake::getPosition(double time,int curHour,int curMin){
    auto p = new Point(this->xs,this->ys);
    p->id = this->id;
    // 计算当前时间与起始时间的差值
    double diff = abs(curHour - this->start_hour) * 3600 + abs(curMin - this->start_min) * 60 + time;
    p->x += double(this->speed) * 1000.0 / 3600.0 * time * this->getCos();
    p->y += double(this->speed) * 1000.0 / 3600.0 * time * this->getSin();
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
}

double fake_station::distance() {
    double x = this->xe - this->xs;
    double y = this->ye - this->ys;
    return sqrt(x*x + y*y);
};

// 根据time获取当前时间戳
string getTimeStamp(double time, mobile m,tm* t){
    string timeStamp(11, '\0');
    if(time + m.start_min >= 60){
        t->tm_hour = m.start_hour + (time + m.start_min) / 60; //小时进位
        t->tm_min = int(time + m.start_min) % 60; //分钟进位，会丢失精度
        t->tm_sec = (time + m.start_min - int(time + m.start_min)) * 60; //秒数进位，会丢失精度
    }else{
        t->tm_hour = m.start_hour;
        t->tm_min = int(time + m.start_min) % 60;
        t->tm_sec = (time + m.start_min - int(time + m.start_min)) * 60;
    }
    strftime(&timeStamp[0], timeStamp.size(), "[%H:%M:%S]", t);
    return timeStamp;
}

vector<fake *> fakeFilter(vector<fake *> f,mobile m){
    vector<fake *> result;
    for (const auto &item: f){
        int start1 = item->start_hour * 60 + item->start_min;
        int end1 = item->end_hour * 60 + item->end_min;
        int start2 = m.start_hour * 60 + m.start_min;
        int end2 = m.end_hour * 60 + m.end_min;
        if (end1 <= start2 || end2 <= start1){
            continue;
        }else{
            result.push_back(item);
        }
    }
    return result;
}
