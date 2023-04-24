#ifndef CAMERA_H
#define CAMERA_H

#include "d3dUtil.h"

class Camera
{
public:

	Camera();
	~Camera();

	// ��ȡ/�����������������
	DirectX::XMVECTOR GetPosition() const;
	DirectX::XMFLOAT3 GetPosition3f() const;
	void SetPosition(float x, float y, float z);
	void SetPosition(const DirectX::XMFLOAT3& v);

	// ��ȡ������Ļ�����
	DirectX::XMVECTOR GetRight() const;
	DirectX::XMFLOAT3 GetRight3f() const;
	DirectX::XMVECTOR GetUp() const;
	DirectX::XMFLOAT3 GetUp3f() const;
	DirectX::XMVECTOR GetLook() const;
	DirectX::XMFLOAT3 GetLook3f() const;

	// ��ȡ��׶��frustum������
	float GetNearZ() const;
	float GetFarZ() const;
	float GetAspect() const;
	float GetFovY() const;
	float GetFovX() const;

	// ��ȡ�ù۲�ռ������ʾ��Զ��ƽ��Ĵ�С
	float GetNearWindowWidth() const;
	float GetNearWindowHeight() const;
	float GetFarWindowWidth() const;
	float GetFarWindowHeight() const;

	// ������׶��frustum
	void SetLens(float fovY, float aspect, float zn, float zf);

	// ͨ��LookAt�����Ĳ���������������ռ�
	void LookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR worldUp);
	void LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up);

	// ��ȡ�۲������ͶӰ����
	DirectX::XMMATRIX GetView() const;
	DirectX::XMMATRIX GetProj() const;

	DirectX::XMFLOAT4X4 GetView4x4f() const;
	DirectX::XMFLOAT4X4 GetProj4x4f() const;

	// �����������ƽ��Strafe��ǰ���ƶ�
	void Strafe(float d);
	void Walk(float d);
	void UpDown(float d);

	// �������������ת
	void Pitch(float angle);
	void RotateY(float angle);

	// �޸��������λ���볯���,���ô˺��������¹����۲����
	void UpdateViewMatrix();

private:

	// ���������ϵ���������ռ������
	DirectX::XMFLOAT3 mPosition = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 mRight = { 1.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 mUp = { 0.0f,1.0f,0.0f };
	DirectX::XMFLOAT3 mLook = { 0.0f,0.0f,1.0f };

	// ������׶�������
	float mNearZ = 0.0f;
	float mFarZ = 0.0f;
	float mAspect = 0.0f;
	float mFovY = 0.0f;
	float mNearWindowHeight = 0.0f;
	float mFarWindowHeight = 0.0f;

	bool mViewDirty = true;

	// �۲������ͶӰ����
	DirectX::XMFLOAT4X4 mView = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 mProj = MathHelper::Identity4x4();
};

#endif // CAMERA_H