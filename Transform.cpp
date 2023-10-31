#include "Transform.h"


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

void Transform::Identity()
{
	TRANSFORM id;
	id.mRot = XMMatrixIdentity();
}

void Transform::FromMatrix(XMMATRIX* pMat)
{
	TRANSFORM ObjectMatrix;
	ObjectMatrix.matrix = *pMat;
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
	
}

void Transform::Rotate(float yaw, float pitch, float roll)
{
	XMMatrixRotationRollPitchYaw(yaw, pitch, roll);
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
