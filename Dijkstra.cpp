#include "framework.h"

void Dijkstra::AddNode(Vector3 pos)
{
	DiNode temp;
	size_t id = NodeList.size();
    //리스트 내에 비어있는 인덱스가 있는지
    for (size_t i = 0; i < NodeList.size(); i++)
    {
        //i가 없는 인덱스
        if (NodeList.find(i) == NodeList.end())
        {
            id = i;
            break;
        }
    }
    temp.id = id;
    temp.pos = pos;
    NodeList[temp.id] = temp;
}

void Dijkstra::PopNode(int id)
{
    //리스트내에 노드가 지울id값이 있다면 지워버리기
    /*for (auto it = NodeList.begin(); it != NodeList.end(); it++)
    {
        if (it->second.linkedWay.find(id) != it->second.linkedWay.end())
            it->second.linkedWay.erase(id);
    }*/

    if (NodeList.find(id) != NodeList.end())
    {
        for (auto it = NodeList[id].linkedWay.begin();
            it != NodeList[id].linkedWay.end(); it++)
        {
            NodeList[it->first].linkedWay.erase(id);
        }

        NodeList.erase(id);
    }
     
}

void Dijkstra::LinkNode(int id1, int id2)
{
    Vector3 Dis = NodeList[id1].pos - NodeList[id2].pos;


    NodeList[id1].linkedWay[id2] = Dis.Length();
    NodeList[id2].linkedWay[id1] = Dis.Length();
}

int Dijkstra::PickNode(Vector3 pos)
{
    int id = 0;
    //전체탐색
    float min = FLT_MAX;
    for (auto it = NodeList.begin(); it != NodeList.end(); it++)
    {
        Vector3 Dis = it->second.pos - pos;
        float len = Dis.Length();
        if (min > len)
        {
            min = len;
            id = it->first;
        }
    }
    return id;
}

bool Dijkstra::PathFinding(deque<Vector3>& Way, int Start, int End)
{
    if (NodeList.find(Start) == NodeList.end() ||
        NodeList.find(End) == NodeList.end())
    {
        return false;
    }


    for (auto it = NodeList.begin(); it != NodeList.end(); it++)
    {
        it->second.Reset();
    }

    priority_queue<DiNode*, vector<DiNode*>, DiNodeCompare > Q;

    NodeList[Start].cost = 0;
    //s , q
   
    Q.push(&NodeList[Start]);
    while (not Q.empty())
    {
        DiNode* top = Q.top();
        Q.pop();
        for (auto it = top->linkedWay.begin();
            it != top->linkedWay.end(); it++)
        {
            //기존노드의 비용 갱신
            if (NodeList[it->first].cost > top->cost + it->second)
            {
                NodeList[it->first].cost = top->cost + it->second;
                NodeList[it->first].prev = top->id;
                if (not NodeList[it->first].find)
                {
                    Q.push(&NodeList[it->first]);
                    NodeList[it->first].find = true;
                }
            }
        }

    }
    //길 추가
    Way.clear();

    DiNode* iter = &NodeList[End];
    while (1)
    {
        //cout << iter->id << "->";
        Way.push_back(iter->pos);

        if (iter->prev == -1)return false;

        iter = &NodeList[iter->prev];
        if (iter == &NodeList[Start])
        {
            Way.push_back(iter->pos);
            //cout << iter->id << endl;
            break;
        }
    }
	return true;
}

void Dijkstra::SaveFile(string file)
{
    this->file = file;
    BinaryWriter out;
    wstring path = L"../Contents/Dijkstra/" + Util::ToWString(file);
    out.Open(path);
    out.UInt(NodeList.size());
    for (auto it = NodeList.begin(); it != NodeList.end(); it++)
    {
        out.UInt(it->second.id);
        out.vector3(it->second.pos);
        out.UInt(it->second.linkedWay.size());
        for (auto it2 = it->second.linkedWay.begin();
            it2 != it->second.linkedWay.end(); it2++)
        {
            out.Int(it2->first);    // id
            out.Float(it2->second); // cost
        }
    }
    out.Close();
}

void Dijkstra::LoadFile(string file)
{
    this->file = file;
    BinaryReader in;
    wstring path = L"../Contents/Dijkstra/" + Util::ToWString(file);
    if (in.Open(path))
    {
        NodeList.clear();
        UINT Listsize = in.UInt();
        for (UINT i = 0; i < Listsize; i++)
        {
            DiNode temp;
            temp.id = in.UInt();
            temp.pos = in.vector3();
            NodeList[temp.id] = temp;
            UINT NodeListsize = in.UInt();
            for (UINT j = 0; j < NodeListsize; j++)
            {
                int id = in.Int();
                float cost = in.Float();
                NodeList[temp.id].linkedWay[id] = cost;
            }
        }
    } in.Close();
   
}
