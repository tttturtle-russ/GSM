//
// Created by liuhy on 2023/3/1.
//

#include "QuadTree.h"
#include "util.h"

void QuadTree::extendTask2() const {
    checkOverlap(*mobileInfo[2]);
    checkOverlap(*mobileInfo[5]);
}

void QuadTree::showResult() const {
    for (const auto& item: mobileInfo)
        checkMove(*item);
}


Point *QuadTree::findNearestPoint(const Point &p) const {
    auto v = searchNearbyPoints(p);
    cout << "附近的点有"<<v.size() <<"个"<< endl;
    return p.getMinDistancePoint(v);
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
            // 四个子叶子节点中第一个有值的节点
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
            // 四个子叶子节点中第一个有值的节点
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

