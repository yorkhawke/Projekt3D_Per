#ifndef QUADTREE_H
#define QUADTREE_H
#include "Globals.h"
#include "Cam.h"

class QuadTree
{
private:
	UINT* indices; //Indices array
	int nrIndices;// number of indices
	bool render = true;
	QuadTree* Children;//mah Chiledreren
	ID3D11Buffer* IndexB;

public:
	QuadTree();
	~QuadTree();
	void Initialzie(UINT* Ind, int NrIn, ID3D11Device* Device);
	void Render(ID3D11DeviceContext* DeviceContext);
	bool CheckFrustum();

protected:
	BoundingBox box;
};

#endif
