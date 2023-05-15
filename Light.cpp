#include "framework.h"

LightManager::LightManager()
{
    {
        D3D11_BUFFER_DESC desc = { 0 };
        desc.ByteWidth = sizeof(DirLight);
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수버퍼
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;
        HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, NULL, &dirLightBuffer);
        assert(SUCCEEDED(hr));
        D3D->GetDC()->PSSetConstantBuffers(2, 1, &dirLightBuffer);
    }

    {
        D3D11_BUFFER_DESC desc = { 0 };
        desc.ByteWidth = sizeof(LightBuffer)*MAX_LIGHT;
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수버퍼
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;
        HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, NULL, &lightBuffer);
        assert(SUCCEEDED(hr));
        D3D->GetDC()->PSSetConstantBuffers(3, 1, &lightBuffer);
    }

    light[0].size = 0;
}

LightManager::~LightManager()
{
    SafeRelease(dirLightBuffer);
    SafeRelease(lightBuffer);
}

void LightManager::Set()
{
    {
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        D3D->GetDC()->Map(dirLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        memcpy_s(mappedResource.pData, sizeof(DirLight), &dirLight, sizeof(DirLight));
        D3D->GetDC()->Unmap(dirLightBuffer, 0);
    }

    {
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        D3D->GetDC()->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        memcpy_s(mappedResource.pData, sizeof(LightBuffer) * MAX_LIGHT, light, sizeof(LightBuffer) * MAX_LIGHT);
        D3D->GetDC()->Unmap(lightBuffer, 0);
    }
}

void LightManager::RenderDetail()
{
    ImGui::SliderFloat3("dirLight", (float*)(&dirLight.direction), -1, 1);
    ImGui::ColorEdit3("dirColor", (float*)(&dirLight.color));
}

void LightManager::SetSize()
{
    int max = -1;
    for (int i = 0; i < MAX_LIGHT; i++)
    {
        if (LIGHT->light[i].isActive)
        {
            if (i > max)
            {
                max = i;
            }
        }
    }
    light[0].size = max + 1;
}

Light* Light::Create(string name)
{
    Light* temp = nullptr;
    for (int i = 0; i < MAX_LIGHT; i++)
    {
        if (not LIGHT->light[i].isActive)
        {
            temp = new Light();
            temp->name = name;
            LIGHT->light[i].isActive = true;
            temp->light = &LIGHT->light[i];
            LIGHT->SetSize();
            temp->mesh = RESOURCE->meshes.Load("1.SphereCollider.mesh");
            temp->shader = RESOURCE->shaders.Load("1.Collider.hlsl");
            temp->type = ObType::Light;
            break;
        }
    }

    return temp;
}

void Light::Release()
{
    light->isActive = false;
    LIGHT->SetSize();
    Actor::Release();
}

void Light::Update()
{
    scale.z = scale.y = scale.x = light->range * 0.5f;
    Actor::Update();
    light->position = GetWorldPos();
}


