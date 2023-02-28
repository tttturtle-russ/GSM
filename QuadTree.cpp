//
// Created by liuhy on 2023/2/24.
//

#include "QuadTree.h"


string QuadTree::init() {
    auto mode = std::getenv("mode");
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
    cout << v.size() << endl;
    int cnt = 0;
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
        cnt++;
    }
    cout << "成功插入" << cnt << "数据" << endl;
    f1.close();
    f2.close();
    t.close();
    return "Init Successfully";
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

bool QuadTree::intersects(double rx, double ry, double rw, double rh) const {
    double rMaxX = rx + rw;
    double rMaxY = ry + rh;
    return !(x > rMaxX || x + w < rx || y > rMaxY || y + h < ry);
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
    double step = 20; // 步长
    int timer = 0; // 计时器
    bool hasConnected = false;
    Point *front = nullptr;
    string buf;
    buf.resize(100);
    while(step * timer < m.distance()){
        // 每次移动10米
        Point p = Point(m.xs + step * timer * m.getCos(), m.ys + step * timer * m.getSin());
        auto cur = findMostPowerfulPoint(p);
        double time = step * timer / (m.speed * 50 / 3);
        auto *t = new tm;
        double power;
        if (cur == nullptr){
            if (hasConnected){
                int sec = (time - int(time)) * 60;
                if (time + m.start_min > 60) {
                    t->tm_hour = m.start_hour + 1;
                    t->tm_min = int(time + m.start_min - 60);
                    t->tm_sec = sec;
                    strftime(&buf[0], buf.size(), "[%H:%M:%S]", t);
                    cout << buf << "\t与" << front->value->id << "号基站断开连接，相对信号强度" << power <<endl;
                }else{
                    t->tm_hour = m.start_hour;
                    t->tm_min = int(time + m.start_min);
                    t->tm_sec = sec;
                    strftime(&buf[0], buf.size(), "[%H:%M:%S]", t);
                    cout << buf << "\t与" << front->value->id << "号基站断开连接，相对信号强度" << power << endl;
                }
                hasConnected = false;
            }
        }else{
            if (!hasConnected){
                power = p.calculateEquivalentIntensity(*cur);
                int sec = (time - int(time)) * 60;
                if (time + m.start_min > 60) {
                    t->tm_hour = m.start_hour + 1;
                    t->tm_min = int(time + m.start_min - 60);
                    t->tm_sec = sec;
                    strftime(&buf[0], buf.size(), "[%H:%M:%S]", t);
                    cout << buf << "\t与" << cur->value->id << "号基站成功连接，相对信号强度" << power << endl;
                }else{
                    t->tm_hour = m.start_hour;
                    t->tm_min = int(time + m.start_min);
                    t->tm_sec = sec;
                    strftime(&buf[0], buf.size(), "[%H:%M:%S]", t);
                    cout << buf << "\t与" << cur->value->id << "号基站成功连接，相对信号强度" << power << endl;
                }
                hasConnected = true;
                front = cur;
            }
        }
        timer++;
    }
}

void QuadTree::showResult() const {
    auto m = getMobileInfo();
    for (const auto& item: m)
        checkMove(*item);
}