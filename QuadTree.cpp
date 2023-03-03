//
// Created by liuhy on 2023/2/24.
//

#include "QuadTree.h"
#include "util.h"
string QuadTree::init() {
    auto mode = std::getenv("mode");
    string result;
    // read from ./data/*.txt and store them to QuadTree
    ifstream f1,f2,t;
    // release 环境和 debug 环境
    if (strcmp(mode,"release") == 0) {
        cout << "release mode" << endl;
        f1.open("./data/jz001.txt", ios::in);
        f2.open("./data/jz002.txt", ios::in);
        if (!f1.is_open()|| !f2.is_open()) {
            cout << "打开基站文件失败" << endl;
            system("pause");
            exit(1);
        }
    }
    else{
        t.open("./data/test.txt", ios::in);
        cout << "test mode" << endl;
        if (!t.is_open()) {
            system("pause");
            exit(1);
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
        // 解析数据，并绑定到结构体中
        istringstream iss(i);
        vector<string> tokens;
        string token,type,p;
        while (std::getline(iss, token, ',')) {
            tokens.push_back(token);
        }
        double x = stod(tokens[0]);
        double y = stod(tokens[1]);
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
    sprintf_s(&result[0],result.size(),"Init QuadTree Successfully in %d ms",end-start);
    return result;
}

void QuadTree::insert(Point *p) {
    if (p->x > x + w || p->x < x || p->y > y + h || p->y < y)
        return;
    if (isLeaf()){
        if (value.size() < POINT_CAPACITY){
            value.push_back(p);
            // 是叶子且叶子容量未满，直接插入即可
        }else{
            //是叶子但是叶子容量已满，则叶子节点4分
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

// 查找距离p点距离小于r的点 (r默认5000)
vector<Point *> QuadTree::searchNearbyPoints(const Point &p, double r) const{
    vector<Point *> result;
    if (isLeaf()){
        for (auto &i : value){
            // 距离小于r就插入result
            if (i->distance(p) <= r){
                result.push_back(i);
            }
        }
        return result;
    }
    // 递归的向下查询
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
    double cos = m.getCos(); //x分量
    double sin = m.getSin(); //y分量
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
        // 查找直到连接上第一个基站
        if (cur == nullptr)
            break;
        // 如果pre != cur则连接到新基站
        if (pre != cur){
            pre = cur;
            // 迭代获取精确值
            p.enterIterateCalculate(cur,pre,cos,sin);
            double time = p.distance(Point(m.xs,m.ys)) / (double(m.speed) * 50 / 3);
            timeStamp = getTimeStamp(time, m,t);
            // 格式化字符串
            cout << timeStamp << "\t与" << cur->value->id << "建立连接" << endl;
            // 继续寻找断开连接的时刻
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
    double cos = m.getCos(); // x分量
    double sin = m.getSin(); // y分量
    double dis = m.distance();
    int timer = 0; // 计时器
    Point p = Point(m.xs, m.ys);
    Point *cur = nullptr; // 当前连接的基站
    Point *pre = nullptr; // 前一个连接的基站
    auto *start_point = new Point(); // 进入重叠区域时的点
    auto *end_point = new Point(); // 离开重叠区域时的点
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
    // 此时pre 和 cur为两个重叠的基站
    // 从头开始查询
    Point tmp = Point(m.xs, m.ys);
    while(!tmp.isValid(*cur) || !tmp.isValid(*pre)){
        tmp.x += step * cos;
        tmp.y += step * sin;
    }
    // 进入重叠区域后记录start_point的值
    start_point->x = tmp.x;
    start_point->y = tmp.y;
    while(tmp.isValid(*cur) && tmp.isValid(*pre)){
        tmp.x += step * cos;
        tmp.y += step * sin;
    }
    // 离开重叠区域后记录end_point的值
    end_point->x = tmp.x;
    end_point->y = tmp.y;
    // 迭代求得精确坐标
    start_point->enterIterateCalculate(cur,pre,cos,sin);
    end_point->outIterateCalculate(cur,pre,cos,sin);
    double time1 = start_point->distance(Point(m.xs, m.ys)) / (double(m.speed) * 50 / 3);
    double time2 = end_point->distance(Point(m.xs, m.ys)) / (double(m.speed) * 50 / 3);
    // 时间差
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
