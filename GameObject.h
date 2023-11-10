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
	std::string Type;
	MeshGeometry* Geo = nullptr;
	bool gameOver;
	bool RemoveAsteroid;
	bool dontMove;
	std::vector<RenderItem*> CheckCollision(std::vector<RenderItem*> mOpaqueList,size_t offset) {
		XMMATRIX translation = XMLoadFloat4x4(&World);
		if (Type == "asteroid") {
			XMMATRIX temp = XMLoadFloat4x4(&World);
			translation = XMMatrixMultiply(temp, Translate(0, 0, -0.01));
		}
		if (Type == "projectile") {
			XMMATRIX temp = XMLoadFloat4x4(&World);
			translation = XMMatrixMultiply(temp, Translate(0, 0, 0.01));
		}

		for (size_t i = offset+1; i < mOpaqueList.size(); i++)
		{
			if (World._41 <= mOpaqueList[i]->World._41+0.25 && World._41 >= mOpaqueList[i]->World._41 - 0.25 && World._42 <= mOpaqueList[i]->World._42+0.25 && World._42 >= mOpaqueList[i]->World._42 - 0.25 && World._43 <= mOpaqueList[i]->World._43+0.25 && World._43 >= mOpaqueList[i]->World._43 - 0.25) {
				if (Type == "asteroid" && mOpaqueList[i]->Type == "player") {
					gameOver = !gameOver;
					mOpaqueList.clear();
				}
				else if (Type == "player" && mOpaqueList[i]->Type == "asteroid") {
					gameOver = !gameOver;
					mOpaqueList.clear();
				}
				else if (Type == "asteroid" && mOpaqueList[i]->Type == "projectile") {
					RemoveAsteroid = !RemoveAsteroid;
					mOpaqueList.erase(mOpaqueList.begin() + offset);
					mOpaqueList.erase(mOpaqueList.begin() + (i-1));
				}
				else if (Type == "projectile" && mOpaqueList[i]->Type == "asteroid") {
					RemoveAsteroid = !RemoveAsteroid;
					mOpaqueList.erase(mOpaqueList.begin() + (i-1));
					mOpaqueList.erase(mOpaqueList.begin() + offset);
				}
				/*else if (Type == "asteroid" && mOpaqueList[i]->Type == "asteroid") {
					dontMove = !dontMove;
				}*/
			}
		}
		if(!dontMove) {
			XMStoreFloat4x4(&World, translation);
		}
		return mOpaqueList;
	};

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
	Microsoft::WRL::ComPtr<ID3D12Device> m_device;
	void Init(ComPtr<ID3D12GraphicsCommandList> cmdList, ComPtr<ID3D12Device> device);
	void BuildRenderOpBox();
	void BuildRenderOpPyramide();
	void BuildRenderOpProjectile(float playerPosX, float playerPosY, float playerPosZ);
	void BuildRenderOpCircle();
	const std::vector<RenderItem*>& GetOpaqueItems();
	std::vector<std::unique_ptr<RenderItem>>& GetAllItems();
	void SetOpaqueItems(std::vector<RenderItem*> OpaqueItems);
	void BuildObjectConstantBuffers(std::unique_ptr<RenderItem>* sphere);

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