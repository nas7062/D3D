#pragma once
class ObRect :public GameObject
{
	//POINT a;
	
	ID3D11Buffer*	vertexBuffer;
public:
	ObRect();
	~ObRect()override;
	

	void Render() override;
};

