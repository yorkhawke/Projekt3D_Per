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
#include "Ssao.h"
#include "ShadowMap.h"

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
	ID3D11Buffer* SunBuffer;
	ID3D11Buffer* LightBuffer;
	Matrix matrix;
	//ShaderResources

	ID3D11RenderTargetView* Backbuffer;
	//map
	LandObject hMap;
	LoadObject obj;
	Defered DeferedRendering;
	SSao Ssao;
	Matrix sunMatrix;
	DirectionalLight Sun;
	ShadowMap shadow;

public:
	GameSystem();
	~GameSystem();
	void CreateBuffers();
	HRESULT CreateSwapChain();
	void createShaders();
	void setShaders();
	void StartGame(float gametime, float fps, HINSTANCE hinstance);
	void Render();
	void LightSun();
	HWND getMainHwnd();
};

#endif
