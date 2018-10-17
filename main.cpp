#include <iostream>
#include <cinttypes>
#include <fstream>
#include <algorithm>
#include <ctime>

void fillbuffer(char *buffer, unsigned int buffersize) {
    std::srand(std::time(0));
    std::generate(buffer, buffer + buffersize, std::rand);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cout << "Usage: generate_huge_file <filename> <size-in-bytes> <zero|trash|trashrnd>" << std::endl;
        std::cout << "zero    - very fast" << std::endl;
        std::cout << "trash   - slow" << std::endl;
        std::cout << "zerornd - very slow" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    uintmax_t filesize = std::strtoumax(argv[2], nullptr, 10);
    std::string datatype = argv[3];

    std::ofstream outfile(filename, std::fstream::trunc | std::fstream::binary);

    if (!outfile.is_open()) {
        std::cerr << "Out file could't open!" << std::endl;
        return 1;
    }

    if (datatype == "zero") {
        outfile.seekp(filesize, std::fstream::beg);
        outfile.write("0", 1);
        outfile.close();
        std::cout << "Successfully create file with zero's!" << std::endl;
    } else {
        uintmax_t writebytes = 0;
        unsigned int buffersize = 32 * 1024 * 1024; // 32mb
        char *buffer = new char[buffersize];

        while (writebytes < filesize) {
            if (writebytes + buffersize > filesize) {
                buffersize = static_cast<unsigned int>(filesize - writebytes);
            }

            if (datatype == "trashrnd") {
                fillbuffer(buffer, buffersize);
            }

            outfile.write(buffer, buffersize);

            writebytes += buffersize;
        }

        std::cout << "Successfully create file with trash!" << std::endl;

        outfile.close();
    }

    return 0;
}