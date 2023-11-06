#include "GenerateObj.h"
struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};
std::unique_ptr<MeshGeometry> GenerateObj::GetmBoxGeo()
{
	return std::unique_ptr<MeshGeometry>();
}
void GenerateObj::Initialize(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>cmdList, Microsoft::WRL::ComPtr <ID3D12Device> Device)
{
	std::array<Vertex, 8> vertices =
	{
		Vertex({ XMFLOAT3(-.5f, -.5f, -.5f), XMFLOAT4(Colors::White) }),
		Vertex({ XMFLOAT3(-.5f, +.5f, -.5f), XMFLOAT4(Colors::Black) }),
		Vertex({ XMFLOAT3(+.5f, +.5f, -.5f), XMFLOAT4(Colors::Red) }),
		Vertex({ XMFLOAT3(+.5f, -.5f, -.5f), XMFLOAT4(Colors::Green) }),
		Vertex({ XMFLOAT3(-.5f, -.5f, +.5f), XMFLOAT4(Colors::Blue) }),
		Vertex({ XMFLOAT3(-.5f, +.5f, +.5f), XMFLOAT4(Colors::Yellow) }),
		Vertex({ XMFLOAT3(+.5f, +.5f, +.5f), XMFLOAT4(Colors::Cyan) }),
		Vertex({ XMFLOAT3(+.5f, -.5f, +.5f), XMFLOAT4(Colors::Magenta) })
	};

	std::array<std::uint16_t, 36> indices =
	{
		// front face
		1, 3, 0,
		2, 3, 1,

		// back face
		6, 4, 7,
		5, 4, 6,

		// left face
		5, 0, 4,
		1, 0, 5,

		// right face
		3, 6, 7,
		2, 6, 3,

		// top face
		1, 6, 2,
		5, 6, 1,

		// bottom face
		7, 0, 3,
		4, 0, 7
	};

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	mBoxGeo = std::make_unique<MeshGeometry>();
	mBoxGeo->Name = "boxGeo";

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &mBoxGeo->VertexBufferCPU));
	CopyMemory(mBoxGeo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &mBoxGeo->IndexBufferCPU));
	CopyMemory(mBoxGeo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	mBoxGeo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(Device.Get(),
		cmdList.Get(), vertices.data(), vbByteSize, mBoxGeo->VertexBufferUploader);

	mBoxGeo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(Device.Get(),
		cmdList.Get(), indices.data(), ibByteSize, mBoxGeo->IndexBufferUploader);

	mBoxGeo->VertexByteStride = sizeof(Vertex);
	mBoxGeo->VertexBufferByteSize = vbByteSize;
	mBoxGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
	mBoxGeo->IndexBufferByteSize = ibByteSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	mBoxGeo->DrawArgs["box"] = submesh;


	/*CubePos = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR posVec = XMLoadFloat4(&CubePos);

	XMMATRIX tmpMat = XMMatrixTranslationFromVector(posVec);
	XMStoreFloat4x4(&CubeRotMat, XMMatrixIdentity());*/
	//XMStoreFloat4x4(&mWorld, tmpMat);
}

void GenerateObj::Update()
{

}
