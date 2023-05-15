#include "framework.h"

Billboard* Billboard::Create(string name)
{
	Billboard* temp = new Billboard();
	temp->name = name;
	temp->type = ObType::Billboard;
	temp->mesh = RESOURCE->meshes.Load("9.Billboard.mesh");
	temp->shader = RESOURCE->shaders.Load("9.Cube.hlsl");
	return temp;
}

void Billboard::Update()
{
	Actor::Update();
}

void Billboard::Render()
{
	//

	//DEPTH->Set(false);
	VertexPSP* vertex = (VertexPSP*)mesh->vertices;
	vertex[0].size = Vector2(S._11, S._22);
	vertex[0].pivot = Vector2(parent->S._11, parent->S._22);
	mesh->UpdateMesh();
	Actor::Render();

	//DEPTH->Set(true);
}
