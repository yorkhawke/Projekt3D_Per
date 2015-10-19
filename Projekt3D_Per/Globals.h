#ifndef GLOBALS_H
#define GLOBALS_H

//Pragma
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
//includes
#include <stdio.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <io.h>
#include <fcntl.h>
#include <DirectXMath.h>
#include <DXGI1_2.h>
#include <iostream>
#include <Windows.h>
#include <windowsx.h>
#include <vector>
#include <fstream>
#include "pch.h"
#include "WICTextureLoader.h"

//Globals

const int BufferC = 4;

const int ScreenHeight = 800;
const int ScreenWidth = 1040;

//nameSpace
using namespace DirectX;
using namespace std;


struct DirectionalLight
{
	XMFLOAT4 ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT3 SunPosition;
	float mp;
};

#endif