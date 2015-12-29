#ifndef QUADTREE_H
#define QUADTREE_H
#include "Globals.h"
#include "Cam.h"

class QuadTree
{
private:
	UINT* Indices;
	QuadTree* Children;//mah Chiledreren

public:
	void Init(ID3D11Device* Device);
	void Render(ID3D11DeviceContext* DeviceContext);
	bool CheckFrustum(Camera Cam);

};

#endif
