#include "Framework.h"

Shader::Shader():
 vertexLayout(nullptr), vertexShader(nullptr)
, pixelShader(nullptr),geometryShader(nullptr)
{
    
}

Shader::~Shader()
{
    SafeRelease(vertexLayout);
    SafeRelease(vertexShader);
    SafeRelease(pixelShader);
    SafeRelease(geometryShader);
}

void Shader::LoadFile(string file)
{
    this->file = file;
    this->vertextype = (VertexType)(file[0]-48);
    //컴파일결과를 담아놓는 인터페이스
    ID3D10Blob* VsBlob;
    ID3D10Blob* PsBlob;

    wstring path = L"../Shaders/"+ Util::ToWString(file);

    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;


    D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "VS", "vs_5_0", flags, 0, &VsBlob, nullptr);

    D3D->GetDevice()->CreateVertexShader(VsBlob->GetBufferPointer(), VsBlob->GetBufferSize(),
        nullptr, &vertexShader);


    switch (vertextype)
    {
    case VertexType::P:
         D3D->GetDevice()->CreateInputLayout
         (
             VertexP::LayoutDesc,
             1,
             VsBlob->GetBufferPointer(),
             VsBlob->GetBufferSize(),
             &vertexLayout
         );
        break;
    case VertexType::PC:
    
        D3D->GetDevice()->CreateInputLayout
        (
            VertexPC::LayoutDesc,
            2,
            VsBlob->GetBufferPointer(),
            VsBlob->GetBufferSize(),
            &vertexLayout
        );
        break;
    case VertexType::PCN:

        D3D->GetDevice()->CreateInputLayout
        (
            VertexPCN::LayoutDesc,
            3,
            VsBlob->GetBufferPointer(),
            VsBlob->GetBufferSize(),
            &vertexLayout
        );
        break;
    case VertexType::PTN:

        D3D->GetDevice()->CreateInputLayout
        (
            VertexPTN::LayoutDesc,
            3,
            VsBlob->GetBufferPointer(),
            VsBlob->GetBufferSize(),
            &vertexLayout
        );
        break;

    case VertexType::MODEL:

        D3D->GetDevice()->CreateInputLayout
        (
            VertexModel::LayoutDesc,
            6,
            VsBlob->GetBufferPointer(),
            VsBlob->GetBufferSize(),
            &vertexLayout
        );
        break;

    case VertexType::TERRAIN:

        D3D->GetDevice()->CreateInputLayout
        (
            VertexTerrain::LayoutDesc,
            4,
            VsBlob->GetBufferPointer(),
            VsBlob->GetBufferSize(),
            &vertexLayout
        );
        break;
    case VertexType::PT:

        D3D->GetDevice()->CreateInputLayout
        (
            VertexPT::LayoutDesc,
            2,
            VsBlob->GetBufferPointer(),
            VsBlob->GetBufferSize(),
            &vertexLayout
        );
        break;

    case VertexType::PS:

        D3D->GetDevice()->CreateInputLayout
        (
            VertexPS::LayoutDesc,
            2,
            VsBlob->GetBufferPointer(),
            VsBlob->GetBufferSize(),
            &vertexLayout
        );

        LoadGeometry();
        break;
    case VertexType::PSV:

        D3D->GetDevice()->CreateInputLayout
        (
            VertexPSV::LayoutDesc,
            3,
            VsBlob->GetBufferPointer(),
            VsBlob->GetBufferSize(),
            &vertexLayout
        );
        LoadGeometry();
        break;

    case VertexType::PSP:

        D3D->GetDevice()->CreateInputLayout
        (
            VertexPSP::LayoutDesc,
            3,
            VsBlob->GetBufferPointer(),
            VsBlob->GetBufferSize(),
            &vertexLayout
        );
        LoadGeometry();
        break;
    }

    D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "PS", "ps_5_0", flags, 0, &PsBlob, nullptr);

    D3D->GetDevice()->CreatePixelShader(PsBlob->GetBufferPointer(), PsBlob->GetBufferSize(),
        nullptr, &pixelShader);

    SafeRelease(VsBlob);
    SafeRelease(PsBlob);
}


void Shader::LoadGeometry()
{
    SafeRelease(geometryShader);

    ID3D10Blob* GsBlob;

    wstring path = L"../Shaders/" + Util::ToWString(file);

    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;


    D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "GS", "gs_5_0", flags, 0, &GsBlob, nullptr);

    D3D->GetDevice()->CreateGeometryShader(GsBlob->GetBufferPointer(), GsBlob->GetBufferSize(),
        nullptr, &geometryShader);

    SafeRelease(GsBlob);
}

void Shader::Set()
{
    D3D->GetDC()->VSSetShader(vertexShader, 0, 0);
    D3D->GetDC()->PSSetShader(pixelShader, 0, 0);
    D3D->GetDC()->GSSetShader(geometryShader, 0, 0);
    D3D->GetDC()->IASetInputLayout(vertexLayout);
}
