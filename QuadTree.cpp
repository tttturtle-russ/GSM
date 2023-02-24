//
// Created by liuhy on 2023/2/21.
//

#include <cstring>
#include "QuadTree.h"
string InitResult;
void Quad::insert(Node *node)
{
    // ����ڵ㲻�ڵ�ǰ�ڵ�ķ�Χ�ڣ��򷵻�
    if (!inBoundary(node->pos)) {
        return;
    }
    // �����ǰ�ڵ�û���ӽڵ㣬�򽫸ýڵ���Ϊ��ǰ�ڵ���ӽڵ�
    if (n == nullptr) {
        n = node;
        return;
    }
    // �����ǰ�ڵ����ӽڵ㣬�򽫸ýڵ���뵽�ʵ����ӽڵ���
    if (topLeftTree == nullptr) {
        subdivide();
    }
    topLeftTree->insert(node);
    topRightTree->insert(node);
    botLeftTree->insert(node);
    botRightTree->insert(node);
}



Node* Quad::search(Point p)
{
    if (!inBoundary(p))
        return nullptr;
    if (this->n->pos == p)
        return n;
    if ((botLeft.x + topRight.x) / 2 >= p.x)
    {
        if ((botLeft.y + topRight.y) / 2 >= p.y)
        {
            if (this->topLeftTree == nullptr)
                return nullptr;
            return topLeftTree->search(p);
        }
        else
        {
            if (this->botLeftTree == nullptr)
                return nullptr;
            return botLeftTree->search(p);
        }
    }
    else
    {
        if ((botLeft.y + topRight.y) / 2 >= p.y)
        {
            if (this->topRightTree == nullptr)
                return nullptr;
            return topRightTree->search(p);
        }
        else
        {
            if (this->botRightTree == nullptr)
                return nullptr;
            return botRightTree->search(p);
        }
    }
};


bool Quad::inBoundary(Point p) const
{
    return (p.x >= this->botLeft.x &&
            p.x <= this->topRight.x &&
            p.y >= this->botLeft.y &&
            p.y <= this->topRight.y);
}

bool Quad::inBoundary(Point p, double radius) const {
    return p.x >= botLeft.x - radius && p.x <= topRight.x + radius
           && p.y >= botLeft.y - radius && p.y <= topRight.y + radius;
}

bool Quad::inBoundary(double x1, double y1, double x2, double y2) const {
    return (topRight.x >= x1 && botLeft.x <= x2 && topRight.y >= y1 && botLeft.y <= y2);
}

Quad::Quad(Point _botLeft, Point _topRight)
{
    botLeft = _botLeft;
    topRight = _topRight;
    n = nullptr;
    topLeftTree  = nullptr;
    topRightTree = nullptr;
    botLeftTree  = nullptr;
    botRightTree = nullptr;
}


Quad::Quad()
{
    botLeft = Point(0, 0);
    topRight = Point(0, 0);
    n = nullptr;
    topLeftTree  = nullptr;
    topRightTree = nullptr;
    botLeftTree  = nullptr;
    botRightTree = nullptr;
}


bool Quad::init() {
    auto mode = std::getenv("mode");
    // read from ./data/*.txt and store them to QuadTree
    ifstream f1,f2,t;
    if (strcmp(mode,"release") == 0) {
        cout << "release mode" << endl;
        f1.open("./data/jz001.txt", ios::in);
        f2.open("./data/jz002.txt", ios::in);
        if (!f1.is_open()|| !f2.is_open()) {
            InitResult = "Error: cannot open file";
            return false;
        }
    }
    else{
        t.open("./data/test.txt", ios::in);
        cout << "test mode" << endl;
        if (!t.is_open()) {
            InitResult = "Error: cannot open file";
            return false;
        }
    }
    // read jz001.txt
    string buf;
    vector<string > v;
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
    cout << v.size() << endl;
    for (const auto & i : v) {
        istringstream iss(i);
        vector<string> tokens;
        string token,type,p;
        while (std::getline(iss, token, ',')) {
            tokens.push_back(token);
        }
        int x = stoi(tokens[0]);
        int y = stoi(tokens[1]);
        istringstream ss(tokens[2]);
        ss >> type >> p;
        float power = stof(p);
        int id = stoi(token);
        // ���������ݲ����Ĳ���
        this->insert(new Node(Point(x,y),new base(Point(x,y),id,power,type)));
    }
    f1.close();
    f2.close();
    t.close();
    InitResult = "Init Success!";
    return true;
}

/*
 * ���������뾶�ں���������нڵ�
 */
