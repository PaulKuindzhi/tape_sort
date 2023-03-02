#include <vector>

#include "pch.h"
#include "CppUnitTest.h"
#include "../tape_sort/FileHandler.h"
#include "../tape_sort/Tape.h"
#include "../tape_sort/Tape.cpp"
#include "../tape_sort/ITape.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

	//change fileDir to not absolute
	std::string fileDir = "D:/Projects/VS/tape_sort/tape_sort_tests/test_data/";
	std::string get_fN(unsigned int testNum) {
		return fileDir + std::to_string(testNum) + "test.txt";
	};
namespace filetests
{	
	
	TEST_CLASS(Binary)
	{
	public:
		TEST_METHOD(write_and_check_exist) {
			std::string fN = get_fN(1);

			FileHandler::write<std::vector<int>>(fN, std::vector<int>());
			Assert::IsTrue(FileHandler::is_file_exist(fN));
			FileHandler::del_file(fN);
		}

		TEST_METHOD(check_exist_not_exist_file) {
			std::string fN = get_fN(2);
			Assert::IsFalse(FileHandler::is_file_exist(fN));
		}

		TEST_METHOD(check_exist_exist_file) {
			std::string fN = get_fN(3);
			Assert::IsTrue(FileHandler::is_file_exist(fN));
		}

		TEST_METHOD(write_one_numb)
		{
			std::vector<int> seq = { 1 };
			std::string fN = get_fN(4);

			FileHandler::write<std::vector<int>>(fN, seq);
			auto res = FileHandler::read<std::vector<int>>( fN);
			for (int i = 0; i < seq.size(); i++)
				Assert::AreEqual(seq[i], res[i]);

			FileHandler::del_file(fN);
		}
		TEST_METHOD(write_seq)
		{
			std::vector<int> seq = { 1,2,3,4,5,6,7,8,9,10,1000,10000,100000,1000000 };
			std::string fN = get_fN(5);

			FileHandler::write<std::vector<int>>(fN, seq);
			auto res = FileHandler::read<std::vector<int>>(fN);
			for (int i = 0; i < seq.size(); i++)
				Assert::AreEqual(seq[i], res[i]);

			FileHandler::del_file(fN);
		}

		TEST_METHOD(big_seq) {
			std::string fN = get_fN(11);
			std::vector<int> seq(10000);
			for (int i = 0; i < seq.size(); i++) {
				seq[i] = std::rand();
			}
			FileHandler::write<std::vector<int>>(fN, seq);
			auto seq2 = FileHandler::read<std::vector<int>>(fN);
			for (int i = 0; i < seq.size(); i++) {
				Assert::AreEqual(seq[i], seq2[i]);
			}
		}
		TEST_METHOD(big_seq_with_get) {
			std::string fN = get_fN(12);
			std::vector<int> seq(10000);
			FileHandler::write<std::vector<int>>(fN, seq);

			std::ifstream f;
			f.open(fN);
			for (int i = 0; i < seq.size(); i++) {
				Assert::AreEqual(seq[i],FileHandler::get_nextB_el(f));
			}
		}
	};

	TEST_CLASS(GetNext)
	{
	public:
		TEST_METHOD(read_elem) {
			std::string fN = get_fN(6);
			std::ifstream f;
			f.open(fN);
			Assert::AreEqual(FileHandler::get_next_el(f), 515);
		}

		TEST_METHOD(read_seq) {
			std::string fN = get_fN(7);
			std::ifstream f;
			f.open(fN);
			std::vector<int> seq = { 1,2,3,4,5,6,7,8,9,10,1000,10000,100000,1000000 };
			for (int i = 0; i < seq.size(); i++)
				Assert::AreEqual(seq[i], FileHandler::get_next_el(f));
		}

		TEST_METHOD(read_2_elems) {
			std::string fN = get_fN(8);
			std::ifstream f;
			f.open(fN);
			std::vector<int> seq = { 1,2 };
			for (int i = 0; i < seq.size(); i++)
				Assert::AreEqual(seq[i], FileHandler::get_next_el(f));
		}

		TEST_METHOD(big_seq) {
			std::string fN = get_fN(13);
			std::vector<int> seq(10000);
			std::ofstream f;
			f.open(fN);
			for (int i = 0; i < seq.size(); i++) {
				seq[i] = std::rand();
				FileHandler::push_el(f, seq[i]);
			}
			
			f.close();
			std::ifstream f2;
			f2.open(fN);
			for (int i = 0; i < seq.size(); i++) {
				Assert::AreEqual(seq[i], FileHandler::get_next_el(f2));
			}
			f.close();
		}
	};

};
