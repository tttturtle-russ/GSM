//
// Created by liuhy on 2023/2/21.
//

#include "QuadTree.h"
string InitResult;
void Quad::insert(Node *node)
{
//    if (node == nullptr)
//        return;
//
//    // Current quad cannot contain it
//    if (!inBoundary(node->pos))
//        return;
//
//    // We are at a quad of unit area
//    // We cannot subdivide this quad further
//    if (abs(botLeft.x - topRight.x) <= 1 && abs(botLeft.y - topRight.y) <= 1)
//    {
//        if (this->n == nullptr)
//            n = node;
//        return;
//    }
//
//    if ((botLeft.x + topRight.x) / 2 >= node->pos.x)
//    {
//        // Indicates topLeftTree
//        if ((botLeft.y + topRight.y) / 2 >= node->pos.y)
//        {
//            if (this->topLeftTree == nullptr)
//                topLeftTree = new Quad(
//                        Point(botLeft.x, botLeft.y),
//                        Point((botLeft.x + topRight.x) / 2,
//                              (botLeft.y + topRight.y) / 2));
//            topLeftTree->insert(node);
//        }
//
//            // Indicates botLeftTree
//        else
//        {
//            if (this->botLeftTree == nullptr)
//                botLeftTree = new Quad(
//                        Point(botLeft.x,
//                              (botLeft.y + topRight.y) / 2),
//                        Point((botLeft.x + topRight.x) / 2,
//                              topRight.y));
//            botLeftTree->insert(node);
//        }
//    }
//    else
//    {
//        // Indicates topRightTree
//        if ((botLeft.y + topRight.y) / 2 >= node->pos.y)
//        {
//            if (this->topRightTree == nullptr)
//                topRightTree = new Quad(
//                        Point((botLeft.x + topRight.x) / 2,
//                              botLeft.y),
//                        Point(topRight.x,
//                              (botLeft.y + topRight.y) / 2));
//            topRightTree->insert(node);
//        }
//
//            // Indicates botRightTree
//        else
//        {
//            if (this->botRightTree == nullptr)
//                botRightTree = new Quad(
//                        Point((botLeft.x + topRight.x) / 2,
//                              (botLeft.y + topRight.y) / 2),
//                        Point(topRight.x, topRight.y));
//            botRightTree->insert(node);
//        }
//    }
//    if (!inBoundary(node->pos)) {
//        return;
//    }
//    // 如果当前四叉树为空，则将节点插入到当前节点
//    if (n == nullptr) {
//        n = node;
//        return;
//    }
//    // 如果当前四叉树不为空，但范围仅为1个点，则创建四个子树
//    if (botLeft == topRight) {
//        if (topLeftTree == nullptr) {
//            topLeftTree = new Quad(Point(botLeft.x, botLeft.y), Point((botLeft.x + topRight.x) / 2, (botLeft.y + topRight.y) / 2));
//        }
//        if (topRightTree == nullptr) {
//            topRightTree = new Quad(Point((botLeft.x + topRight.x) / 2 + 1, botLeft.y), Point(topRight.x, (botLeft.y + topRight.y) / 2));
//        }
//        if (botLeftTree == nullptr) {
//            botLeftTree = new Quad(Point(botLeft.x, (botLeft.y + topRight.y) / 2 + 1), Point((botLeft.x + topRight.x) / 2, topRight.y));
//        }
//        if (botRightTree == nullptr) {
//            botRightTree = new Quad(Point((botLeft.x + topRight.x) / 2 + 1, (botLeft.y + topRight.y) / 2 + 1), Point(topRight.x, topRight.y));
//        }
//    }
//    // 插入到子树中
//    if (node->pos.x <= (botLeft.x + topRight.x) / 2) {
//        if (node->pos.y <= (botLeft.y + topRight.y) / 2) {
//            if (topLeftTree == nullptr) {
//                topLeftTree = new Quad(Point(botLeft.x, botLeft.y), Point((botLeft.x + topRight.x) / 2, (botLeft.y + topRight.y) / 2));
//            }
//            topLeftTree->insert(node);
//        } else {
//            if (botLeftTree == nullptr) {
//                botLeftTree = new Quad(Point(botLeft.x, (botLeft.y + topRight.y) / 2 + 1), Point((botLeft.x + topRight.x) / 2, topRight.y));
//            }
//            botLeftTree->insert(node);
//        }
//    } else {
//        if (node->pos.y <= (botLeft.y + topRight.y) / 2) {
//            if (topRightTree == nullptr) {
//                topRightTree = new Quad(Point((botLeft.x + topRight.x) / 2 + 1, botLeft.y), Point(topRight.x, (botLeft.y + topRight.y) / 2));
//            }
//            topRightTree->insert(node);
//        } else {
//            if (botRightTree == nullptr) {
//                botRightTree = new Quad(Point((botLeft.x + topRight.x) / 2 + 1, (botLeft.y + topRight.y) / 2 + 1), Point(topRight.x, topRight.y));
//            }
//            botRightTree->insert(node);
//        }
//    }
// 检查当前节点是否在 Quadtree 的范围内
    // 如果节点不在当前节点的范围内，则返回
    if (!inBoundary(node->pos)) {
        return;
    }
    // 如果当前节点没有子节点，则将该节点作为当前节点的子节点
    if (n == nullptr) {
        n = node;
        return;
    }
    // 如果当前节点有子节点，则将该节点插入到适当的子节点中
    if (topLeftTree == nullptr) {
        subdivide();
    }
    topLeftTree->insert(node);
    topRightTree->insert(node);
    botLeftTree->insert(node);
    botRightTree->insert(node);
}

