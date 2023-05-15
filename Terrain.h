#pragma once
class Terrain : public Actor 
{
    friend GameObject;

    //크기를 잡기위한 class
    //삼각형 단위
    struct InputDesc
    {
        UINT index;
        Vector3 v0, v1, v2;
    };
    struct OutputDesc
    {
        int picked;
        float u, v, distance;
    };
    struct RayDesc
    {
        Vector3 position;
        float size;
        Vector3 direction;
        float padding;
    };
	static ID3D11ComputeShader* computeShader;
    static class Actor* Node; //구 모양
    static class Actor* Line; //선 모양
public:
    static void		CreateStaticMember();
    static void		DeleteStaticMember();
    static Terrain* Create(string Name = "Terrain", int	terrainSize = 257, float uvScale = 1.0f);
private:
    //compute Input
    InputDesc* inputArray;
    ID3D11Resource* input;
    ID3D11ShaderResourceView* srv = nullptr;//읽기전용
    //compute Output
    OutputDesc* outputArray;
    ID3D11Resource* output;
    ID3D11UnorderedAccessView* uav;//읽기쓰기 둘다가능
    //copy용
    ID3D11Resource* result;
    //ray
    RayDesc                     ray;
    ID3D11Buffer* rayBuffer;

    Dijkstra                    dijkstra;
    bool                        showNode = true;
public:
    Terrain(string Name);
    float                       uvScale;
    int                         triSize;
    void	        RenderDetail();

    //메쉬 재성성(갯수 변경 등등)
    void            CreateMesh(int Size = 257);
    //재생성 없이 갱신
    void            UpdateMeshNormal();
    void            UpdateMeshUv();

    //CS
    void            CreateStructuredBuffer();
    void            DeleteStructuredBuffer();
    //void          
    bool            ComPutePicking(Ray WRay, OUT Vector3& HitPoint);


    void            AddNode(Vector3 pos);
    void            PopNode(int id);
    void            LinkNode(int id1, int id2);
    //가장 가까운노드 반환
    int             PickNode(Vector3 pos);
    bool            PathFinding(deque<Vector3>& Way, int Start, int End);
    void            Render() override;
    virtual void	CubeMapRender() override;
    bool            RayCastingCollider(Ray WRay, OUT Vector3& HitPoint);
};

