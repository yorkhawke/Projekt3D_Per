#include <random>
#include "Globals.h"
const int wind = 1024;

using namespace std;
using namespace DirectX;
class SSao
{
private:

	ID3D11PixelShader* SSaoP;
	ID3D11VertexShader* SSaoV;
	ID3D11Buffer* PixelBuffer;
	ID3D11SamplerState* Pointsampler;
	ID3D11Buffer* ScreenBuffer;

	ID3D11ComputeShader* SSaoCF;
	ID3D11ComputeShader* SSaoCS;
	ID3D11UnorderedAccessView* blur;
	ID3D11UnorderedAccessView* blur2;
	ID3D11ShaderResourceView* CShaderTex;
	ID3D11ShaderResourceView* CShaderTex2;

	ID3D11ShaderResourceView* ShaderRandTex;
	ID3D11RenderTargetView* SSaoRTV;
	ID3D11InputLayout* Layout;

	ID3D11Texture2D* SSaoTex;
	ID3D11ShaderResourceView* SSaoSRV;
public:
	SSao();
	~SSao();
	void randomTex(ID3D11Device* dev, ID3D11DeviceContext *devCon);
	void renderPass(ID3D11Device* dev, ID3D11DeviceContext* devCon, XMFLOAT4X4 projView);
	void startUp(ID3D11Device* dev, ID3D11DeviceContext *devCon);
};


