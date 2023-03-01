//
// Created by liuhy on 2023/2/24.
//

#include <memory>
#include "QuadTree.h"


string QuadTree::init() {
    auto mode = std::getenv("mode");
    string result;
    // read from ./data/*.txt and store them to QuadTree
    ifstream f1,f2,t;
    if (strcmp(mode,"release") == 0) {
        cout << "release mode" << endl;
        f1.open("./data/jz001.txt", ios::in);
        f2.open("./data/jz002.txt", ios::in);
        if (!f1.is_open()|| !f2.is_open()) {
            return "Error open file";
        }
    }
    else{
        t.open("./data/test.txt", ios::in);
        cout << "test mode" << endl;
        if (!t.is_open()) {
            return "Error open file";
        }
    }
    // read jz001.txt
    string buf;
    vector<string > v;
    clock_t start,end;
    start = clock();
    // 不处理第一行和最后一行
    if (strcmp(mode,"release") == 0){
        getline(f1,buf);
        while (getline(f1,buf)) {
            v.push_back(buf);
        }
        v.pop_back();
        getline(f2,buf);
        while (getline(f2,buf)) {
            v.push_back(buf);
        }
        v.pop_back();
    } else{
        getline(t,buf);
        while (getline(t,buf)) {
            v.push_back(buf);
        }
        v.pop_back();
    }
    for (const auto & i : v) {
        istringstream iss(i);
        vector<string> tokens;
        string token,type,p;
        while (std::getline(iss, token, ',')) {
            tokens.push_back(token);
        }
        float x = stof(tokens[0]);
        float y = stof(tokens[1]);
        istringstream ss(tokens[2]);
        ss >> type >> p;
        float power = stof(p);
        int id = stoi(token);
        // 将上述数据插入四叉树
        insert(new Point(x,y,new base(id,power,type)));
    }
    f1.close();
    f2.close();
    t.close();
    end = clock();
    result.resize(40);
    sprintf_s(&result[0],result.size(),"Init Successfully in %d ms",end-start);
    return result;
}

void QuadTree::insert(Point *p) {
    if (p->x > x + w || p->x < x || p->y > y + h || p->y < y)
        return;
    if (isLeaf()){
        if (value.size() < POINT_CAPACITY){
            value.push_back(p);
        }else{
            subdivide();
            // 重新分配节点到子节点叶子中
            for (auto i : value) {
                if (i->x < nw->x + nw->w && i->y > nw->y){
                    nw->insert(i);
                }else if (i->x > ne->x && i->y > ne->y){
                    ne->insert(i);
                }else if (i->x < sw->x + sw->w && i->y > sw->y){
                    sw->insert(i);
                }else{
                    se->insert(i);
                }
            }
            value.clear();
            // 清空非叶子节点的值
        }
    }else{
        // 不是叶子节点则递归的向下插入
        if (p->x < nw->x + nw->w && p->y > nw->y){
            nw->insert(p);
        }else if (p->x > ne->x && p->y > ne->y){
            ne->insert(p);
        }else if (p->x < sw->x + sw->w && p->y > sw->y){
            sw->insert(p);
        }else{
            se->insert(p);
        }
    }
}

Point *QuadTree::query(const Point &p) const {
    if (isLeaf()) {
        for (auto i:value) {
            if (*i == p){
                return i;
            }
        } // 如果当前节点是叶子节点，返回存储的值
        return nullptr;
    }
    if (p.x < x + w / 2 && p.y > y + h / 2) {
        return nw->query(p); // 如果查询点在左上子节点所表示的矩形区域内，则递归查询左上子节点
    } else if (p.x > x + w / 2 && p.y > y + h / 2) {
        return ne->query(p); // 如果查询点在右上子节点所表示的矩形区域内，则递归查询右上子节点
    } else if (p.x < x + w / 2 && p.y < y + h / 2) {
        return sw->query(p); // 如果查询点在左下子节点所表示的矩形区域内，则递归查询左下子节点
    } else {
        return se->query(p); // 如果查询点在右下子节点所表示的矩形区域内，则递归查询右下子节点
    }
}

