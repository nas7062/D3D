#include "framework.h"

RasterState::RasterState()
{

	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc,sizeof(D3D11_RASTERIZER_DESC));
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_FRONT;
	D3D->GetDevice()->CreateRasterizerState(&desc, &CullFrontFillSolid);
	desc.CullMode = D3D11_CULL_NONE;
	D3D->GetDevice()->CreateRasterizerState(&desc, &CullNoneFillSolid);
	desc.CullMode = D3D11_CULL_BACK;
	D3D->GetDevice()->CreateRasterizerState(&desc, &CullBackFillSolid);
	
}

RasterState::~RasterState()
{
	SafeRelease(CullBackFillSolid);
	SafeRelease(CullFrontFillSolid);
	SafeRelease(CullNoneFillSolid);
}

void RasterState::Set(D3D11_CULL_MODE cull, D3D11_FILL_MODE fill)
{
	if (cull == D3D11_CULL_BACK)
		D3D->GetDC()->RSSetState(CullBackFillSolid);
	else if (cull == D3D11_CULL_FRONT)
		D3D->GetDC()->RSSetState(CullFrontFillSolid);
	else if (cull == D3D11_CULL_NONE)
		D3D->GetDC()->RSSetState(CullNoneFillSolid);
}
