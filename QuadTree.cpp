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
    // release ������ debug ����
    if (strcmp(mode,"release") == 0) {
        cout << "release mode" << endl;
        f1.open("./data/jz001.txt", ios::in);
        f2.open("./data/jz002.txt", ios::in);
        if (!f1.is_open()|| !f2.is_open()) {
            cout << "�򿪻�վ�ļ�ʧ��" << endl;
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
    // �������һ�к����һ��
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
        // �������ݣ����󶨵��ṹ����
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
        // ���������ݲ����Ĳ���
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
            // ��Ҷ����Ҷ������δ����ֱ�Ӳ��뼴��
        }else{
            //��Ҷ�ӵ���Ҷ��������������Ҷ�ӽڵ�4��
            subdivide();
            // ���·���ڵ㵽�ӽڵ�Ҷ����
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
            // ��շ�Ҷ�ӽڵ��ֵ
        }
    }else{
        // ����Ҷ�ӽڵ���ݹ�����²���
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
        } // �����ǰ�ڵ���Ҷ�ӽڵ㣬���ش洢��ֵ
        return nullptr;
    }
    if (p.x < x + w / 2 && p.y > y + h / 2) {
        return nw->query(p); // �����ѯ���������ӽڵ�����ʾ�ľ��������ڣ���ݹ��ѯ�����ӽڵ�
    } else if (p.x > x + w / 2 && p.y > y + h / 2) {
        return ne->query(p); // �����ѯ���������ӽڵ�����ʾ�ľ��������ڣ���ݹ��ѯ�����ӽڵ�
    } else if (p.x < x + w / 2 && p.y < y + h / 2) {
        return sw->query(p); // �����ѯ���������ӽڵ�����ʾ�ľ��������ڣ���ݹ��ѯ�����ӽڵ�
    } else {
        return se->query(p); // �����ѯ���������ӽڵ�����ʾ�ľ��������ڣ���ݹ��ѯ�����ӽڵ�
    }
}

// ���Ҿ���p�����С��r�ĵ� (rĬ��5000)
vector<Point *> QuadTree::searchNearbyPoints(const Point &p, double r) const{
    vector<Point *> result;
    if (isLeaf()){
        for (auto &i : value){
            // ����С��r�Ͳ���result
            if (i->distance(p) <= r){
                result.push_back(i);
            }
        }
        return result;
    }
    // �ݹ�����²�ѯ
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

//�켣�ƶ�
void QuadTree::checkMove(mobile m) const {
    double step = 10; // ����
    double cos = m.getCos(); //x����
    double sin = m.getSin(); //y����
    double dis = m.distance();
    int timer = 0; // ��ʱ��
    Point p = Point(m.xs, m.ys);
    Point *cur = nullptr; // ��ǰ���ӵĻ�վ
    Point *pre = nullptr; // ǰһ�����ӵĻ�վ
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
        // ����ֱ�������ϵ�һ����վ
        if (cur == nullptr)
            break;
        // ���pre != cur�����ӵ��»�վ
        if (pre != cur){
            pre = cur;
            // ������ȡ��ȷֵ
            p.enterIterateCalculate(cur,pre,cos,sin);
            double time = p.distance(Point(m.xs,m.ys)) / (double(m.speed) * 50 / 3);
            timeStamp = getTimeStamp(time, m,t);
            // ��ʽ���ַ���
            cout << timeStamp << "\t��" << cur->value->id << "��������" << endl;
            // ����Ѱ�ҶϿ����ӵ�ʱ��
            while(step * timer < dis){
                timer++;
                p.x += step * cos;
                p.y += step * sin;
                cur = findMostPowerfulPoint(p);
                if (cur == nullptr || cur != pre){
                    p.outIterateCalculate(pre,cos,sin);
                    double time = p.distance(Point(m.xs,m.ys)) / (double(m.speed) * 50 / 3);
                    timeStamp = getTimeStamp(time, m,t);
                    cout << timeStamp << "\t��" << pre->value->id << "�Ͽ�����" << endl;
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
    double step = 10; // ����
    double cos = m.getCos(); // x����
    double sin = m.getSin(); // y����
    double dis = m.distance();
    int timer = 0; // ��ʱ��
    Point p = Point(m.xs, m.ys);
    Point *cur = nullptr; // ��ǰ���ӵĻ�վ
    Point *pre = nullptr; // ǰһ�����ӵĻ�վ
    auto *start_point = new Point(); // �����ص�����ʱ�ĵ�
    auto *end_point = new Point(); // �뿪�ص�����ʱ�ĵ�
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
    // ��ʱpre �� curΪ�����ص��Ļ�վ
    // ��ͷ��ʼ��ѯ
    Point tmp = Point(m.xs, m.ys);
    while(!tmp.isValid(*cur) || !tmp.isValid(*pre)){
        tmp.x += step * cos;
        tmp.y += step * sin;
    }
    // �����ص�������¼start_point��ֵ
    start_point->x = tmp.x;
    start_point->y = tmp.y;
    while(tmp.isValid(*cur) && tmp.isValid(*pre)){
        tmp.x += step * cos;
        tmp.y += step * sin;
    }
    // �뿪�ص�������¼end_point��ֵ
    end_point->x = tmp.x;
    end_point->y = tmp.y;
    // ������þ�ȷ����
    start_point->enterIterateCalculate(cur,pre,cos,sin);
    end_point->outIterateCalculate(cur,pre,cos,sin);
    double time1 = start_point->distance(Point(m.xs, m.ys)) / (double(m.speed) * 50 / 3);
    double time2 = end_point->distance(Point(m.xs, m.ys)) / (double(m.speed) * 50 / 3);
    // ʱ���
    double time = (time2 - time1) * 60;
    preTimeStamp = getTimeStamp(time1, m, preTime);
    curTimeStamp = getTimeStamp(time2, m, curTime);
    cout << preTimeStamp << "\t����" << pre->value->id << "��" << cur->value->id << "���ص�����" << endl;
    cout << curTimeStamp << "\t�뿪" << pre->value->id << "��" << cur->value->id << "���ص�����" << endl;
    cout << time << "��" << endl;
    delete start_point;
    delete end_point;
    delete preTime;
    delete curTime;
}
