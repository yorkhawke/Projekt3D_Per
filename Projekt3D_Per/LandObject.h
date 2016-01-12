#ifndef LANDOBJECT_H
#define LANDOBJECT_H

#include "Object.h"
#include "QuadTree.h"

class LandObject : public Entity
{
public:
	LandObject();
	~LandObject();

	float getMapHeight(float x, float z);
	void detail(int m, int n);
	float averageH(int i, int j);
	void HMap(int m, int n, const string& fileName, float HeightScale, float HeightOffset);
	void CreateMap(int width, int height, UINT m, UINT n, ID3D11Device* device, ID3D11DeviceContext* devCont);
	void Indices(int m, int n);
	float HMap(float x, float z);
	void renderFrustCull(ID3D11DeviceContext* devCont, const XMMATRIX &projection, const XMMATRIX &view);
	void setupFrust(UINT m, UINT n, ID3D11Device* device);
protected:
	float** HeightMap;
	QuadTree frustCull;
	int faceCount;
};
#endif