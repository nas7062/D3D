#include "framework.h"
ID3D11Buffer* CubeMap::camBuffer = nullptr;
ID3D11Buffer* CubeMap::PSBuffer = nullptr;
void CubeMap::CreateStaticMember()
{
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.ByteWidth = sizeof(GSCamBuffer);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수버퍼
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, NULL, &camBuffer);
		assert(SUCCEEDED(hr));

	}
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.ByteWidth = sizeof(PSCubeMap);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수버퍼
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, NULL, &PSBuffer);
		assert(SUCCEEDED(hr));

	}
}

void CubeMap::DeleteStaticMember()
{
	SafeRelease(camBuffer);
	SafeRelease(PSBuffer);
}

CubeMap::CubeMap(UINT width, UINT height)
{
	water = RESOURCE->textures.Load("Water.dds");

	DXGI_FORMAT rtvFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	//Create Texture2D - RTV
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = width;
		desc.Height = height;
		desc.ArraySize = 6;
		desc.Format = rtvFormat;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;

		Check(D3D->GetDevice()->CreateTexture2D(&desc, NULL, &rtvTexture));
	}

	//Create RTV
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		desc.Format = rtvFormat;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.ArraySize = 6;

		Check(D3D->GetDevice()->CreateRenderTargetView(rtvTexture, &desc, &rtv));
	}

	//Create SRV
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		desc.Format = rtvFormat;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		desc.TextureCube.MipLevels = 1;

		Check(D3D->GetDevice()->CreateShaderResourceView(rtvTexture, &desc, &rtvSrv));
	}


	DXGI_FORMAT dsvFormat = DXGI_FORMAT_D32_FLOAT;
	//Create Texture - DSV
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = width;
		desc.Height = height;
		desc.ArraySize = 6;
		desc.Format = dsvFormat;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;

		Check(D3D->GetDevice()->CreateTexture2D(&desc, NULL, &dsvTexture));
	}

	//CreateDSV
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		desc.Format = dsvFormat;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.ArraySize = 6;

		Check(D3D->GetDevice()->CreateDepthStencilView(dsvTexture, &desc, &dsv));
	}

	//Create SRV2
	/*{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		desc.Format = DXGI_FORMAT_R32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		desc.TextureCube.MipLevels = 1;
		Check(D3D->GetDevice()->CreateShaderResourceView(dsvTexture, &desc, &dsvSrv));
	}*/

	viewport.width = (float)width;
	viewport.height = (float)height;
	this->width = (float)width;
	this->height = (float)height;
}

CubeMap::~CubeMap()
{
	SafeRelease(rtvTexture);
	SafeRelease(dsvTexture);
	//SafeRelease(dsvSrv);
	SafeRelease(rtvSrv);
	SafeRelease(rtv);
	SafeRelease(dsv);
}

void CubeMap::SetRenderTarget(Vector3 position, float fov,float zNear, float zFar, Color clear)
{

	
	this->position = position;

	//Create Views
	{
		float x = position.x;
		float y = position.y;
		float z = position.z;

		struct LookAt
		{
			Vector3 LookAt;
			Vector3 Up;
		} lookAt[6];

		lookAt[0] = { Vector3(x + 1.0f, y, z), Vector3(0, 1.0f, 0) };
		lookAt[1] = { Vector3(x - 1.0f, y, z), Vector3(0, 1.0f, 0) };
		lookAt[2] = { Vector3(x, y + 1.0f, z), Vector3(0, 0, -1.0f) };
		lookAt[3] = { Vector3(x, y - 1.0f, z), Vector3(0, 0, +1.0f) };
		lookAt[4] = { Vector3(x, y, z + 1.0f), Vector3(0, 1.0f, 0) };
		lookAt[5] = { Vector3(x, y, z - 1.0f), Vector3(0, 1.0f, 0) };

		for (UINT i = 0; i < 6; i++)
		{
			desc.Views[i] =
				Matrix::CreateLookAt(position, lookAt[i].LookAt, lookAt[i].Up);
			desc.Views[i] = desc.Views[i].Transpose();
		}
	}
	desc.Projection=
	Matrix::CreatePerspectiveFieldOfView(fov, width / height, zNear, zFar);

	desc.Projection = desc.Projection.Transpose();

	D3D->GetDC()->RSSetViewports(1, viewport.Get11());
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		D3D->GetDC()->Map(camBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy_s(mappedResource.pData, sizeof(GSCamBuffer), &desc, sizeof(GSCamBuffer));
		D3D->GetDC()->Unmap(camBuffer, 0);
		D3D->GetDC()->GSSetConstantBuffers(4, 1, &camBuffer);
	}
	D3D->SetRenderTarget(rtv, dsv);
	D3D->Clear(clear, rtv, dsv);

}

