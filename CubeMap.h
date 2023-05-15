#pragma once
class CubeMap
{
private:
    static ID3D11Buffer* camBuffer;
    static ID3D11Buffer* PSBuffer;

    UINT width, height;

    ID3D11RenderTargetView* rtv;
    ID3D11DepthStencilView* dsv;


    //쉐이더자원으로 쓸때 인터페이스
    ID3D11ShaderResourceView* rtvSrv;
    //ID3D11ShaderResourceView* dsvSrv;

    //자원
    ID3D11Texture2D* rtvTexture;
    ID3D11Texture2D* dsvTexture;

    struct GSCamBuffer
    {
        Matrix Views[6];
        Matrix Projection;
    } desc;
    struct PSCubeMap
    {
        int CubeMapType = 0;
        float RefractIndex = 1.0f;
        float WaterIndex = 0.01f;
        float CubeMapPadding;
        Matrix Rotation;
    };
    Viewport    viewport;
    Vector3     position;

  
public:
    shared_ptr<Texture> water;
    PSCubeMap desc2;
    Vector3   rot;
  
    static void CreateStaticMember();
    static void DeleteStaticMember();


    CubeMap(UINT width = 256, UINT height = 256);
    ~CubeMap();

    void SetRenderTarget(Vector3  position, float fov = PI_DIV2,float zNear = 0.1f, float zFar = 500.0f, Color clear = Color(0, 0, 0, 1));

    void Set();

    ID3D11ShaderResourceView* GetRTVSRV() { return rtvSrv; }
    //ID3D11ShaderResourceView* GetDSVSRV() { return dsvSrv; }
    ID3D11RenderTargetView* GetRTV() { return rtv; }
    ID3D11DepthStencilView* GetDSV() { return dsv; }

    //창사이즈 조절
    void ResizeScreen(float width, float height);

    //창사이즈 조절
    void CreateBackBuffer(float width, float height);
    void DeleteBackBuffer();

    void RenderDetail();
};

