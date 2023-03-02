#include "Tape.h"
#include <exception>
#include <iostream>
#include <algorithm>
#include <limits>
#include "FileHandler.h"
#include <chrono>
#include <thread>
const std::string Tape::tmpDir = "../tmp/";
const std::string Tape::tmpFileName = "tmp.txt";
const std::string Tape::config = "config.txt";
Tape::Tape()
{
	configure();
}

void Tape::configure()
{
	try {
		std::ifstream cFile;
		cFile.open(config);
		auto par = FileHandler::get_next_str(cFile);
		if (par == "MaxRAMBt") {
			set_RAM_volume(std::stoul(FileHandler::get_next_str(cFile)));
		};
		par = FileHandler::get_next_str(cFile);
		if (par == "RWDelayNS") {
			RWDelayNS = std::stoul(FileHandler::get_next_str(cFile));
		};
		par = FileHandler::get_next_str(cFile);
		if (par == "ScrollDelayNS") {
			ScrollDelayNS = std::stoul(FileHandler::get_next_str(cFile));
		};

	}
	catch (std::exception& exp) {
		maxNumCountInRAM = 100;
		RWDelayNS = 0;
		ScrollDelayNS = 0;
	}
}

void Tape::do_rw_delay()
{
	std::this_thread::sleep_for(std::chrono::nanoseconds(RWDelayNS));
}

void Tape::do_scroll_delay()
{
	std::this_thread::sleep_for(std::chrono::nanoseconds(ScrollDelayNS));
}

void Tape::set_files(std::string in, std::string out)
{
	try{
		if(!FileHandler::is_file_exist(in)) throw std::exception("File doesn't exist");
		inFileName = in;
		outFileName = out;
	}
	catch (std::exception& exp) {
		std::cout << exp.what();
	}
}
void Tape::sort_tape() {
	try {
		make_sorted_parts();
		merge_sorted_parts();
	}
	catch (std::exception& exp) {
		std::cout << exp.what();
	}

}
void Tape::set_RAM_volume(unsigned long byte)
{ 
	maxNumCountInRAM = static_cast <unsigned long> (std::floor(byte / sizeof(int)));
}
void Tape::make_sorted_parts()
{
	std::ifstream inTape;
	unsigned long elCounter = 0;
	std::vector<int> numsInRAM(maxNumCountInRAM);
	int tmpEl;
	try {
		inTape.open(inFileName);
		for (;;) {
			//reading numbers
			do_scroll_delay();
			tmpEl = FileHandler::get_next_el(inTape);
			do_rw_delay();
			numsInRAM[++elCounter - 1] = tmpEl;
			//splitting into tmp files
			if (elCounter == maxNumCountInRAM) {
				make_sorted_part(numsInRAM);
				elCounter = 0;
			}
		}		
	}
	catch (std::out_of_range& exp) {
		//ending input file
		//processing the remaining numbers 
		if (elCounter != 0) {
			numsInRAM.resize(elCounter);
			make_sorted_part(numsInRAM);
		}
	}
	catch (std::exception& exp) {
		throw std::exception("Error reading input file");
	}
	inTape.close();
	
}

void Tape::make_sorted_part(std::vector<int> seq)
{
	std::sort(seq.begin(), seq.end());
	save_to_tmp_file(std::to_string(tmpsCount++), seq);
}


void Tape::save_to_tmp_file(std::string fileName, std::vector<int> seq)
{
	FileHandler::write<std::vector<int>>(tmpDir + fileName + tmpFileName, seq);
	for (int t = 0; t < seq.size(); t++) { do_rw_delay(); do_scroll_delay(); }
}

void Tape::merge_sorted_parts() {
	std::ofstream outTape;
	outTape.open(outFileName, std::ifstream::out);

	std::vector<std::ifstream> tmps(tmpsCount);
	std::vector<int> minNumFromParts(tmpsCount);
	//initialize minNums
	for (int i = 0; i < tmpsCount; i++) {
		tmps[i] = std::ifstream();
		tmps[i].open(tmpDir + std::to_string(i) + tmpFileName);
	}
	for (int i = 0; i < tmpsCount; i++) {
		if (!tmps[i].is_open()) continue;
		try {
			do_scroll_delay();
			minNumFromParts[i] = FileHandler::get_nextB_el(tmps[i]);
			do_rw_delay();
		}
		catch (std::out_of_range & exp) {
			tmps[i].close();
		}
	}
	//finding and pushing mins to outTape
	int min;
	unsigned long minPos;
	bool stop;
	int tmpEl;
	do {
		min = 2147483647;
		stop = true;
		for (int i = 0; i < tmpsCount; i++) {
			if (!tmps[i].is_open()) continue;
			if (minNumFromParts[i] <= min) {
				min = minNumFromParts[i];
				minPos = i;
				stop = false;
			}
		}
		if (stop) continue;

		FileHandler::push_el(outTape, min);
		try {
			do_scroll_delay();
			minNumFromParts[minPos] = FileHandler::get_nextB_el(tmps[minPos]);
			do_rw_delay();
		}
		catch (std::out_of_range & exp) {
			tmps[minPos].close();
		}
			
	} while (!stop);

	outTape.close();
}

void Tape::load_from_tmp_file(std::string fileName, std::vector<int>& seq)
{
	seq = FileHandler::read<std::vector<int>>(tmpDir + fileName + tmpFileName);
	for (int t = 0; t < seq.size(); t++) { do_rw_delay(); do_scroll_delay(); }
}


