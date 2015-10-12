#ifndef LOADOBJECT_H
#define LOADOBJECT_H

#include "Object.h"

class LoadObject : public Entity
{
public:
	LoadObject();
	~LoadObject();

	void LoadObjFile(wstring fileName);

private:


};
#endif