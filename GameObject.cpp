#include "GameObject.h"



GameObject::GameObject()
{
}

GameObject::~GameObject()
{

}

void GameObject::Init(ComPtr<ID3D12GraphicsCommandList> cmdList, ComPtr<ID3D12Device> device) {
	CreateGeometry geoGen;
	CreateGeometry::MeshData box = geoGen.CreateBox(.5f, 0.5f, 1.5f, 3);
	CreateGeometry::MeshData sphere = geoGen.CreateSphere(0.5f, 20, 20);
	UINT boxVertexOffset = 0;
	UINT boxIndexOffset = 0;
	UINT sphereVertexOffset = (UINT)box.Vertices.size();
	UINT sphereIndexOffset = (UINT)box.Indices32.size();


	SubmeshGeometry boxSubmesh;
	boxSubmesh.IndexCount = (UINT)box.Indices32.size();
	boxSubmesh.StartIndexLocation = boxIndexOffset;
	boxSubmesh.BaseVertexLocation = boxVertexOffset;

	SubmeshGeometry sphereSubmesh;
	sphereSubmesh.IndexCount = (UINT)sphere.Indices32.size();
	sphereSubmesh.StartIndexLocation = sphereIndexOffset;
	sphereSubmesh.BaseVertexLocation = sphereVertexOffset;

	auto totalVertexCount = box.Vertices.size() + sphere.Vertices.size();
	std::vector<Vertex> vertices(totalVertexCount);
	UINT k = 0;
	for (size_t i = 0; i < box.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box.Vertices[i].Position;
		vertices[k].Color = XMFLOAT4(DirectX::Colors::DarkGreen);
	}
	for (size_t i = 0; i < sphere.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = sphere.Vertices[i].Position;
		vertices[k].Color = XMFLOAT4(DirectX::Colors::Crimson);
	}

	std::vector<std::uint16_t> indices;
	indices.insert(indices.end(),
		std::begin(box.GetIndices16()),
		std::end(box.GetIndices16()));
	indices.insert(indices.end(),
		std::begin(sphere.GetIndices16()),
		std::end(sphere.GetIndices16()));

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);
	auto geo = std::make_unique<MeshGeometry>();
	geo->Name = "shapeGeo";
	ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
	CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(),
		vbByteSize);
	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(),
		ibByteSize);
	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(device.Get(),
		cmdList.Get(), vertices.data(), vbByteSize, geo->VertexBufferUploader);
	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(device.Get(),
		cmdList.Get(), indices.data(), ibByteSize, geo->IndexBufferUploader);
	geo->VertexByteStride = sizeof(Vertex);
	geo->VertexBufferByteSize = vbByteSize;
	geo->IndexFormat = DXGI_FORMAT_R16_UINT;
	geo->IndexBufferByteSize = ibByteSize;
	geo->DrawArgs["box"] = boxSubmesh;
	geo->DrawArgs["sphere"] = sphereSubmesh;
	mGeometries[geo->Name] = std::move(geo);
}

void GameObject::BuildRenderOpBox(ComPtr<ID3D12Device> device) {
	auto boxRitem = std::make_unique<RenderItem>();
	boxRitem->ObjCBIndex = ObjIndex;
	boxRitem->Geo = mGeometries["shapeGeo"].get();
	boxRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	boxRitem->IndexCount = boxRitem->Geo->DrawArgs["box"].IndexCount;
	boxRitem->StartIndexLocation = boxRitem->Geo->DrawArgs["box"].StartIndexLocation;
	boxRitem->BaseVertexLocation = boxRitem->Geo->DrawArgs["box"].BaseVertexLocation;

	BuildObjectConstantBuffers(device, &boxRitem);

	mAllRitems.push_back(std::move(boxRitem));
	mOpaqueRitems.push_back(mAllRitems[ObjIndex].get());
	ObjIndex++;

}

void GameObject::BuildRenderOpCircle(ComPtr<ID3D12Device> device) {
	auto leftSphereRitem = std::make_unique<RenderItem>();
	leftSphereRitem->ObjCBIndex = ObjIndex;
	leftSphereRitem->Geo = mGeometries["shapeGeo"].get();
	leftSphereRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	leftSphereRitem->IndexCount = leftSphereRitem->Geo->DrawArgs["sphere"].IndexCount;
	leftSphereRitem->StartIndexLocation = leftSphereRitem->Geo->DrawArgs["sphere"].StartIndexLocation;
	leftSphereRitem->BaseVertexLocation = leftSphereRitem->Geo->DrawArgs["sphere"].BaseVertexLocation;

	BuildObjectConstantBuffers(device, &leftSphereRitem);

	mAllRitems.push_back(std::move(leftSphereRitem));
	mOpaqueRitems.push_back(mAllRitems[ObjIndex].get());
	ObjIndex++;

}

const std::vector<RenderItem*>& GameObject::GetOpaqueItems()
{
	return mOpaqueRitems;
}

std::vector<std::unique_ptr<RenderItem>>& GameObject::GetAllItems()
{
	return mAllRitems;
}

void GameObject::BuildObjectConstantBuffers(ComPtr<ID3D12Device> device, std::unique_ptr<RenderItem>* object)
{
	object->get()->mObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(device.Get(), 1, true);
}


