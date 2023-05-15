#include "Framework.h"

RenderTarget::RenderTarget(UINT width, UINT height)
    : width(width), height(height)
{

    HRESULT hr;
    {//Create RTV Texture
        D3D11_TEXTURE2D_DESC desc = { 0 };
        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

        hr = D3D->GetDevice()->CreateTexture2D(&desc, nullptr, &rtvTexture);
        Check(hr);
    }

    {//Create RTV
        D3D11_RENDER_TARGET_VIEW_DESC desc = {};
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

        hr = D3D->GetDevice()->CreateRenderTargetView(rtvTexture, &desc, &rtv);
        Check(hr);
    }

    {//Create SRV
        D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        desc.Texture2D.MipLevels = 1;

        hr = D3D->GetDevice()->CreateShaderResourceView(rtvTexture, &desc, &rtvSrv);
        Check(hr);
    }
    {//Create DSV Texture
        D3D11_TEXTURE2D_DESC desc;
        ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
        desc.Width = this->width;
        desc.Height = this->height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R32_TYPELESS;
        desc.SampleDesc.Count = 1;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
        hr = D3D->GetDevice()->CreateTexture2D(&desc, nullptr, &dsvTexture);
        Check(hr);
    }

    {//Create DSV
        D3D11_DEPTH_STENCIL_VIEW_DESC desc;
        ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
        desc.Format = DXGI_FORMAT_D32_FLOAT;
        desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        desc.Texture2D.MipSlice = 0;

        hr = D3D->GetDevice()->CreateDepthStencilView(dsvTexture, &desc, &dsv);
        Check(hr);
    }

    //Create SRV2
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC desc;
        ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
        desc.Format = DXGI_FORMAT_R32_FLOAT;
        desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        desc.Texture2D.MipLevels = 1;
        hr = D3D->GetDevice()->CreateShaderResourceView(dsvTexture, &desc, &dsvSrv);
        Check(hr);
    }
}

RenderTarget::~RenderTarget()
{
    SafeRelease(rtvTexture);
    SafeRelease(dsvTexture);
    SafeRelease(dsvSrv);
    SafeRelease(rtvSrv);
    SafeRelease(rtv);
    SafeRelease(dsv);
    //SAFE_DELETE(VP);
}

void RenderTarget::Set(Color clear)
{

    D3D->SetRenderTarget(rtv, dsv);
    D3D->Clear(clear, rtv, dsv);

}

void RenderTarget::ResizeScreen(float width, float height)
{
    if (width < 1 || height < 1)
        return;

    this->width = width;
    this->height = height;

    DeleteBackBuffer();

    CreateBackBuffer(width, height);
}

void RenderTarget::CreateBackBuffer(float width, float height)
{
    HRESULT hr;
    {//Create RTV Texture
        D3D11_TEXTURE2D_DESC desc = {};
        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

        hr = D3D->GetDevice()->CreateTexture2D(&desc, nullptr, &rtvTexture);
        Check(hr);
    }

    {//Create RTV
        D3D11_RENDER_TARGET_VIEW_DESC desc = {};
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

        hr = D3D->GetDevice()->CreateRenderTargetView(rtvTexture, &desc, &rtv);
        Check(hr);
    }

    {//Create SRV
        D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        desc.Texture2D.MipLevels = 1;

        hr = D3D->GetDevice()->CreateShaderResourceView(rtvTexture, &desc, &rtvSrv);
        Check(hr);
    }


    //rtvTexture -> rtv , srv µÎ°³¸¦ ¸¸µê

    {//Create DSV Texture
        D3D11_TEXTURE2D_DESC desc;
        ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
        desc.Width = this->width;
        desc.Height = this->height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R32_TYPELESS;
        desc.SampleDesc.Count = 1;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
        hr = D3D->GetDevice()->CreateTexture2D(&desc, nullptr, &dsvTexture);
        Check(hr);
    }

    {//Create DSV
        D3D11_DEPTH_STENCIL_VIEW_DESC desc;
        ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
        desc.Format = DXGI_FORMAT_D32_FLOAT;
        desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        desc.Texture2D.MipSlice = 0;

        hr = D3D->GetDevice()->CreateDepthStencilView(dsvTexture, &desc, &dsv);
        Check(hr);
    }


    //Create SRV2
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC desc;
        ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
        desc.Format = DXGI_FORMAT_R32_FLOAT;
        desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        desc.Texture2D.MipLevels = 1;
        hr = D3D->GetDevice()->CreateShaderResourceView(dsvTexture, &desc, &dsvSrv);
        Check(hr);
    }
}

void RenderTarget::DeleteBackBuffer()
{
    SafeRelease(rtvTexture);
    SafeRelease(dsvTexture);
    SafeRelease(dsvSrv);
    SafeRelease(rtvSrv);
    SafeRelease(rtv);
    SafeRelease(dsv);
}
