#include "Transform.h"


struct TRANSFORM
{
	XMFLOAT3 vSca;
	XMFLOAT4X4 mSca;

	XMFLOAT3 vDir;
	XMFLOAT3 vRight;
	XMFLOAT3 vUp;
	XMFLOAT4 qRot;
	XMFLOAT4X4 mRot;

	XMFLOAT3 vPos;
	XMFLOAT4X4 mPos;

	XMFLOAT4X4 mWorld;
};

void Transform::Identity()
{
	TRANSFORM id;
	id.mSca = MathHelper::Identity4x4();
	id.mRot = MathHelper::Identity4x4();
	id.mPos = MathHelper::Identity4x4();
}

void Transform::FromMatrix(XMMATRIX* pMat)
{
	
}

void Transform::UpdateRotationFromVectors()
{

}

void Transform::UpdateRotationFromQuaternion()
{
	
}

void Transform::UpdateRotationFromMatrix()
{
}

void Transform::UpdateMatrix()
{
	TRANSFORM mat;
	mat.mWorld = mat.mSca;

	
}

void Transform::Rotate(float yaw, float pitch, float roll)
{
	
}

void Transform::RotateYaw(float angle)
{
	XMMatrixRotationX(angle);
}

void Transform::RotatePitch(float angle)
{
	XMMatrixRotationY(angle);
}

void Transform::RotateRoll(float angle)
{
	XMMatrixRotationZ(angle);
}

void Transform::RotateWorld(XMMATRIX* pMatrix)
{
	/*XMMatrixRotationRollPitchYaw(yaw, pitch, roll);*/
}

void Transform::RotateWorldX(float angle)
{
}

void Transform::RotateWorldY(float angle)
{
}

void Transform::RotateWorldZ(float angle)
{
}

void Transform::MultiplyFloat4X4(XMFLOAT4X4 mat1, XMFLOAT4X4 mat2)
{
	mat1._11 = mat1._11 * mat2._11 + mat1._12 * mat2._21 + mat1._13 * mat2._31 + mat1._14 * mat2._41;
	mat1._12 = mat1._11 * mat2._12 + mat1._12 * mat2._22 + mat1._13 * mat2._32 + mat1._14 * mat2._42;
}