void Quad::subdivide() {
    Point mid = Point((botLeft.x + topRight.x)/2, (topRight.y + botLeft.y)/2);
    topLeftTree = new Quad(botLeft, mid);
    topRightTree = new Quad(Point(mid.x, botLeft.y), Point(topRight.x, mid.y));
    botLeftTree = new Quad(Point(botLeft.x, mid.y), Point(mid.x, topRight.y));
    botRightTree = new Quad(mid, topRight);
}

Node* Quad::search(Point p)
{
    // Current quad cannot contain it
    if (!inBoundary(p))
        return nullptr;

    // We are at a quad of unit length
    // We cannot subdivide this quad further
    if (this->n != nullptr)
        return n;

    if ((botLeft.x + topRight.x) / 2 >= p.x)
    {
        // Indicates topLeftTree
        if ((botLeft.y + topRight.y) / 2 >= p.y)
        {
            if (this->topLeftTree == nullptr)
                return nullptr;
            return topLeftTree->search(p);
        }

            // Indicates botLeftTree
        else
        {
            if (this->botLeftTree == nullptr)
                return nullptr;
            return botLeftTree->search(p);
        }
    }
    else
    {
        // Indicates topRightTree
        if ((botLeft.y + topRight.y) / 2 >= p.y)
        {
            if (this->topRightTree == nullptr)
                return nullptr;
            return topRightTree->search(p);
        }

            // Indicates botRightTree
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
    // read from ./data/*.txt and store them to QuadTree
//    ifstream f1 ("./data/jz001.txt", ios::in);
//    ifstream f2 ("./data/jz002.txt", ios::in);
//    if (!f1.is_open()|| !f2.is_open()) {
//        InitResult = "Error: cannot open file";
//        return false;
//    }
    ifstream f("./data/test.txt", ios::in);
    if (!f.is_open()){
        InitResult = "Error: cannot open file";
        return false;
    }
    // read jz001.txt
    string buf;
    vector<string > v;
//    getline(f1,buf);
//    while (getline(f1,buf)) {
//        v.push_back(buf);
//    }
//    v.pop_back();
//    getline(f2,buf);
//    while (getline(f2,buf)) {
//        v.push_back(buf);
//    }
//    v.pop_back();
    getline(f,buf);
    while (getline(f,buf)) {
        v.push_back(buf);
    }
    v.pop_back();
    cout << v.size() << endl;
    // 不处理第一行和最后一行
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
        // 将上述数据插入四叉树
        this->insert(new Node(Point(x,y),new base(Point(x,y),id,power,type)));
    }
//    f1.close();
//    f2.close();
    f.close();
    InitResult = "Init Success!";
    return true;
}

/*
 * 搜索给定半径内和坐标的所有节点
 */
void Quad::searchNearbyNodes(Point p, double radius, vector<Node *> &result) {
    // 如果当前节点的范围没有与给定半径相交，则返回
    if (!inBoundary(p, radius)) {
        return;
    }
    // 如果当前节点没有子节点，则将该节点添加到结果中
    if (n != nullptr) {
        double distance = n->pos.distance(p);
        if (distance <= radius) {
            result.push_back(n);
        }
    }
    // 如果当前节点有子节点，则递归搜索它们
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

base* Quad::findMostPowerfulBase(Point p, double radius) {
    vector<Node *> result;
    searchNearbyNodes(p, radius, result);
    if (result.empty()) {
        return nullptr;
    }
    base *mostPowerfulBase = nullptr;
    for (auto & i : result) {
        if (mostPowerfulBase == nullptr) {
            mostPowerfulBase = i->data;
        } else {
            if (i->data->power > mostPowerfulBase->power) {
                mostPowerfulBase = i->data;
            }
        }
    }
    return mostPowerfulBase;
}
