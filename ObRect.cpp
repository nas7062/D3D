#include "framework.h"

ObRect::ObRect()
{
    VertexP			vertex[4];
   vertex[0].position.x = -0.5f;
   vertex[0].position.y = -0.5f;
   vertex[0].position.z = 0.0f;

   vertex[1].position.x = -0.5f;
   vertex[1].position.y = 0.5f;
   vertex[1].position.z = 0.0f;

   vertex[2].position.x = 0.5f;
   vertex[2].position.y = -0.5f;
   vertex[2].position.z = 0.0f;

   vertex[3].position.x = 0.5f;
   vertex[3].position.y = 0.5f;
   vertex[3].position.z = 0.0f;

    //CreateVertexBuffer
    {
        D3D11_BUFFER_DESC desc;
        desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = sizeof(VertexP) * 4;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA data = { 0 };
        data.pSysMem = vertex;
        HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
        Check(hr);
    }
    /*mesh = new Mesh();
    mesh->LoadFile();*/

    shader = new Shader();
    shader->LoadFile("0.Exam.hlsl", VertexType::P);
}

ObRect::~ObRect()
{
    SafeDelete(shader);
    SafeRelease(vertexBuffer);
}

void ObRect::Render()
{

    //정점의 첫위치는 고정

    //정점이 이동된위치를 받아줄 지역변수

    if (visible)
    {
        GameObject::Render();
        shader->Set(); 
        UINT stride = sizeof(VertexP);
        UINT offset = 0;
        D3D->GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
        D3D->GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
        D3D->GetDC()->Draw(4, 0);

        //Vector3 TransfomVertex[4];

        //for (int i = 0; i < 4; i++)
        //{
        //    // 이동된좌표  = 버텍스 * W;
        //    // 이동된좌표  = 버텍스 * S*R*T;
        //    TransfomVertex[i] = Vector3::Transform(vertex[i], W);
        //    TransfomVertex[i] = Vector3::Transform(TransfomVertex[i], cam->view);
        //    TransfomVertex[i] = Vector3::Transform(TransfomVertex[i], cam->proj);

        //    float w = TransfomVertex[i].x * cam->proj._14 + TransfomVertex[i].y * cam->proj._24
        //        + TransfomVertex[i].z * cam->proj._34 + 1.0f * cam->proj._44;

        //    TransfomVertex[i].x /= w;
        //    TransfomVertex[i].y /= w;
        //    TransfomVertex[i].z /= w;

        //    TransfomVertex[i] = Vector3::Transform(TransfomVertex[i], cam->viewport);


        //}
        ////0->1->2->3->0

        //MoveToEx(g_MemDC, TransfomVertex[0].x, TransfomVertex[0].y, NULL);
        //LineTo(g_MemDC, TransfomVertex[1].x, TransfomVertex[1].y);
        //LineTo(g_MemDC, TransfomVertex[2].x, TransfomVertex[2].y);
        //LineTo(g_MemDC, TransfomVertex[3].x, TransfomVertex[3].y);
        //LineTo(g_MemDC, TransfomVertex[0].x, TransfomVertex[0].y);
    }
   

    for (int i = 0; i < children.size(); i++)
    {
        children[i]->Render();
    }

}
