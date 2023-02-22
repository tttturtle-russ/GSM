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
//    // �����ǰ�Ĳ���Ϊ�գ��򽫽ڵ���뵽��ǰ�ڵ�
//    if (n == nullptr) {
//        n = node;
//        return;
//    }
//    // �����ǰ�Ĳ�����Ϊ�գ�����Χ��Ϊ1���㣬�򴴽��ĸ�����
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
//    // ���뵽������
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
// ��鵱ǰ�ڵ��Ƿ��� Quadtree �ķ�Χ��
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
    // �������һ�к����һ��
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
        this->insert(new Node(Point(x,y),new base(x,y,id,power,type)));
    }
//    f1.close();
//    f2.close();
    f.close();
    InitResult = "Init Success!";
    return true;
}

void Quad::searchTopLeft(vector<Node*> &result) {
    if (n != nullptr) {
        result.push_back(n);
    }
    if (topLeftTree != nullptr) {
        topLeftTree->searchTopLeft(result);
        topRightTree->searchTopLeft(result);
        botLeftTree->searchTopLeft(result);
        botRightTree->searchTopLeft(result);
    }
}

vector<Node*> Quad::searchNearbyNodes(Point p, double radius) {
    vector<Node*> result;

    if (!inBoundary(p)) {
        // ����㲻�� Quadtree ��Χ�ڣ����ؿս������
        return result;
    }

    if (n != nullptr) {
        // �����ǰ�ڵ����һ���ڵ㣬����ýڵ��Ƿ��ڰ뾶�ڣ�����ڣ�����ӵ����������
        double distance = n->pos.distance(p);
        if (distance <= radius) {
            result.push_back(n);
        }
    }

    if (topLeftTree != nullptr) {
        // �����ǰ�ڵ����ӽڵ㣬��ݹ�����ӽڵ�� searchNearbyNodes ����
        vector<Node*> topLeftResult = topLeftTree->searchNearbyNodes(p, radius);
        vector<Node*> topRightResult = topRightTree->searchNearbyNodes(p, radius);
        vector<Node*> botLeftResult = botLeftTree->searchNearbyNodes(p, radius);
        vector<Node*> botRightResult = botRightTree->searchNearbyNodes(p, radius);

        // �������ӽڵ�Ľ�������ϲ�
        result.insert(result.end(), topLeftResult.begin(), topLeftResult.end());
        result.insert(result.end(), topRightResult.begin(), topRightResult.end());
        result.insert(result.end(), botLeftResult.begin(), botLeftResult.end());
        result.insert(result.end(), botRightResult.begin(), botRightResult.end());
    }

    return result;
}