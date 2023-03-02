
#include <iostream>
#include "ITape.h"
#include "Tape.h"
#include "FileHandler.h"
using namespace std;
int main()
{
    std::cout << "Enter input file: ";
    std::string in, out;
    std::cin >> in;
    std::cout << "Enter output file: ";
    std::cin >> out;
    Tape tape;
    tape.set_files(in, out);
    //tape.set_files("test_data/in.txt", "test_data/out.txt");
    tape.sort_tape();
    
}
