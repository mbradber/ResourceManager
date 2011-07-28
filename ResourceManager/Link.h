#pragma once
#include"Utility.h"
#include"Vertex.h"

class Link
{
public:
	Link();
	Link(ID3D10Device* device, D3DXVECTOR4 &from, D3DXVECTOR4 &to);
	~Link(void);
	void init(ID3D10Device* device, D3DXVECTOR4 &from, D3DXVECTOR4 &to);

	void draw();
	void deallocate();

private:
	
	DWORD numVerts;
	ID3D10Device* pDevice;
	ID3D10Buffer* vb;
};

