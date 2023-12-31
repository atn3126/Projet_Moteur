#include "GameObject.h"



GameObject::GameObject()
{
}

GameObject::~GameObject()
{

}

void GameObject::Init(ComPtr<ID3D12GraphicsCommandList> cmdList, ComPtr<ID3D12Device> device) {
	m_device = device;
	CreateGeometry geoGen;
	CreateGeometry::MeshData box = geoGen.CreateBox(.5f, 0.5f, 1.5f, 3);
	CreateGeometry::MeshData sphere = geoGen.CreateSphere(0.5f, 20, 20);
	CreateGeometry::MeshData pyramide = geoGen.CreatePyramide(1.f, 1.f, 0.3f, 3);
	CreateGeometry::MeshData projectile = geoGen.CreateCylinder(0.05f, 0.05f, 1.f, 380, 250);
	UINT boxVertexOffset = 0;
	UINT boxIndexOffset = 0;
	UINT sphereVertexOffset = (UINT)box.Vertices.size();
	UINT sphereIndexOffset = (UINT)box.Indices32.size();
	UINT pyramideVertexOffset = (UINT)box.Vertices.size() + (UINT)sphere.Vertices.size();
	UINT pyramideIndexOffset = (UINT)box.Indices32.size() + (UINT)sphere.Indices32.size();
	UINT projectileVertexOffset = (UINT)box.Vertices.size() + (UINT)sphere.Vertices.size() + (UINT)pyramide.Vertices.size();
	UINT projectileIndexOffset = (UINT)box.Indices32.size() + (UINT)sphere.Indices32.size() + (UINT)pyramide.Vertices.size();


	SubmeshGeometry boxSubmesh;
	boxSubmesh.IndexCount = (UINT)box.Indices32.size();
	boxSubmesh.StartIndexLocation = boxIndexOffset;
	boxSubmesh.BaseVertexLocation = boxVertexOffset;

	SubmeshGeometry sphereSubmesh;
	sphereSubmesh.IndexCount = (UINT)sphere.Indices32.size();
	sphereSubmesh.StartIndexLocation = sphereIndexOffset;
	sphereSubmesh.BaseVertexLocation = sphereVertexOffset;

	SubmeshGeometry pyramideSubmesh;
	pyramideSubmesh.IndexCount = (UINT)pyramide.Indices32.size();
	pyramideSubmesh.StartIndexLocation = pyramideIndexOffset;
	pyramideSubmesh.BaseVertexLocation = pyramideVertexOffset;

	SubmeshGeometry projectileSubmesh;
	projectileSubmesh.IndexCount = (UINT)projectile.Indices32.size();
	projectileSubmesh.StartIndexLocation = projectileIndexOffset;
	projectileSubmesh.BaseVertexLocation = projectileVertexOffset;

	auto totalVertexCount = box.Vertices.size() + sphere.Vertices.size() + pyramide.Vertices.size() + projectile.Vertices.size();
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
	for (size_t i = 0; i < pyramide.Vertices.size(); i++, k++) {
		vertices[k].Pos = pyramide.Vertices[i].Position;
		vertices[k].Color = XMFLOAT4(DirectX::Colors::Yellow);
	}

	for (size_t i = 0; i < projectile.Vertices.size(); i++, k++) {
		vertices[k].Pos = projectile.Vertices[i].Position;
		vertices[k].Color = XMFLOAT4(DirectX::Colors::Aquamarine);
	}

	std::vector<std::uint16_t> indices;
	indices.insert(indices.end(),
		std::begin(box.GetIndices16()),
		std::end(box.GetIndices16()));
	indices.insert(indices.end(),
		std::begin(sphere.GetIndices16()),
		std::end(sphere.GetIndices16()));
	indices.insert(indices.end(),
		std::begin(pyramide.GetIndices16()),
		std::end(pyramide.GetIndices16()));
	indices.insert(indices.end(),
		std::begin(projectile.GetIndices16()),
		std::end(projectile.GetIndices16()));

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
	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(m_device.Get(),
		cmdList.Get(), vertices.data(), vbByteSize, geo->VertexBufferUploader);
	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(m_device.Get(),
		cmdList.Get(), indices.data(), ibByteSize, geo->IndexBufferUploader);
	geo->VertexByteStride = sizeof(Vertex);
	geo->VertexBufferByteSize = vbByteSize;
	geo->IndexFormat = DXGI_FORMAT_R16_UINT;
	geo->IndexBufferByteSize = ibByteSize;
	geo->DrawArgs["box"] = boxSubmesh;
	geo->DrawArgs["sphere"] = sphereSubmesh;
	geo->DrawArgs["pyramide"] = pyramideSubmesh;
	geo->DrawArgs["projectile"] = projectileSubmesh;
	mGeometries[geo->Name] = std::move(geo);
}

void GameObject::BuildRenderOpBox() {
	auto boxRitem = std::make_unique<RenderItem>();
	boxRitem->ObjCBIndex = ObjIndex;
	boxRitem->Geo = mGeometries["shapeGeo"].get();
	boxRitem->Type = "testBox";
	boxRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	boxRitem->IndexCount = boxRitem->Geo->DrawArgs["box"].IndexCount;
	boxRitem->StartIndexLocation = boxRitem->Geo->DrawArgs["box"].StartIndexLocation;
	boxRitem->BaseVertexLocation = boxRitem->Geo->DrawArgs["box"].BaseVertexLocation;

	BuildObjectConstantBuffers(&boxRitem);

	mAllRitems.push_back(std::move(boxRitem));
	mOpaqueRitems.push_back(mAllRitems[ObjIndex].get());
	ObjIndex++;

}