vector<Point *> QuadTree::findFirstNorthWestPoints() const {
    auto tmp = this;
    while(true){
        // 查找顺序为西北，东北，西南，东南
        // 若查找到了叶子节点，则对4个叶子判断，找到第一个有值的区域（叶子）
        if (!tmp->nw->isLeaf()){
            tmp = tmp->nw;
        }else if (!tmp->ne->isLeaf()){
            tmp = tmp->ne;
        }else if (!tmp->sw->isLeaf()){
            tmp = tmp->sw;
        }else if (!tmp->se->isLeaf()){
            tmp = tmp->se;
        }else{
            if (!tmp->nw->value.empty()){
                tmp = tmp->nw;
            }else if (!tmp->ne->value.empty()){
                tmp = tmp->ne;
            }else if (!tmp->sw->value.empty()){
                tmp = tmp->sw;
            }else if (!tmp->se->value.empty()){
                tmp = tmp->se;
            }else{
                return tmp->value;
            }
            break;
        }
    }
    return tmp->value;
}

vector<Point *> QuadTree::findFirstSouthEastPoints() const {
    auto tmp = this;
    while(true){
        // 查找顺序为东南，西南，东北，西北
        // 若查找到了叶子节点，则对4个叶子判断，找到第一个有值的区域（叶子）
        if (!tmp->se->isLeaf()){
            tmp = tmp->se;
        }else if (!tmp->sw->isLeaf()){
            tmp = tmp->sw;
        }else if (!tmp->ne->isLeaf()){
            tmp = tmp->ne;
        }else if (!tmp->nw->isLeaf()){
            tmp = tmp->nw;
        }else{
            if (!tmp->se->value.empty()){
                tmp = tmp->se;
            }else if (!tmp->sw->value.empty()){
                tmp = tmp->sw;
            }else if (!tmp->ne->value.empty()){
                tmp = tmp->ne;
            }else if (!tmp->nw->value.empty()){
                tmp = tmp->nw;
            }else{
                return tmp->value;
            }
            break;
        }
    }
    return tmp->value;
}

vector<Point *> QuadTree::findNorthWestPointsOnEast() const {
    auto tmp = this;
    while(true){
        if (!tmp->nw->isLeaf()){
            tmp = tmp->nw;
        }else if (!tmp->ne->isLeaf()){
            tmp = tmp->ne;
        }else if (!tmp->sw->isLeaf()){
            tmp = tmp->sw;
        }else if (!tmp->se->isLeaf()){
            tmp = tmp->se;
        }else{
            tmp = tmp->ne;
            break;
        }
    }
    return tmp->value;
}

vector<Point *> QuadTree::findNorthWestPointsOnSouth() const {
    auto tmp = this;
    while(true){
        if (!tmp->nw->isLeaf()){
            tmp = tmp->nw;
        }else if (!tmp->ne->isLeaf()){
            tmp = tmp->ne;
        }else if (!tmp->sw->isLeaf()){
            tmp = tmp->sw;
        }else if (!tmp->se->isLeaf()){
            tmp = tmp->se;
        }else{
            tmp = tmp->sw;
            break;
        }
    }
    return tmp->value;
}

vector<Point *> QuadTree::findSouthEastPointsOnNorthWest() const {
    auto tmp = this;
    while(true){
        if (!tmp->se->isLeaf()){
            tmp = tmp->se;
        }else if (!tmp->sw->isLeaf()){
            tmp = tmp->sw;
        }else if (!tmp->ne->isLeaf()){
            tmp = tmp->ne;
        }else if (!tmp->nw->isLeaf()){
            tmp = tmp->nw;
        }else{
            tmp = tmp->nw;
            break;
        }
    }
    return tmp->value;
}

