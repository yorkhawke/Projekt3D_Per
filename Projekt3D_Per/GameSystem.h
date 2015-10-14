#ifndef GAME_SYSTEM_H
#define GAME_SYSTEM_H
// local Includes
#include "Globals.h"
#include "WindowsMain.h"
#include "Dx.h"
#include"Time.h"
#include "Cam.h"
#include "Object.h"
#include "LandObject.h"
#include "defered.h"
#include "LoadObject.h"


struct Matrix
{
	XMFLOAT4X4 World;
	XMFLOAT4X4 View;
	XMFLOAT4X4 Proj;
};

class GameSystem
{
private:
	//system classes goes here
	WindowsMain mainHwnd;
	Dx directX;
	bool defRender = true;
	Time gameTime;
	Camera cam;

	//stuff
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;

	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader* PixelShader;
	ID3D11InputLayout* VertexLayout;

	ID3D11SamplerState* SampleState;
	ID3D11Buffer* ScreenBuffer;
	ID3D11Buffer* MatrixBuffer;
	Matrix matrix;
	//ShaderResources

	ID3D11RenderTargetView* Backbuffer;
	//map
	LandObject hMap;
	LoadObject obj;
	Defered DeferedRendering;


public:
	GameSystem();
	~GameSystem();
	void CreateBuffers();
	HRESULT CreateSwapChain();
	void createShaders();
	void setShaders();
	void StartGame(float gametime, float fps, HINSTANCE hinstance);
	void Render();
	HWND getMainHwnd();
};

#endif
