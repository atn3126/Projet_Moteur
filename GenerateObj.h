#pragma once
#include "d3dApp.h"
#include "MathHelper.h"
#include "UploadBuffer.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

struct ObjectConstants
{
    XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
    XMFLOAT3 translation;
};

class GenerateObj {

public:
    void Initialize();
    void Update();
private:
    XMFLOAT4 CubePos;
    XMFLOAT4X4 CubeRotMat;
};