vector<Point *> QuadTree::findSouthEastPointsOnNorthWestOnNorth() const {
    auto tmp = this;
    const QuadTree* front;
    // front用于记录父节点，便于查找相邻分支
    if (!this->se->isLeaf()){
        tmp = tmp->se;
    }
    while(true){
        if (!tmp->se->isLeaf()){
            front = tmp;
            tmp = tmp->se;
        }else if (!tmp->sw->isLeaf()){
            front = tmp;
            tmp = tmp->sw;
        }else if (!tmp->ne->isLeaf()){
            front = tmp;
            tmp = tmp->ne;
        }else if (!tmp->nw->isLeaf()){
            front = tmp;
            tmp = tmp->nw;
        }else{
            tmp = front->ne->sw;
            break;
        }
    }
    return tmp->value;
}

Point *QuadTree::findNearestPoint(const Point &p) const {
    auto v = searchNearbyPoints(p);
    cout << "附近的点有"<<v.size() <<"个"<< endl;
    return p.getMinDistancePoint(v);
}

// 查找距离p点距离小于r的点
vector<Point *> QuadTree::searchNearbyPoints(const Point &p, double r) const{
    vector<Point *> result;
    if (isLeaf()){
        for (auto &i : value){
            if (i->distance(p) <= r){
                result.push_back(i);
            }
        }
        return result;
    }
    if (nw != nullptr){
        auto nwResult = nw->searchNearbyPoints(p, r);
        result.insert(result.end(), nwResult.begin(), nwResult.end());
    }
    if (ne != nullptr){
        auto neResult = ne->searchNearbyPoints(p, r);
        result.insert(result.end(), neResult.begin(), neResult.end());
    }
    if (sw != nullptr){
        auto swResult = sw->searchNearbyPoints(p, r);
        result.insert(result.end(), swResult.begin(), swResult.end());
    }
    if (se != nullptr){
        auto seResult = se->searchNearbyPoints(p, r);
        result.insert(result.end(), seResult.begin(), seResult.end());
    }
    return result;
}

Point* QuadTree::findMostPowerfulPoint(Point& p) const{
    auto v = searchNearbyPoints(p);
    return p.getMaxPowerPoint(v);
}

//轨迹移动
void QuadTree::checkMove(mobile m) const {
    double step = 10; // 步长
    double cos = m.getCos();
    double sin = m.getSin();
    double dis = m.distance();
    int timer = 0; // 计时器
    Point p = Point(m.xs, m.ys);
    Point *cur = nullptr; // 当前连接的基站
    Point *pre = nullptr; // 前一个连接的基站
    auto t = new tm;
    string timeStamp;
    timeStamp.resize(11);
    while(step * timer < dis){
        cur = findMostPowerfulPoint(p);
        while (cur == nullptr && step * timer < dis){
            timer++;
            p.x += step * cos;
            p.y += step * sin;
            cur = findMostPowerfulPoint(p);
        }
        if (cur == nullptr)
            break;
        if (pre != cur){
            pre = cur;
            p.enterIterateCalculate(cur,pre,cos,sin);
            double time = p.distance(Point(m.xs,m.ys)) / (double(m.speed) * 50 / 3);
            timeStamp = getTimeStamp(time, m,t);
            // 格式化字符串
            cout << timeStamp << "\t与" << cur->value->id << "建立连接" << endl;
            while(step * timer < dis){
                timer++;
                p.x += step * cos;
                p.y += step * sin;
                cur = findMostPowerfulPoint(p);
                if (cur == nullptr || cur != pre){
                    p.outIterateCalculate(pre,cos,sin);
                    double time = p.distance(Point(m.xs,m.ys)) / (double(m.speed) * 50 / 3);
                    timeStamp = getTimeStamp(time, m,t);
                    cout << timeStamp << "\t与" << pre->value->id << "断开连接" << endl;
                    break;
                }
            }
        }
        timer++;
        p.x += step * cos;
        p.y += step * sin;
    }
}

