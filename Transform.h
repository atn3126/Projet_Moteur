#pragma once

#include "d3dApp.h"
#include "MathHelper.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

class Transform
{
public :
	

private :

	struct TRANSFORM
	{

		XMVECTOR vSca;
		XMVECTOR vDir;
		XMVECTOR vRight;
		XMVECTOR vUp;
		XMFLOAT4 qRot;
		XMMATRIX mRot;
		XMVECTOR vPos;
		XMMATRIX matrix;

	};

	void Identity();
	void FromMatrix(XMMATRIX* pMat);
	void UpdateRotationFromVectors();
	void UpdateRotationFromQuaternion();
	void UpdateRotationFromMatrix();
	void UpdateMatrix();
	void Rotate(float yaw, float pitch, float roll);
	void RotateYaw(float angle);
	void RotatePitch(float angle);
	void RotateRoll(float angle);
	void RotateWorld(XMMATRIX* pMatrix);
	void RotateWorldX(float angle);
	void RotateWorldY(float angle);
	void RotateWorldZ(float angle);
};