#include "Cam.h"
Camera::Camera()
{

	Up = XMLoadFloat3(&XMFLOAT3(0, 1, 0));
	Position = XMLoadFloat3(&XMFLOAT3(0,0,0));
	CamDir = XMLoadFloat3(&XMFLOAT3(0, 0, -1));
	XMStoreFloat4x4(&Proj, XMMatrixPerspectiveFovLH(XM_PI / 4, 800.0f / 600.0f, 0.5, 20000.0f));
	Right = XMVector3Cross(Up, CamDir);

	//For MouseInput
	currMouse.x = 0;
	currMouse.y = 0;
	prevMouse.x = 0;
	prevMouse.y = 0;
	flying = false;
}

Camera::~Camera()
{

}

void Camera::setPos(XMFLOAT3 Pos)
{
	Position = XMLoadFloat3(&Pos);
}

XMMATRIX Camera::GetViewMa() const
{
	XMMATRIX temp = XMLoadFloat4x4(&View);
	return temp;
}

void Camera::Input(float time, HWND hwnd)
{

	if (GetAsyncKeyState('W') & 0x8000)
		Walk(50 * time);

	if (GetAsyncKeyState('A') & 0x8000)
		Strafe(-50 * time);

	if (GetAsyncKeyState('S') & 0x8000)
		Walk(-50 * time);

	if (GetAsyncKeyState('D') & 0x8000)
		Strafe(50 * time);

	if (GetAsyncKeyState('F') & 0x8000)
	{
		if (flying)
			flying = false;
		else
			flying = true;
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (flying)
		{
		XMFLOAT3 temp = getPos();
		setPos(XMFLOAT3(temp.x, temp.y + 4.0, temp.z));	
		}
	}
	GetCursorPos(&currMouse);
	ScreenToClient(hwnd, &currMouse);

	if (GetAsyncKeyState(VK_LBUTTON))
	{

		float angleX, angleY;

		angleX = (currMouse.x - prevMouse.x) / 200.0f;
		angleY = (currMouse.y - prevMouse.y) / 200.0f;

		RECT r;

		GetWindowRect(hwnd, &r);

		MoveX(angleX);

		MoveY(angleY);

	}

	prevMouse = currMouse;

	Update();
	if (GetAsyncKeyState(VK_ESCAPE))
		PostQuitMessage;


}

XMMATRIX Camera::GetProjMa() const
{
	XMMATRIX temp = XMLoadFloat4x4(&Proj);
	return temp;
}

XMFLOAT3 Camera::getPos()
{
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, Position);
	return temp;
}

XMFLOAT4X4 Camera::GetViewXM()const
{
	return View;
}

XMFLOAT3 Camera::getDir()
{
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, CamDir);
	return temp;
}

XMFLOAT4X4 Camera::GetProjXM()const
{
	return Proj;
}

void Camera::SetProj(float fovY, float aspect, float zn, float zf)
{

	XMStoreFloat4x4(&Proj, XMMatrixPerspectiveFovLH(fovY, aspect, zn, zf));
}

void Camera::Update()
{

	XMStoreFloat4x4(&View, XMMatrixLookAtLH(Position, Position + CamDir, Up));

}

void Camera::Walk(float d)
{
	Position += XMVectorMultiply(CamDir, XMVectorReplicate(d));
	Update();
}

void Camera::Strafe(float d)
{
	Right = XMVector3Cross(Up, CamDir);

	Position += XMVectorMultiply(XMVectorReplicate(d), Right);
	Update();
}

void Camera::MoveY(float angle)
{
	XMMATRIX Rot = XMMatrixRotationAxis(Right, angle);

	Up = XMVector3TransformNormal(Up, Rot);
	CamDir = XMVector3TransformNormal(CamDir, Rot);
	Update();
}

void Camera::MoveX(float angle)
{
	XMMATRIX Rot = XMMatrixRotationY(angle);
	Right = XMVector3TransformNormal(Right, Rot);
	Up = XMVector3TransformNormal(Up, Rot);

	CamDir = XMVector3TransformNormal(CamDir, Rot);
	Update();
}