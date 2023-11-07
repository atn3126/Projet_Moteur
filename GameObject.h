#pragma once
#include "MathHelper.h"
#include "UploadBuffer.h"
#include "CreateGeometry.h"
#include "Transform.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

struct ObjectConstants
{
	XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
};

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

struct RenderItem : Transform {
	RenderItem() = default;
	XMFLOAT4X4 World = MathHelper::Identity4x4();

	// Dirty flag indicating the object data has changed and we need to update the constant buffer.
	// Because we have an object cbuffer for each FrameResource, we have to apply the
	// update to each FrameResource.  Thus, when we modify obect data we should set 
	// NumFramesDirty = gNumFrameResources so that each frame resource gets the update.
	// Index into GPU constant buffer corresponding to the ObjectCB for this render item.
	UINT ObjCBIndex = -1;

	MeshGeometry* Geo = nullptr;

	// Primitive topology.
	D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;
	// DrawIndexedInstanced parameters.
	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	int BaseVertexLocation = 0;

};


class GameObject {
public:
	GameObject();
	~GameObject();
	void Init(ComPtr<ID3D12GraphicsCommandList> cmdList, ComPtr<ID3D12Device> device);
	void BuildRenderOpBox(ComPtr<ID3D12Device> device);
	void BuildRenderPyramideBox(ComPtr<ID3D12Device> device);
	void BuildRenderOpCircle(ComPtr<ID3D12Device> device);
	const std::vector<RenderItem*>& GetOpaqueItems();
	std::vector<std::unique_ptr<RenderItem>>& GetAllItems();
	void BuildObjectConstantBuffers(ComPtr<ID3D12Device> device, std::unique_ptr<RenderItem>* sphere);

private:
	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries;
	UINT ObjIndex = 0;

	//Stock RenderItem
	std::vector<std::unique_ptr<RenderItem>> mAllRitems;

	std::vector<RenderItem*> mOpaqueRitems;
	std::vector<RenderItem*> mTransparentRitems;
	UINT mPassCbvOffset = 0;
	std::unique_ptr<MeshGeometry> mBoxGeo = nullptr;

};