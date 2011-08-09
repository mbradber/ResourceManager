#include "GraphManager.h"
#include <cstring>
#include "FileIO.h"

GraphManager::GraphManager()
{
}

void GraphManager::init(ID3D10Device* device, std::list<std::string> lines)
{
	lineList = lines;

	//from the list of lines in the file, parse each line into nodes
	for(std::list<std::string>::iterator itr = lineList.begin(); itr != lineList.end(); itr++)
	{
		//if empty line, continue
		if(itr->size() < 1)
			continue;
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
		while(token != NULL)
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

		delete tokenizeMe;
	}
}

Node* GraphManager::searchNodes(std::string nodeID)
{
	for(std::list<Node*>::iterator itr = nodeList.begin(); itr!= nodeList.end(); itr++)
	{
		Node* temp = *itr;
		if(temp->nodeID == nodeID)
			return temp;
	}

	return NULL;
}

void GraphManager::updateNodes()
{
	FileIO io;

	std::string disabled;
	std::string deleted;

	std::list<std::string> disabledList;
	std::list<std::string> deletedList;

	std::vector<Node*> disabledVector;
	std::vector<Node*> deletedVector;

	io.getStatus(deleted, disabled);

	//convert the line to a c string
	char* tokenizeMe = new char[deleted.size()+1];
	tokenizeMe[deleted.size()] = NULL;
	memcpy(tokenizeMe, deleted.c_str(), deleted.size());
	char* token = strtok(tokenizeMe, ",");

	while(token != NULL)
	{
		deletedList.push_back(token);
		token = strtok(NULL, ",");
	}
	
	delete tokenizeMe;

	tokenizeMe = new char[disabled.size()+1];
	tokenizeMe[disabled.size()] = NULL;
	memcpy(tokenizeMe, disabled.c_str(), disabled.size());
	token = strtok(tokenizeMe, ",");

	while(token != NULL)
	{
		disabledList.push_back(token);
		token = strtok(NULL, ",");
	}

	for(std::list<std::string>::iterator itr = deletedList.begin(); itr != deletedList.end(); itr++)
	{
		Node* tempNode = searchNodes(*itr);
		tempNode->visible = false;
		tempNode->enabled = false;

		deletedVector.push_back(tempNode);
	}

	for(std::list<std::string>::iterator itr = disabledList.begin(); itr != disabledList.end(); itr++)
	{
		Node* tempNode = searchNodes(*itr);
		tempNode->enabled = false;

		disabledVector.push_back(tempNode);
	}

	//remove all links to the deleted nodes
	for(std::list<Node*>::iterator itr = nodeList.begin(); itr != nodeList.end(); itr++)
	{
		for(std::vector<Node*>::iterator it = deletedVector.begin(); it != deletedVector.end(); it++)
			(*itr)->dependList.remove(*it);
	}

	//remove the deleted nodes from the list
	for(std::vector<Node*>::iterator it = deletedVector.begin(); it != deletedVector.end(); it++)
		nodeList.remove(*it);
}

GraphManager::~GraphManager(void)
{
	for(std::list<Node*>::iterator itr = nodeList.begin(); itr!= nodeList.end(); itr++)
		delete *itr;
}