void QuadTree::checkOverlap(mobile m) const{
    double step = 10; // 步长
    double cos = m.getCos();
    double sin = m.getSin();
    double dis = m.distance();
    int timer = 0; // 计时器
    Point p = Point(m.xs, m.ys);
    Point *cur = nullptr; // 当前连接的基站
    Point *pre = nullptr; // 前一个连接的基站
    auto *start_point = new Point();
    auto *end_point = new Point();
    auto preTime = new tm;
    auto curTime = new tm;
    string preTimeStamp(11, '0');
    string curTimeStamp(11, '0');
    cur = findMostPowerfulPoint(p);
    while (cur == nullptr && step * timer < dis){
        timer++;
        p.x += step * cos;
        p.y += step * sin;
        cur = findMostPowerfulPoint(p);
    }
    if (cur == nullptr)
        return;
    pre = cur;
    while (pre == cur){
        timer++;
        p.x += step * cos;
        p.y += step * sin;
        cur = findMostPowerfulPoint(p);
    }
    Point tmp = Point(m.xs, m.ys);
    while(!tmp.isValid(*cur) || !tmp.isValid(*pre)){
        tmp.x += step * cos;
        tmp.y += step * sin;
    }
    start_point->x = tmp.x;
    start_point->y = tmp.y;
    while(tmp.isValid(*cur) && tmp.isValid(*pre)){
        tmp.x += step * cos;
        tmp.y += step * sin;
    }
    end_point->x = tmp.x;
    end_point->y = tmp.y;
    // 迭代求得精确坐标,误差0.1m
    start_point->enterIterateCalculate(cur,pre,cos,sin);
    end_point->outIterateCalculate(cur,pre,cos,sin);
    double time1 = start_point->distance(Point(m.xs, m.ys)) / (double(m.speed) * 50 / 3);
    // 7.242773428
    double time2 = end_point->distance(Point(m.xs, m.ys)) / (double(m.speed) * 50 / 3);
    // 7.799414063
    double time = (time2 - time1) * 60;
    preTimeStamp = getTimeStamp(time1, m, preTime);
    curTimeStamp = getTimeStamp(time2, m, curTime);
    cout << preTimeStamp << "\t进入" << pre->value->id << "与" << cur->value->id << "的重叠区域" << endl;
    cout << curTimeStamp << "\t离开" << pre->value->id << "与" << cur->value->id << "的重叠区域" << endl;
    cout << time << "秒" << endl;
    delete start_point;
    delete end_point;
    delete preTime;
    delete curTime;
}


void QuadTree::checkConnectToFake(mobile m,const vector<fake_station *>& f) const{
    double timer = 0;
    Point p = Point(m.xs, m.ys);
    int curHour = m.start_hour;
    int curMinute = m.start_min;
    int curSecond = 0;
    while(timer <= m.distance() / (double (m.speed) * 1000/ 3600)){
        for (const auto &item: f)
        {
            auto tmp = item->getPosition(timer,curHour,curMinute);
            if (tmp->distance(p) <= 40){
                string timeStamp(11, '\0');
                auto t = new tm;
                timeStamp = getTimeStamp(timer/60,m,t);
                cout << timeStamp << "\t与" << item->id << "号伪基站建立连接" << endl;
            }
        }
        timer++;
        if (curSecond + timer >= 60){
            curSecond = curSecond + int(timer) - 60;
            curMinute++;
            if (curMinute >= 60){
                curMinute = 0;
                curHour++;
            }
        }
    }
}

void QuadTree::showResult() const {
    auto m = readMobileInfo();
    for (const auto& item: m)
        checkMove(*item);
}

void QuadTree::extendTask2() const {
    auto m = readMobileInfo();
    checkOverlap(*m[2]);
    checkOverlap(*m[5]);
}


// 升级功能1
void QuadTree::advancedTask1() const {
    auto f = readFakeInfo();
    auto m = readMobileInfo();
    checkConnectToFake(*m[11],f);
}



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
