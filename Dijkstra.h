#pragma once
class DiNode // ���
{
public:
    Vector3         pos;
    int             id;
    map<int, float> linkedWay; //����

    //��ã���
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
    //�ð�ȭ�� ������Ʈ

    //����� �Ÿ��θ� ���
 
public:
    string                      file = " ";
    unordered_map<int, DiNode>   NodeList;
    void AddNode(Vector3 pos);
    void PopNode(int id);
    void LinkNode(int id1, int id2);
    //���� ������� ��ȯ
    int PickNode(Vector3 pos);
    bool PathFinding(deque<Vector3>& Way, int Start, int End);
    void SaveFile(string file);
    void LoadFile(string file);
};

