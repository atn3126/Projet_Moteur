#include "Transform.h"

/*qx,α=cosα2+(sinα2)i
qy,β=cosβ2+(sinβ2)j
qz,γ=cosγ2+(sinγ2)k*/

/*matPos = XMMatrixTranslation(x, y, z);*/

//matRot = XMMatrixRotationRollPitchYaw(yaw, pitch, roll);

void Transform::Identity(TRANSFORM* mat)
{
	mat->mSca = MathHelper::Identity4x4();
	mat->mRot = MathHelper::Identity4x4();
	mat->mPos = MathHelper::Identity4x4();
}

//void Transform::UpdateMatrix()
//{
//	TRANSFORM mat{};
//	mat.mWorld = mat.mSca;
//	mat.mWorld = MultiplyFloat4X4(mat.mWorld, mat.mRot);
//	mat.mWorld = MultiplyFloat4X4(mat.mWorld, mat.mPos);
//}

XMMATRIX Transform::Rotate(float yaw, float pitch, float roll)
{
	TRANSFORM mat{};
	Identity(&mat);

	float cosPitch = cosf(pitch);
	float sinPitch = sinf(pitch);

	float cosYaw = cosf(yaw);
	float sinYaw = sinf(yaw);

	float cosRaw = cosf(roll);
	float sinRaw = sinf(roll);

	mat.mRot._11 = cosRaw * cosYaw + sinRaw * sinPitch * sinYaw;
	mat.mRot._12 = sinRaw * cosPitch;
	mat.mRot._13 = sinRaw * sinPitch * cosYaw - cosRaw * sinYaw;

	mat.mRot._21 = cosRaw * sinPitch * sinYaw - sinRaw * cosYaw;
	mat.mRot._22 = cosRaw * cosPitch;
	mat.mRot._23 = sinRaw * sinYaw + cosRaw * sinPitch * cosYaw;

	mat.mRot._31 = cosPitch * sinYaw;
	mat.mRot._32 = -sinPitch;
	mat.mRot._33 = cosPitch * cosYaw;

	XMMATRIX matRot = XMLoadFloat4x4(&mat.mRot);
	return matRot;
}

XMMATRIX Transform::Translate(float x, float y, float z)
{
	TRANSFORM mat{};
	Identity(&mat);
	mat.mPos._41 = x;
	mat.mPos._42 = y;
	mat.mPos._43 = z;
	XMMATRIX matPos = XMLoadFloat4x4(&mat.mPos);
	return matPos;
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
