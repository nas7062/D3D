#pragma once
class DiNode // 노드
{
public:
    Vector3         pos;
    int             id;
    map<int, float> linkedWay; //간선

    //길찾기용
    float           cost;
    int             prev;
    bool            find;
    void            Reset()
    {
        prev = -1;
        cost = FLT_MAX;
        find = false;
    }
};

class DiNodeCompare
{
public:
    bool operator()(DiNode*& a, DiNode*& b)
    {
        return a->cost > b->cost;
    }
};

class Dijkstra
{
    //시각화할 오브젝트

    //비용을 거리로만 계산
 
public:
    string                      file = " ";
    unordered_map<int, DiNode>   NodeList;
    void AddNode(Vector3 pos);
    void PopNode(int id);
    void LinkNode(int id1, int id2);
    //가장 가까운노드 반환
    int PickNode(Vector3 pos);
    bool PathFinding(deque<Vector3>& Way, int Start, int End);
    void SaveFile(string file);
    void LoadFile(string file);
};

