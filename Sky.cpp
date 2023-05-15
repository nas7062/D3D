#include "framework.h"

Sky* Sky::Create(string name)
{
	Sky* temp = new Sky();
	temp->name = name;
	//temp->type = ObType::UI;

	temp->mesh = RESOURCE->meshes.Load("0.Cube.mesh");
	temp->shader = RESOURCE->shaders.Load("0.Exam.hlsl");

	temp->texCube = make_shared<Texture>();
	temp->texCube->LoadFile("sky.dds");
	//temp->texCube->LoadFile("sky.dds");

	return temp;
}


void Sky::Render()
{
	DEPTH->Set(false);
	BLEND->Set(true);
	RASTER->Set(D3D11_CULL_FRONT);
	texCube->Set(4);
	Actor::Render();
	RASTER->Set(D3D11_CULL_BACK);
	DEPTH->Set(true);
	BLEND->Set(false);
}

void Sky::CubeMapRender()
{
	DEPTH->Set(false);
	BLEND->Set(true);
	RASTER->Set(D3D11_CULL_FRONT);
	texCube->Set(4);
	Actor::CubeMapRender();
	RASTER->Set(D3D11_CULL_BACK);
	DEPTH->Set(true);
	BLEND->Set(false);
}