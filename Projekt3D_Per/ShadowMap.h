#include "Globals.h"

#ifndef SHADOWMAP_H
#define SHADOWMAP_H

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();
	
	void startup(ID3D11Device* dev, ID3D11DeviceContext* devCont);
	void render(ID3D11DeviceContext* devCont);

private:

	ID3D11VertexShader* ShadowVertex;
	ID3D11PixelShader* ShadowPixel;
	
	ID3D11Texture2D* shadowMapTex;
	ID3D11RenderTargetView* ShadowMapRTV;
	ID3D11ShaderResourceView* ShadowMapSRV;


	//skapa shaders till shadowmappen
	// rendertarget view texture2d shaderresource
	//ska ta info från varje ljus som finns i programmet?
	//

};

#endif
