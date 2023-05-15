#pragma once
class RenderTarget
{
private:

    UINT width, height;

    //�ڿ���

    //���� ����Ÿ������ ���� �ʿ��� �������̽�
    ID3D11RenderTargetView* rtv;
    ID3D11DepthStencilView* dsv;


    //���̴��ڿ����� ���� �������̽�
    ID3D11ShaderResourceView* rtvSrv;
    ID3D11ShaderResourceView* dsvSrv;

    //�ڿ�
    ID3D11Texture2D* rtvTexture;
    ID3D11Texture2D* dsvTexture;

public:
    RenderTarget(UINT width = App.GetWidth() ,
        UINT height = App.GetHeight());
    ~RenderTarget();

    void Set(Color clear = Color(0, 0, 0, 1));

    ID3D11ShaderResourceView* GetRTVSRV() { return rtvSrv; }
    ID3D11ShaderResourceView* GetDSVSRV() { return dsvSrv; }
    ID3D11RenderTargetView* GetRTV() { return rtv; }
    ID3D11DepthStencilView* GetDSV() { return dsv; }

    //â������ ����
    void ResizeScreen(float width, float height);

    //â������ ����
    void CreateBackBuffer(float width, float height);
    void DeleteBackBuffer();
};

