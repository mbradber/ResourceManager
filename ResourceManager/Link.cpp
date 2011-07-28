#include "Link.h"
#include<vector>

Link::Link()
{
}

Link::Link(ID3D10Device* device, D3DXVECTOR4 &from, D3DXVECTOR4 &to):
numVerts(0), pDevice(0), vb(0)
{
	init(device, from, to);
}

void Link::deallocate()
{
	pDevice = NULL;
	vb->Release();//release the vertex buffer
	vb = NULL;
}


Link::~Link(void)
{

}

void Link::init(ID3D10Device* device, D3DXVECTOR4 &from, D3DXVECTOR4 &to)
{
	pDevice = device;
	numVerts = 4;

	D3DXVECTOR3 midPoint = (D3DXVECTOR3(from.x, from.y, from.z) + D3DXVECTOR3(to.x, to.y, to.z)) * 0.5f;

	std::vector<PosColVertex> v(numVerts);
	v[0] = PosColVertex(D3DXVECTOR3(from.x, from.y, from.z), BLACK);
	v[1] = PosColVertex(midPoint, BLACK);
	v[2] = PosColVertex(midPoint, RED);
	v[3] = PosColVertex(D3DXVECTOR3(to.x, to.y, to.z), RED);
	
	D3D10_BUFFER_DESC vbd;
    vbd.Usage = D3D10_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(PosColVertex) * numVerts;
    vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &v[0];
    pDevice->CreateBuffer(&vbd, &vinitData, &vb);
}

void Link::draw()
{
	UINT stride = sizeof(PosColVertex);
    UINT offset = 0;
    pDevice->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
	pDevice->Draw(numVerts, 0);
}
