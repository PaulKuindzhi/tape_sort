#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

class FileHandler 
{
public:
	static bool is_file_exist(std::string fileName) noexcept {
		std::fstream file(fileName);
		return (bool) file;

	}
	static void del_file(std::string fileName) {
		if (!is_file_exist(fileName)) return;
		std::remove(fileName.c_str());
	}

	static std::ifstream * iOpen(std::string inFileName) {
		if (!is_file_exist(inFileName)) throw std::exception("File doesn't exist");
		std::ifstream file(inFileName);
		file.open(inFileName, std::ios::in);
		return &file;
	}

	static int get_next_el(std::ifstream & inFile) {
		try {
			std::string line;
			std::getline(inFile, line);
			return std::stoi(line);
		}
		catch (std::exception exp) {
			throw std::out_of_range("There is no next element");
		}
		
	}

	static std::string get_next_str(std::ifstream& inFile) {
		try {
			std::string line;
			std::getline(inFile, line);
			return line;
		}
		catch (std::exception exp) {
			throw std::out_of_range("There is no next element");
		}

	}

	static int push_el(std::ofstream& oFile, int el) {
		if (!oFile.is_open()) throw std::exception("File isn't open");
		oFile << el << std::endl;
	}


	//near for binary files ->
	static int get_nextB_el(std::ifstream& inFile) {
		try {
			int el ;
			inFile.read((char*)&(el), sizeof(int));
			if (inFile.eof()) 
				throw std::exception();
			return el;
		}
		catch (std::exception exp) {
			throw std::out_of_range("There is no next element");
		}
	}

	template <typename Container>
	static Container read(std::string inFileName) {
		if (!is_file_exist(inFileName)) throw std::exception("File doesn't exist");

		Container container;
		typedef decltype(*std::begin(container)) dataType;
		std::ifstream file;
		auto curEl = 0;

		file.open(inFileName, std::ios::binary | std::ios::in);
		for (;;) {
			file.read((char*)&((dataType)curEl), sizeof(dataType));
			if (file.eof()) break;
			container.push_back(curEl);
		}
		file.close();
		return container;
	};

	template <typename Container>
	static void write(std::string outFileName, Container container) {
		std::ofstream file;
		file.open(outFileName, std::ios::binary | std::ios::out);
		typedef decltype(*std::begin(container)) dataType;

		std::for_each(container.cbegin(), container.cend(),
			[&file](const int & curNum) {
				file.write((char*)&curNum, sizeof(dataType)); 
			});
		file.close();
	};

};