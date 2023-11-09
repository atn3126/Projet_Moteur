#pragma once

#include "d3dApp.h"
#include "MathHelper.h"
#include "UploadBuffer.h"
#include "Transform.h"
#include "CreateGeometry.h"
#include "GameObject.h"
#include "InputManager.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

struct PassConstants {
    XMFLOAT4X4 View;
    XMFLOAT4X4 InvView;
    XMFLOAT4X4 Proj;
    XMFLOAT4X4 InvProj;
    XMFLOAT4X4 ViewProj;
    XMFLOAT4X4 InvViewProj;
    XMFLOAT3 EyePosW;
    float cbPerObjectPad1;
    XMFLOAT2 RenderTargetSize;
    XMFLOAT2 InvRenderTargetSize;
    float NearZ;
    float FarZ;
    float TotalTime;
    float DeltaTime;
};

class BoxApp : public D3DApp
{
public:
    BoxApp(HINSTANCE hInstance);
    BoxApp(const BoxApp& rhs) = delete;
    BoxApp& operator=(const BoxApp& rhs) = delete;
    ~BoxApp();

    virtual bool Initialize()override;

private:
    virtual void OnResize()override;
    void CameraInputs(const GameTimer& gt);
    void Camera(const GameTimer& gt);
    virtual void Update(const GameTimer& gt)override;
    void DrawRenderItems();
    virtual void Draw(const GameTimer& gt)override;

    void BuildDescriptorHeaps();
    void BuildConstantBuffers();
    void BuildRootSignature();
    void BuildShadersAndInputLayout();
    void BuildPSO();

private:
    XMFLOAT4 CubePos;
    XMFLOAT4X4 CubeRotMat;
    ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
    ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;

    GameObject gameObject;
    std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries;

    //Constant Buffer
    std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;
    std::unique_ptr<UploadBuffer<PassConstants>> PassCB = nullptr;

    //Stock RenderItem

    UINT mPassCbvOffset = 0;
    std::unique_ptr<MeshGeometry> mBoxGeo = nullptr;

    ComPtr<ID3DBlob> mvsByteCode = nullptr;
    ComPtr<ID3DBlob> mpsByteCode = nullptr;

    std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

    ComPtr<ID3D12PipelineState> mPSO = nullptr;

    // Inputs
    InputManager inputManager;

    // Camera
    XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    XMVECTOR DefaultUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    XMVECTOR camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    XMVECTOR camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    XMVECTOR camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    XMVECTOR camPosition = XMVectorSet(0.0f, 0.0f, -4.0f, 1.0f);

    XMMATRIX camRotationMatrix;
    XMMATRIX groundWorld;
    XMMATRIX camView;

    XMVECTOR getCam;

    float moveLeftRight = 0.0f;
    float moveBackForward = 0.0f;

    XMVECTOR camTarget;

    float camYaw = 0.0f;
    float camPitch = 0.0f;

    XMFLOAT4X4 mWorld = MathHelper::Identity4x4();
    XMFLOAT4X4 mView = MathHelper::Identity4x4();
    XMFLOAT4X4 mProj = MathHelper::Identity4x4();
};