void GameObject::BuildRenderOpPyramide() {
	auto pyramideRitem = std::make_unique<RenderItem>();
	pyramideRitem->ObjCBIndex = ObjIndex;
	pyramideRitem->Geo = mGeometries["shapeGeo"].get();
	pyramideRitem->Type = "player";
	pyramideRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	pyramideRitem->IndexCount = pyramideRitem->Geo->DrawArgs["pyramide"].IndexCount;
	pyramideRitem->StartIndexLocation = pyramideRitem->Geo->DrawArgs["pyramide"].StartIndexLocation;
	pyramideRitem->BaseVertexLocation = pyramideRitem->Geo->DrawArgs["pyramide"].BaseVertexLocation;
	XMStoreFloat4x4(&pyramideRitem->World, XMMatrixMultiply(XMLoadFloat4x4(&pyramideRitem->World), pyramideRitem->Rotate(0, 70, 0)));
	XMStoreFloat4x4(&pyramideRitem->World, XMMatrixMultiply(XMLoadFloat4x4(&pyramideRitem->World), pyramideRitem->Translate(0, -1, 0)));

	BuildObjectConstantBuffers(&pyramideRitem);

	mAllRitems.push_back(std::move(pyramideRitem));
	mOpaqueRitems.push_back(mAllRitems[ObjIndex].get());
	ObjIndex++;

}

void GameObject::BuildRenderOpProjectile(float playerPosX, float playerPosY, float playerPosZ) {
	auto projectileRitem = std::make_unique<RenderItem>();
	projectileRitem->ObjCBIndex = ObjIndex;
	projectileRitem->lifeTime = 20;
	projectileRitem->Geo = mGeometries["shapeGeo"].get();
	projectileRitem->Type = "projectile";
	projectileRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	projectileRitem->IndexCount = projectileRitem->Geo->DrawArgs["projectile"].IndexCount;
	projectileRitem->StartIndexLocation = projectileRitem->Geo->DrawArgs["projectile"].StartIndexLocation;
	projectileRitem->BaseVertexLocation = projectileRitem->Geo->DrawArgs["projectile"].BaseVertexLocation;
	XMStoreFloat4x4(&projectileRitem->World, XMMatrixMultiply(XMLoadFloat4x4(&projectileRitem->World), projectileRitem->Rotate(0, 90, 0)));
	projectileRitem->World._41 = playerPosX;
	projectileRitem->World._42 = playerPosY;
	projectileRitem->World._43 = playerPosZ;
	BuildObjectConstantBuffers(&projectileRitem);

	mAllRitems.push_back(std::move(projectileRitem));
	mOpaqueRitems.push_back(mAllRitems[ObjIndex].get());
	ObjIndex++;

}

void GameObject::BuildRenderOpCircle() {
	// Providing a seed value
	srand((unsigned)time(NULL));
	// Get a random number
	float randomX = (float)rand() / (float)(RAND_MAX / (1.5f));
	// Providing a seed value
	srand((unsigned)time(NULL));

	int randomSignX = 1 + (rand() % 2);

	// Providing a seed value
	srand((unsigned)time(NULL));
	// Get a random number
	float randomY = (float)rand() / (float)(RAND_MAX / (1.5f));

	srand((unsigned)time(NULL));
	int randomSignY = 1 + (rand() % 2);

	auto leftSphereRitem = std::make_unique<RenderItem>();
	leftSphereRitem->ObjCBIndex = ObjIndex;
	leftSphereRitem->Geo = mGeometries["shapeGeo"].get();
	leftSphereRitem->Type = "asteroid";
	leftSphereRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	leftSphereRitem->IndexCount = leftSphereRitem->Geo->DrawArgs["sphere"].IndexCount;
	leftSphereRitem->StartIndexLocation = leftSphereRitem->Geo->DrawArgs["sphere"].StartIndexLocation;
	leftSphereRitem->BaseVertexLocation = leftSphereRitem->Geo->DrawArgs["sphere"].BaseVertexLocation;
	XMMATRIX temp = XMLoadFloat4x4(&leftSphereRitem->World);
	if (randomSignX == 1 && randomSignY == 1) {
		XMStoreFloat4x4(&leftSphereRitem->World, XMMatrixMultiply(temp, leftSphereRitem->Translate(randomX, randomY, 5)));
	}
	else if (randomSignX == 2 && randomSignY == 1) {
		XMStoreFloat4x4(&leftSphereRitem->World, XMMatrixMultiply(temp, leftSphereRitem->Translate(-randomX, randomY, 5)));
	}
	else if (randomSignX == 2 && randomSignY == 2) {
		XMStoreFloat4x4(&leftSphereRitem->World, XMMatrixMultiply(temp, leftSphereRitem->Translate(-randomX, -randomY, 5)));
	}
	else if (randomSignX == 1 && randomSignY == 2) {
		XMStoreFloat4x4(&leftSphereRitem->World, XMMatrixMultiply(temp, leftSphereRitem->Translate(randomX, -randomY, 5)));
	}
	BuildObjectConstantBuffers(&leftSphereRitem);

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

void GameObject::SetOpaqueItems(std::vector<RenderItem*> OpaqueItems)
{
	mOpaqueRitems = OpaqueItems;
}

void GameObject::BuildObjectConstantBuffers(std::unique_ptr<RenderItem>* object)
{
	object->get()->mObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(m_device.Get(), 1, true);
}

void GameObject::RemoveObject(size_t index)
{
	mOpaqueRitems.erase(mOpaqueRitems.begin() + index);
}
void GameObject::setGameOver(bool newGameOver)
{
	gameOver = newGameOver;
}
bool GameObject::getGameOver()
{
	return gameOver;
}


