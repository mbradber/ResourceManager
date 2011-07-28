#pragma once
#include<list>
#include<string>
#include"Utility.h"
#include"Node.h"

class DisplayManager
{
public:
	DisplayManager();
	~DisplayManager(void);

	void init(ID3D10Device*, std::list<std::string>);
	std::list<Node*> nodeList;

private:
	std::list<std::string> lineList;
	ID3D10Device* pDevice;
	Node* searchNodes(std::string);
};

