//
// Created by liuhy on 2023/2/22.
//
#include <cmath>
#include "QuadTree.h"
#include "util.h"

vector<mobile *> mobileInfo;
vector<fake *> fakeInfo;

void showChoice(){
    cout << "0.�˳�����" << endl;
    cout << "1.��ʾ�׸������ǵ�����" << endl;
    cout << "2.��ʾ�׸����Ͻǵ�����" << endl;
    cout << "3.��ʾ�����ǵ�һ������Ķ�������" << endl;
    cout << "4.��ʾ�����ǵ�һ��������ϲ�����" << endl;
    cout << "5.��ʾ���Ͻǵ�һ�����������������" << endl;
    cout << "6.��ʾ���Ͻǵ�һ�������������ı�������" << endl;
    cout << "7.����ָ���������Ҫ���վ" << endl;
    cout << "8.��ʾ�ƶ��ն˵Ĺ켣�ͻ�վ�������" << endl;
    cout << "9.�����ص������ʱ��" << endl;
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

// �Ӹ�����Points�����ȡ������С�Ļ�վ
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

// �Ӹ�����Points���������ǿ�����Ļ�վ
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

// ���������Чǿ��
double Point::calculateEquivalentIntensity(const Point p) {
    double result = 0;
    if (p.value->type == "����"){
        double dis = this->distance(p);
        result = p.value->power * pow((CITY_DISTANCE/dis),2);
    }else if (p.value->type == "����"){
        double dis = this->distance(p);
        result = p.value->power * pow((VALLEY_DISTANCE/dis),2);
    }else if (p.value->type == "����") {
        double dis = this->distance(p);
        result = p.value->power * pow((HIGHWAY_DISTANCE/dis),2);
    }
    return result;
}

// ���ļ��ж�ȡ�ƶ��ն���Ϣ
vector<mobile *> readMobileInfo() {
    ifstream f("./data/yd001.txt",ios::in);
    vector<mobile *> result;
    if (!f.is_open()){
        cout << "�ƶ��ն��ļ���ʧ��" << endl;
        return {};
    }
    string line;
    getline(f,line); // ������һ��
    // ���� -1��-1 ʱ����
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

// ���ļ��ж�ȡα��վ��Ϣ
vector<fake *> readFakeInfo(){
    vector<fake *> fake;
    ifstream f("./data/wz001.txt",ios::in);
    if (!f.is_open()){
        cout << "�ٻ�վ�ļ���ʧ��" << endl;
        return {};
    }
    string line;
    getline(f,line); // ������һ��
    // ���� -1��-1 ʱ����
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

// �����վ��Ч��Χ������ֵ
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
        // mid�����ص������_x,_y�ı�
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

// ���ص�����ʱ�ĵ��� (��Ե�����վ�ĵ���)
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

// �ж�this��p֮����Ƿ���Ч
bool Point::isValid(Point p) {
    return this->calculateEquivalentIntensity(p) > 0.999999;
}


Point * fake::getPosition(double time,int curHour,int curMin){
    auto p = new Point(this->xs,this->ys);
    p->id = this->id;
    // ���㵱ǰʱ������ʼʱ��Ĳ�ֵ
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

// ����time��ȡ��ǰʱ���
string getTimeStamp(double time, mobile m,tm* t){
    string timeStamp(11, '\0');
    if(time + m.start_min >= 60){
        t->tm_hour = m.start_hour + (time + m.start_min) / 60; //Сʱ��λ
        t->tm_min = int(time + m.start_min) % 60; //���ӽ�λ���ᶪʧ����
        t->tm_sec = (time + m.start_min - int(time + m.start_min)) * 60; //������λ���ᶪʧ����
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
