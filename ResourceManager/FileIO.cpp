#include"FileIO.h"
#include<sstream>
#include<cstring>

FileIO::FileIO():
lines(0)
{}

FileIO::~FileIO()
{
	out.close();
}

void FileIO::writeFile(std::string fileName, std::string message)
{
	std::string currentText;
	std::string currentLine;
	std::ifstream input(fileName);

	//grab the current content of the file if it exists and add to it
	if(input.is_open())
	{
		while(input.good())
		{
			getline(input, currentLine);
			currentText += currentLine + "\n";
		}

		currentText += message;

		out.open(fileName);
		out << currentText;
	}

	//if it doesn't exist, open it and FileIO the message
	else
	{
		out.open(fileName);
		out << message;
	}

	out.close();
}

void FileIO::readFile(std::string fileName)
{
	std::string currentText;
	std::string currentLine;
	std::ifstream input(fileName);

	if(input.is_open())
	{
		while(input.good())
		{
			getline(input, currentLine);
			lineList.push_back(currentLine);
			currentText += currentLine + "\n";
		}
	}
}

void FileIO::clearFile(std::string file)
{
	out.open(file);
	out<< "";
	out.close();
}

void FileIO::getStatus(std::string& deleted, std::string& disabled)
{
	std::ifstream input("NodeStatus.txt");

	if(input.is_open())
	{
		for(int i = 0; i < 2; i++)
		{
			if(i == 0)
				getline(input, deleted);
			if(i == 1)
				getline(input, disabled);
		}
	}
}