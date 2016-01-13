#include "LandObject.h"
LandObject::LandObject()
{
	Vertices = NULL;
}

LandObject::~LandObject()
{
	//delete HeightMap
	for (int i = 0; i < 256; i++)
		delete[] HeightMap[i];
	delete[] HeightMap;
	delete[] indices;
	delete[] Vertices;
}

float LandObject::getMapHeight(float x, float z)
{

	int px = x + 128;
	int pz = -z + 128;

	if (pz <= 0 || px <= 0)
	{
		return 0;
	}
	else if (pz >= 256 || px >= 256)
	{
		return 0;
	}

	float hi = HeightMap[pz][px];

	return hi;

}

void LandObject::detail(int m, int n)
{
	float** temp = new float*[m];// size of map

	for (int i = 0; i < m; i++)
	{

		temp[i] = new float[n];

		for (int j = 0; j < n; j++)
		{
			float test = averageH(i, j);
			temp[i][j] = test;
		}
	}

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			HeightMap[i][j] = temp[i][j];
		}
	}

	// delete temp pointers
	for (int i = 0; i < m; i++)
	{
		delete temp[i];
	}
	delete[] temp;
}

float LandObject::averageH(int i, int j)
{
	float avg = 0.0f, nr = 0.0f;

	for (int m = i - 1; m <= i + 1; m++)
	{
		for (int n = j - 1; n <= j + 1; n++)
		{
			if (m >= 0 && m < 256 && n >= 0 && n < 256)// 256 size of the map
			{
				avg += HeightMap[m][n];// fan ta i och j 
				nr += 1.0f;
			}

		}
	}

	return avg / nr;
}

float LandObject::HMap(float x, float z)
{
	int px = x + 128;
	int pz = -z + 128;

	if (pz <= 0 || px <= 0)
	{
		return 0;
	}
	else if (pz >= 256 || px >= 256)
	{
		return 0;
	}

	float hi = HeightMap[pz][px];
	return hi;

}

void LandObject::HMap(int m, int n, const string& fileName, float HeightScale, float HeightOffset)
{
	string HeightMapFile = fileName;

	vector<unsigned char> vertexHeights(m*n);

	ifstream file;

	file.open(fileName.c_str(), ios_base::binary);

	file.read((char *)&vertexHeights[0], (streamsize)vertexHeights.size());

	file.close();



	HeightMap = new float*[m];

	for (int i = 0; i < m; i++)
	{
		HeightMap[i] = new float[n];
	}

	for (int i = 0; i < m; i++)
	{

		for (int j = 0; j < n; j++)
		{
			HeightMap[i][j] = (float)vertexHeights[i*n + j] * HeightScale + HeightOffset;
		}

	}

	detail(m, n);

}

void LandObject::Indices(int m, int n)
{
	int k = 0;
	indices = new UINT[nrIndexes];

	for (int i = 0; i < m - 1; i++)
	{

		for (int j = 0; j < n - 1; j++)
		{

			indices[k] = i*n + j;
			indices[k + 1] = i*n + j + 1;
			indices[k + 2] = (i + 1)*n + j;
			indices[k + 3] = (i + 1)*n + j;
			indices[k + 4] = i*n + j + 1;
			indices[k + 5] = (i + 1)*n + j + 1;
			k += 6;
		}
	}
}

void LandObject::CreateMap(int width, int height, UINT m, UINT n, ID3D11Device* device, ID3D11DeviceContext* devCont)
{
	nrVertrices = m*n;
	faceCount = (m - 1)*(n - 1) * 2;
	nrIndexes = faceCount * 3;
	float dx = width / (n - 1);
	float dz = height / (m - 1);

	float du = 1.0f / (n - 1);
	float dv = 1.0f / (m - 1);

	Vertices = new Vertex[nrVertrices];

 	Indices(m, n);

	Entity::createTexture(device, devCont,L"grasscock.png");

	HMap(m, n, "HeightMap.raw", 0.5f, 0);

	for (int i = 0; i < m; i++)
	{
		float z = (height / 2) - i*dz;
		for (int j = 0; j < n; j++)
		{
			float x = -(width / 2) + j*dx;

			Vertices[i*n + j].Position = XMFLOAT3(x, HeightMap[i][j], z);

			Vertices[i*n + j].Normal = { 0, 0, 0 };

			Vertices[i*n + j].Texture.x = j * 64 * du;
			Vertices[i*n + j].Texture.y = i * 64 * dv;
		}
	}

	for (int i = 2; i < nrIndexes; i += 3)
	{

		XMFLOAT3 norm(0, 0, 0);

		XMStoreFloat3(&norm, (XMVector3Cross(XMLoadFloat3(&Vertices[indices[i - 2]].Position) - XMLoadFloat3(&Vertices[indices[i - 1]].Position), XMLoadFloat3(&Vertices[indices[i - 2]].Position) - XMLoadFloat3(&Vertices[indices[i]].Position))));

		Vertices[indices[i - 2]].Normal.x += norm.x;
		Vertices[indices[i - 2]].Normal.y += norm.y;
		Vertices[indices[i - 2]].Normal.z += norm.z;
		Vertices[indices[i - 1]].Normal.x += norm.x;
		Vertices[indices[i - 1]].Normal.y += norm.y;
		Vertices[indices[i - 1]].Normal.z += norm.z;
		Vertices[indices[i]].Normal.x += norm.x;
		Vertices[indices[i]].Normal.y += norm.y;
		Vertices[indices[i]].Normal.z += norm.z;
	}


	for (int i = 0; i < m*n; i++)
	{
		XMStoreFloat3(&Vertices[i].Normal, XMVector3Normalize(XMLoadFloat3(&Vertices[i].Normal)));

	}

	// Buffers
	HRESULT hr;

	//Vertexbuffer
	D3D11_BUFFER_DESC VertexBufferDesc;
	memset(&VertexBufferDesc, 0, sizeof(VertexBufferDesc));
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth = sizeof(Vertex)*nrVertrices;

	D3D11_SUBRESOURCE_DATA Vertexdata;
	Vertexdata.pSysMem = Vertices;
	hr = device->CreateBuffer(&VertexBufferDesc, &Vertexdata, &VertexB);

	// indexBuffer
	D3D11_BUFFER_DESC IndexBufferDesc;
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = 6 * (m - 1)*(n - 1) * 4;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA Indexdata;
	Indexdata.pSysMem = indices;
	hr = device->CreateBuffer(&IndexBufferDesc, &Indexdata, &IndexB);

}

void LandObject::renderFrustCull(ID3D11DeviceContext* devCont, const XMMATRIX &projection, const XMMATRIX &view)
{
	devCont->PSSetShaderResources(0, 1, &Tex);
	frustCull.Render(devCont,projection,view);
}

void LandObject::setupFrust(UINT m, UINT n,ID3D11Device* device)
{
	frustCull.Initialzie(indices, nrIndexes, device, m, n, Vertices,5,256,XMFLOAT3(0.0,0.0,0.0));
}