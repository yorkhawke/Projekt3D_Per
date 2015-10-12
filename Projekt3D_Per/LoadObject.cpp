#include "LoadObject.h"

LoadObject::LoadObject()
{

}

LoadObject::~LoadObject()
{

}

void LoadObject::LoadObjFile(wstring fileName)
{
	HRESULT hr;
	ifstream fileIn;

	fileIn.open(fileName, ifstream::in);

	vector<XMFLOAT3> vPos;
	vector<XMFLOAT3> vNor;
	vector<XMFLOAT2> vTC;

	vector<UINT> indPos;
	vector<UINT> indNor;
	vector<UINT> indTex;
	bool loop = true;
	int test1 = 0;
	int test2 = 0;
	char check;

	if (fileIn.is_open())
	{
		cout << "Open" << endl;
		while (loop)
		{
			check = fileIn.get();

			switch (check)
			{

			case '#':
				check = fileIn.get();
				while (check != '\n')
					check = fileIn.get();
				break;
			case 'v':
				check = fileIn.get();
				if (check == ' ')
				{
					float fx, fy, fz;
					fileIn >> fx >> fy >> fz;
					vPos.push_back(XMFLOAT3(fx, fy + 20.0f, fz));
					nrVertrices++;
				}
				if (check == 't')
				{
					float txx, txz;
					fileIn >> txx >> txz;
					vTC.push_back(XMFLOAT2(txx, txz));
					test1++;
				}
				if (check == 'n')
				{
					float nx, ny, nz;
					fileIn >> nx >> ny >> nz;
					vNor.push_back(XMFLOAT3(nx, ny, nz));
					test2++;

				}
				break;
			case 'f':
				check = fileIn.get();

				if (check == ' ')
				{

					while (check != '\n')
					{

						int indP, indT, indN;

						char pop;

						fileIn >> indP >> pop >> indT >> pop >> indN;

						indNor.push_back(indN);

						indTex.push_back(indT);

						indPos.push_back(indP);

						nrIndexes++;

						check = fileIn.get();

					}
				}
				break;
			}

			if (fileIn.fail())
			{
				loop = false;
			}
		}

		Vertices = new Vertex[nrIndexes];
		for (int i = 0; i < nrIndexes; i++)
		{
			Vertices[i].Position = vPos[indPos[i] - 1];
			Vertices[i].Normal = vNor[indNor[i] - 1];
			Vertices[i].Texture = vTC[indTex[i] - 1];
		}

		fileIn.close();
	}
}