void CubeMap::Set()
{

	desc2.Rotation = Matrix::CreateFromYawPitchRoll(rot.y, rot.x, rot.z);
	desc2.Rotation = desc2.Rotation.Transpose();
	water->Set(6);
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		D3D->GetDC()->Map(PSBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		memcpy_s(mappedResource.pData, sizeof(PSCubeMap), &desc2, sizeof(PSCubeMap));
		D3D->GetDC()->Unmap(PSBuffer, 0);
		D3D->GetDC()->PSSetConstantBuffers(4, 1, &PSBuffer);
	}
}

void CubeMap::ResizeScreen(float width, float height)
{
	if (width < 1 || height < 1)
		return;

	this->width = width;
	this->height = height;

	DeleteBackBuffer();

	CreateBackBuffer(width, height);
}

void CubeMap::CreateBackBuffer(float width, float height)
{
	DXGI_FORMAT rtvFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	//Create Texture2D - RTV
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = width;
		desc.Height = height;
		desc.ArraySize = 6;
		desc.Format = rtvFormat;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;

		Check(D3D->GetDevice()->CreateTexture2D(&desc, NULL, &rtvTexture));
	}

	//Create RTV
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		desc.Format = rtvFormat;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.ArraySize = 6;

		Check(D3D->GetDevice()->CreateRenderTargetView(rtvTexture, &desc, &rtv));
	}

	//Create SRV
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		desc.Format = rtvFormat;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		desc.TextureCube.MipLevels = 1;

		Check(D3D->GetDevice()->CreateShaderResourceView(rtvTexture, &desc, &rtvSrv));
	}


	DXGI_FORMAT dsvFormat = DXGI_FORMAT_D32_FLOAT;
	//Create Texture - DSV
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = width;
		desc.Height = height;
		desc.ArraySize = 6;
		desc.Format = dsvFormat;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;

		Check(D3D->GetDevice()->CreateTexture2D(&desc, NULL, &dsvTexture));
	}

	//CreateDSV
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		desc.Format = dsvFormat;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.ArraySize = 6;

		Check(D3D->GetDevice()->CreateDepthStencilView(dsvTexture, &desc, &dsv));
	}

	//Create SRV2
	/*{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		desc.Format = DXGI_FORMAT_R32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		desc.TextureCube.MipLevels = 1;
		Check(D3D->GetDevice()->CreateShaderResourceView(dsvTexture, &desc, &dsvSrv));
	}*/

	viewport.width = (float)width;
	viewport.height = (float)height;
	this->width = (float)width;
	this->height = (float)height;
}

void CubeMap::DeleteBackBuffer()
{
	SafeRelease(rtvTexture);
	SafeRelease(dsvTexture);
	//SafeRelease(dsvSrv);
	SafeRelease(rtvSrv);
	SafeRelease(rtv);
	SafeRelease(dsv);
}

void CubeMap::RenderDetail()
{
	ImGui::SliderInt("MappingType", &desc2.CubeMapType, 0, 2);
	ImGui::SliderFloat("RefractIndex", &desc2.RefractIndex, 0, 2);
	ImGui::SliderFloat("WaterIndex", &desc2.WaterIndex, 0, 1);
	ImGui::SliderAngle("rotationX", &rot.x);
	ImGui::SliderAngle("rotationY", &rot.y);
	ImGui::SliderAngle("rotationZ", &rot.z);
}
