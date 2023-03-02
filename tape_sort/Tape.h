#pragma once
#include "ITape.h"
#include <fstream>
#include <vector>

class Tape : public ITape
{
public:
	Tape();
	void set_files(std::string in, std::string out) override;
	void sort_tape() override;

private:

	static const std::string tmpDir;
	static const std::string tmpFileName;
	static const std::string config;
	unsigned int tmpsCount = 0;
	std::string inFileName;
	std::string outFileName;
	
	unsigned long maxNumCountInRAM;
	unsigned long RWDelayNS;
	unsigned long ScrollDelayNS;

	void set_RAM_volume(unsigned long byte);
	void configure();

	void make_sorted_parts();
	void merge_sorted_parts();

	void do_rw_delay();
	void do_scroll_delay();

	void make_sorted_part(std::vector<int> seq);
	void save_to_tmp_file(std::string fileName, std::vector<int> seq);
	void load_from_tmp_file(std::string fileName, std::vector<int> & seq);

};
