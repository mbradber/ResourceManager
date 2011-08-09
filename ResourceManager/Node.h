#pragma once
#include<list>
#include"Cube.h"
#include"Utility.h"

class Node
{
public:
	Node(ID3D10Device* device, std::string, float scale);
	~Node(void);

	void draw();
	void loadTexture();

	std::list<Node*> dependList;
	std::string nodeID;
	D3DXVECTOR4 position;

	ID3D10ShaderResourceView* textureSRV;
	ID3D10RenderTargetView* textureRTV;

	bool visible;
	bool enabled;

private:
	void buildCube();

	ID3D10Device* pDevice;
	float cubeScale;
	Cube cube;

};

