#pragma once
class Mesh
{
    //����(������),������ �մ� Ÿ��(��,��,��),

    //������ ���¸����
    //���������������� ��ǥ(�����ǥ) x,y,z,w
    //��
    //�ؽ�ó ��ǥ (uv)
    friend class GameObject;
    friend class Rain;
    friend class Pop;
private:
    ID3D11Buffer*           vertexBuffer;
    ID3D11Buffer*           indexBuffer;
  
    D3D_PRIMITIVE_TOPOLOGY  primitiveTopology;
    VertexType              vertexType;
    UINT                    byteWidth;

  
public:
    UINT                    vertexCount;
    UINT*                   indices;
    UINT                    indexCount;
    void*                   vertices;
    string                  file;
public:
    Mesh();
    Mesh(void* vertices, UINT vertexCount, UINT* indices,
        UINT indexCount, VertexType type);
    ~Mesh();

    void Set(); // binding
    void LoadFile(string file);
    void SaveFile(string file);
    UINT GetIndexCount() { return indexCount; }
    const Vector3& GetVertexPosition(UINT idx);
    Vector3& SetVertexPosition(UINT idx);
    void UpdateMesh();
};

