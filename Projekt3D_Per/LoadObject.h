#ifndef LOADOBJECT_H
#define LOADOBJECT_H

#include "Object.h"

class LoadObject : public Entity
{
public:
	LoadObject();
	~LoadObject();

	void LoadObjFile(wstring fileName);
	void render(ID3D11DeviceContext* devCont);
private:


};
#endif