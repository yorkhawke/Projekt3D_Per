#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <d3d11.h>
#include <crtdbg.h>
#include "GameSystem.h"


//-----------------------------------------------------------------------
//		 _________________________________________________________		
//		|				PROJECT CHECKLIST						  |
//		|														  |
//		| 														  |
//		|	1. Defered Rendering. CHECK							  |
//		|	2.Parsing and rendering OBJ. CHECK					  |
//		|	3. Height-map were you can walk on terrain. CHECK	  |
//		|	4. Dynamic fucking ambient Occlusion. CHECK			  |
//		|	5. Shadow mapping. CHECK							  |
//		|	6. View frustum culling using QuadTree. CHECK		  |
//		|	7. Back-facee culling with Geometry shader. CHECK	  |
//		|	8-9. Gaussian filter done by Compute Shader. CHECK	  |
//		|_________________________________________________________|


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	MSG msg = { 0 };

	GameSystem game;

	AllocConsole();

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(780);
	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, NULL, _IONBF, 1);
	*stdout = *hf_out;

	HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
	hCrt = _open_osfhandle((long)handle_in, _O_TEXT);
	FILE* hf_in = _fdopen(hCrt, "r");
	setvbuf(hf_in, NULL, _IONBF, 128);
	*stdin = *hf_in;
	//game

	game.StartGame(0.0f,0.0f,hInstance);
	ShowWindow(game.getMainHwnd(), nCmdShow);
	while (WM_QUIT != msg.message)
	{

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			game.Render();
		}
	}

	return (int)msg.wParam;
}