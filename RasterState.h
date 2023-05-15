#pragma once
class RasterState :public Singleton<RasterState>
{
private:
	ID3D11RasterizerState* CullBackFillSolid;
	ID3D11RasterizerState* CullFrontFillSolid;
	ID3D11RasterizerState* CullNoneFillSolid;


public:
	RasterState();
	~RasterState();
	void Set(D3D11_CULL_MODE cull = D3D11_CULL_BACK, 
		D3D11_FILL_MODE fill = D3D11_FILL_SOLID);
};

