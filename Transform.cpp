#include "Transform.h"

/*qx,α=cosα2+(sinα2)i
qy,β=cosβ2+(sinβ2)j
qz,γ=cosγ2+(sinγ2)k*/

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



void Transform::RotateYaw(float angle)
{
	TRANSFORM mat{};
	mat.vDir.x = angle;
	mat.vDir.y = 0;
	mat.vDir.z = 0;
	/*XMMatrixRotationX(angle);*/
}

void Transform::RotatePitch(float angle)
{
	TRANSFORM mat{};
	mat.vRight.x = 0;
	mat.vRight.y = angle;
	mat.vRight.z = 0;
	/*XMMatrixRotationY(angle);*/
}

void Transform::RotateRoll(float angle)
{
	TRANSFORM mat{};
	mat.vUp.x = 0;
	mat.vUp.y = 0;
	mat.vUp.z = angle;
	/*XMMatrixRotationZ(angle);*/
}

void Transform::Translate(float x, float y, float z)
{
	TRANSFORM mat{};
	Identity();
	mat.mPos._14 = x;
	mat.mPos._24 = y;
	mat.mPos._34 = z;
	UpdateMatrix();
}


void Transform::UpdateRotationFromVectors()
{
	TRANSFORM mat{};
	mat.qRot.x = sqrt(1 + mat.vDir.x + mat.vRight.y + mat.vUp.z)/2;
	mat.qRot.y = (mat.vUp.y - mat.vRight.z)/(4 * mat.qRot.x);
	mat.qRot.z = (mat.vDir.z - mat.vUp.x) / (4 * mat.qRot.x);
	mat.qRot.w = (mat.vRight.x - mat.vDir.y) / (4 * mat.qRot.x);
}

void Transform::UpdateRotationFromQuaternion()
{
	TRANSFORM mat{};
	mat.mRot._11 = 2 * (mat.qRot.x * mat.qRot.x + mat.qRot.y * mat.qRot.y) - 1;
	mat.mRot._12 = 2 * (mat.qRot.y * mat.qRot.z - mat.qRot.x * mat.qRot.w);
	mat.mRot._13 = 2 * (mat.qRot.y * mat.qRot.w + mat.qRot.x * mat.qRot.z);

	mat.mRot._21 = 2 * (mat.qRot.y * mat.qRot.z + mat.qRot.x * mat.qRot.w);
	mat.mRot._22 = 2 * (mat.qRot.x * mat.qRot.x + mat.qRot.z * mat.qRot.z) - 1;
	mat.mRot._23 = 2 * (mat.qRot.z * mat.qRot.w - mat.qRot.x * mat.qRot.y);

	mat.mRot._31 = 2 * (mat.qRot.y * mat.qRot.w - mat.qRot.x * mat.qRot.z);
	mat.mRot._32 = 2 * (mat.qRot.z * mat.qRot.w + mat.qRot.x * mat.qRot.y);
	mat.mRot._33 = 2 * (mat.qRot.x * mat.qRot.x + mat.qRot.w * mat.qRot.w) - 1;
}

void Transform::UpdateMatrix()
{
	TRANSFORM mat{};
	mat.mWorld = mat.mSca;
	mat.mWorld = MultiplyFloat4X4(mat.mWorld, mat.mRot);
	mat.mWorld = MultiplyFloat4X4(mat.mWorld, mat.mPos);
}

void Transform::Rotate(float yaw, float pitch, float roll)
{
	/*XMMatrixRotationRollPitchYaw(yaw, pitch, roll);*/

	/*
	RotateYaw(float yaw);
	RotatePitch(float pitch);
	RotateRoll(float roll);

	TRANSFORM mat;
	mat.mRot._11 = mat.vDir.x;
	mat.mRot._12 = mat.vDir.y;
	mat.mRot._13 = mat.vDir.z;

	mat.mRot._21 = mat.vRight.x;
	mat.mRot._22 = mat.vRight.y;
	mat.mRot._23 = mat.vRight.z;

	mat.mRot._31 = mat.vUp.x;
	mat.mRot._32 = mat.vUp.y;
	mat.mRot._33 = mat.vUp.z;*/
	Identity();
	RotateYaw(yaw);
	RotatePitch(pitch);
	RotateRoll(roll);
	UpdateRotationFromVectors();
	UpdateRotationFromQuaternion();
	UpdateMatrix();
}




/*==============================================*/

XMFLOAT4X4 Transform::MultiplyFloat4X4(XMFLOAT4X4 mat1, XMFLOAT4X4 mat2)
{
	XMMATRIX matrix1 = XMLoadFloat4x4(&mat1);
	XMMATRIX matrix2 = XMLoadFloat4x4(&mat2);
	XMMATRIX matrixResult = XMMatrixMultiply(matrix1, matrix2);
	XMStoreFloat4x4(&mat1, matrixResult);
	return mat1;
}
