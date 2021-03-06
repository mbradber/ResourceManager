#include "DisplayManager.h"
#include <cstring>

DisplayManager::DisplayManager()
{
}

void DisplayManager::init(ID3D10Device* device, std::list<std::string> lines)
{
	lineList = lines;

	std::list<std::string> uniqueList;
	//from the list of lines in the file, parse each line into nodes
	for(std::list<std::string>::iterator itr = lineList.begin(); itr != lineList.end(); itr++)
	{
		//convert the line to a c string
		char* tokenizeMe = new char[itr->size()+1];
		tokenizeMe[itr->size()] = NULL;
		memcpy(tokenizeMe, itr->c_str(), itr->size());
		char* token = strtok(tokenizeMe, " ");

		//search the pointers to node objects with the current string
		Node* temp = searchNodes(token);
		Node* headNode;
		//if this node doesn't exist, allocate a new node and assign a pointer to it
		if(temp == NULL)
		{
			Node* newNode = new Node(device, token, 1);
			headNode = newNode;
			nodeList.push_back(newNode);
			token = strtok(NULL, " ");
		}
		//if it does, mark it as the head node and grab the next token
		else
		{
			headNode = temp;
			token = strtok(NULL, " ");
		}

		//add any other tokens to the head token's depend list
		//as well as adding any unique tokens to the graph's node list
		while(token!=NULL)
		{
			//search the pointers to the node objects
			Node* tempNode = searchNodes(token);

			if(tempNode == NULL)
			{
				Node* newNode = new Node(device, token, 1);
				nodeList.push_back(newNode);
				headNode->dependList.push_back(newNode);
			}
			else
			{
				headNode->dependList.push_back(tempNode);
			}
			
			token = strtok(NULL, " ");
		}

	}
}

Node* DisplayManager::searchNodes(std::string nodeID)
{
	for(std::list<Node*>::iterator itr = nodeList.begin(); itr!= nodeList.end(); itr++)
	{
		Node* temp = *itr;
		if(temp->nodeID == nodeID)
			return temp;
	}

	return NULL;
}

DisplayManager::~DisplayManager(void)
{
}
