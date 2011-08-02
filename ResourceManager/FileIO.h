#pragma once

#include<fstream>
#include<string>
#include<list>

class FileIO
{
public:
	FileIO();
	~FileIO();
	void writeFile(std::string file, std::string message);
	void readFile(std::string);
	void clearFile(std::string file);
	int getLines() const {return lineList.size();}
	std::list<std::string> getLineList() const {return lineList;}

private:
	std::ofstream out;
	std::ifstream in;
	int lines;

	std::list<std::string> lineList;
};