void Quad::searchNearbyNodes(Point p, double radius, vector<Node *> &result) {
    // �����ǰ�ڵ�ķ�Χû��������뾶�ཻ���򷵻�
    if (!inBoundary(p, radius)) {
        return;
    }
    // �����ǰ�ڵ�û���ӽڵ㣬�򽫸ýڵ���ӵ������
    if (n != nullptr) {
        double distance = n->pos.distance(p);
        if (distance <= radius) {
            result.push_back(n);
        }
    }
    // �����ǰ�ڵ����ӽڵ㣬��ݹ���������
    if (topLeftTree != nullptr) {
        topLeftTree->searchNearbyNodes(p, radius, result);
        topRightTree->searchNearbyNodes(p, radius, result);
        botLeftTree->searchNearbyNodes(p, radius, result);
        botRightTree->searchNearbyNodes(p, radius, result);
    }
}

void Quad::clear() {
    if (topLeftTree != nullptr) {
        topLeftTree->clear();
        topRightTree->clear();
        botLeftTree->clear();
        botRightTree->clear();
    }
    delete this;
}

Quad::~Quad() {
    clear();
}

base* Quad::findMostPowerfulBase(Point p,double radius) {
    vector<Node*> result;
    searchNearbyNodes(p, radius, result);
    if (result.empty()) {
        return nullptr;
    }
    base *mostPowerfulBase = nullptr;
    for (auto & i : result) {
        if (mostPowerfulBase == nullptr) {
            mostPowerfulBase = i->data;
        } else {
            if (!mostPowerfulBase->isPowerful(i->data,p)) {
                mostPowerfulBase = i->data;
            }
        }
    }
    return mostPowerfulBase;
}

void Quad::showTopLeft() const {
    double x1 = botLeft.x;  // ���߽�
    double y1 = topRight.y; // ���߽�
    double x2 = topRight.x; // ���߽�
    double y2 = botLeft.y;  // �ϱ߽�
    // ѭ���������л�վ�ڵ㣬����������ǵĽڵ���Ϣ
    auto result = getNodesInBoundary(x1, y1, x1 + (x2 - x1) / 2.0, y1 - (y1 - y2) / 2.0);
    cout << "��������" << result.size() << "���ڵ�" << endl;
    for (Node* node : result) {
        cout << "�����ǽڵ㣺" << node->data->id << endl;
    }
}

void Quad::showBotRight() const {
    double x1 = botLeft.x;  // ���߽�
    double y1 = topRight.y; // ���߽�
    double x2 = topRight.x; // ���߽�
    double y2 = botLeft.y;  // �ϱ߽�
    // ѭ���������л�վ�ڵ㣬����ڶ��ϽǵĽڵ���Ϣ
    auto result = getNodesInBoundary(x2 - (x2 - x1) / 2.0, y2 + (y1 - y2) / 2.0, x2, y2);
    cout << "���Ͻ���" << result.size() << "���ڵ�" << endl;
    for (Node* node : result){
        cout << "���Ͻǽڵ㣺" << node->data->id << endl;
    }
}

vector<Node*> Quad::getNodesInBoundary(double x1, double y1, double x2, double y2) const {
    vector<Node*> nodes;
    getNodesInBoundaryHelper(nodes, x1, y1, x2, y2);
    return nodes;
}

void Quad::getNodesInBoundaryHelper(vector<Node*>& nodes, double x1, double y1, double x2, double y2) const
{
//    if (!inBoundary(x1, y1, x2, y2)) {
//        return;
//    }
    if (n != nullptr) {
        if (n->inBoundary(x1, y1, x2, y2)) {
            nodes.push_back(n);
        }
    }
    if (topLeftTree != nullptr) {
        topLeftTree->getNodesInBoundaryHelper(nodes, x1, y1, x2, y2);
    }
    if (topRightTree != nullptr) {
        topRightTree->getNodesInBoundaryHelper(nodes, x1, y1, x2, y2);
    }
    if (botLeftTree != nullptr) {
        botLeftTree->getNodesInBoundaryHelper(nodes, x1, y1, x2, y2);
    }
    if (botRightTree != nullptr) {
        botRightTree->getNodesInBoundaryHelper(nodes, x1, y1, x2, y2);
    }
}

void Quad::showEast() const{
    double x1 = botLeft.x;  // ���߽�
    double y1 = topRight.y; // ���߽�
    double x2 = topRight.x; // ���߽�
    double y2 = botLeft.y;  // �ϱ߽�
    auto result = getNodesInBoundary(x2 - (x2 - x1) / 2.0, y1, x2, y2);
    cout << "������" << result.size() << "���ڵ�" << endl;
    for (Node* node : result){
        cout << "���߽ڵ㣺" << node->data->id << endl;
    }
}
// ��ʾ�ϲ�ڵ�
void Quad::showSouth() const{
    double x1 = botLeft.x;  // ���߽�
    double y1 = topRight.y; // ���߽�
    double x2 = topRight.x; // ���߽�
    double y2 = botLeft.y;  // �ϱ߽�
    auto result = getNodesInBoundary(x1, y2 + (y1 - y2) / 2.0, x2, y2);
    cout << "�ϱ���" << result.size() << "���ڵ�" << endl;
    for (Node* node : result){
        cout << "�ϱ߽ڵ㣺" << node->data->id << endl;
    }
}
