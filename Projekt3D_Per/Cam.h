#ifndef CAM_H
#define CAM_H

#include "Globals.h"

class Camera
{
public:
	Camera();
	~Camera();

	XMFLOAT4X4 GetViewXM() const;
	XMFLOAT4X4 GetProjXM() const;
	XMMATRIX GetViewMa() const;
	XMMATRIX GetProjMa() const;

	void SetProj(float fovY, float aspect, float zn, float zf);
	void setPos(XMFLOAT3 Pos);
	XMFLOAT3 getPos();

	void Update();
	void Walk(float d);
	void Strafe(float d);

	void MoveX(float angle);
	void MoveY(float angle);
	XMFLOAT3 getDir();


	void Input(float time, HWND hwnd);

private:

	//Vectors
	XMVECTOR Up;
	XMVECTOR Right;
	XMVECTOR CamDir;
	XMVECTOR Position;

	//Matrix
	XMFLOAT4X4 View;
	XMFLOAT4X4 Proj;

	//Input
	POINT prevMouse;
	POINT currMouse;

};
#endif




