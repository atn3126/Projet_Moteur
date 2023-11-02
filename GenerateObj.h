#pragma once
#include "d3dApp.h"
#include "MathHelper.h"
#include "UploadBuffer.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;
struct RenderItem {
    RenderItem() = default;
    XMFLOAT4X4 World = MathHelper::Identity4x4();
    UINT ObjCBIndex = -1;

    MeshGeometry* Geo = nullptr;

    D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    UINT IndexCount = 0;
    UINT StartIndexLocation = 0;
    int BaseVertexLocation = 0;
};
struct ObjectConstants
{
    XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
    XMFLOAT3 translation;
};

class GenerateObj {

public:
    std::unique_ptr<MeshGeometry> GetmBoxGeo();
    void Initialize(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>cmdList, Microsoft::WRL::ComPtr <ID3D12Device> Device);
    void Update();
private:
    XMFLOAT4 CubePos;
    XMFLOAT4X4 CubeRotMat;
    std::unique_ptr<MeshGeometry> mBoxGeo = nullptr;

};