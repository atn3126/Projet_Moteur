#pragma once

#include "d3dApp.h"
#include "MathHelper.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

class Transform
{
public :

	XMMATRIX Rotate(float yaw, float pitch, float roll);
	XMMATRIX Translate(float x, float y, float z);

private :


	void Identity();
	void UpdateRotationFromVectors();
	void UpdateRotationFromQuaternion();
	void RotateYaw(float angle);
	void RotatePitch(float angle);
	void RotateRoll(float angle);


	XMFLOAT4X4 MultiplyFloat4X4(XMFLOAT4X4 mat1, XMFLOAT4X4 mat2);
};