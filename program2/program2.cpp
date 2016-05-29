#include <iostream>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <fstream>

#include "LFS.h"

void print_usage()
{
    std::cout << "Usage:\n" << std::endl;
    std::cout << "program2 LFS" << std::endl;
    std::cout << "\n" << std::endl;
    std::cout << "-U     Print this Usage\n" << std::endl;
    std::cout << "\n" << std::endl;
    std::cout << "import <filename> <lfs_filename> \n" << std::endl;
    std::cout << "remove <lfs_filename>\n" << std::endl;
    std::cout << "cat <lfs_filename>\n" << std::endl;
    std::cout << "display <lfs_filename> <howmany> <start>\n" << std::endl;
    std::cout << "overwrite <lfs_filename> <howmany> <start> <c>\n" << std::endl;
    std::cout << "list\n" << std::endl;
    std::cout << "exit\n" << std::endl;
    std::cout << "\n" << std::endl;
}

int main(int argc, char* argv[]) 
{
    LFS disk;
    while (true)
    {
        std::cout << "> ";

        std::string command;

        std::cin >> command;

        if (command.size() == 0)
            continue;

        ///run the command here
        if (command == "import") {
            std::string filename, lfs_filename;
            std::cin >> filename >> lfs_filename;

            std::ifstream dataStream(filename, std::ios::in | std::ios::binary);
            if(!dataStream.good())
                std::cout << "File does not exist" << std::endl;
            else disk.import(lfs_filename, dataStream);
        } else if(command == "remove"){
            std::string lfs_filename;

            std::cin >> lfs_filename;
            if(!disk.findFile(lfs_filename)) 
                std::cout << "File does not exist" << std::endl;
            else disk.remove(lfs_filename);
        } else if (command == "cat") {
            std::string lfs_filename;

            std::cin >> lfs_filename;
            if(!disk.findFile(lfs_filename)) 
                std::cout << "File does not exist" << std::endl;
            else std::cout << disk.cat(lfs_filename) << std::endl;
        } else if (command == "display") {
            std::string lfs_filename;
            int howmany;
            int start;

            std::cin >> lfs_filename;
            std::cin >> howmany >> start;

            if(!disk.findFile(lfs_filename)) 
                std::cout << "File does not exist" << std::endl;
            else 
                std::cout << disk.display(lfs_filename, howmany, start) << std::endl;
        } else if (command == "overwrite") {
            std::string lfs_filename;
            int howmany;
            int start;
            char c;

            std::cin >> lfs_filename;
            std::cin >> howmany >> start >> c;

            if(!disk.findFile(lfs_filename)) 
                std::cout << "File does not exist" << std::endl;
            else  
                disk.overwrite(lfs_filename, howmany, start, c);
        } else if (command == "exit") {
            disk.flush();
            break;
        } else if (command == "list") {
            std::cout << disk.list() << std::endl;
        } else {
            std::cerr << "Invalid command: " << command << std::endl;
        }
    }
    return 0;
}
