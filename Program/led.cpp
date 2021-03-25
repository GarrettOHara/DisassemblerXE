/**
 * This Program was written by:
 * 
 * Garrett O'Hara cssc3724 RedId: 822936303
 * Vivian  Reyes  cssc3713 RedId: 821862269
 * 
 * CS 530 | Lenoard
**/
#include <iostream>
#include <fstream>

using namespace std;

int readFile(const char* input){
    std::ifstream file(input);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            printf("%s", line.c_str());
        }
        printf("\n");
    }
    file.close();
    return 0;
}

int main(int argc, char *argv[]){
    printf("The size of argc is: %d\n",argc);
    if(argc < 2){
        cout << "User Error: Please specify a listing file to parse" << endl;
        return 0;
    } else if(argc > 4){
        cout << "Error: Please provide 1-4 listing files to parse" << endl;
        return 0;
    }

    for(int i = 0; i < argc; i++){
        readFile(argv[i]);
    }
    return 0;
}

/**
 * This is a notes section
 * 
 * ELF/ in the output is the text file header, is holds entry addresses 
 * which is the first address in a .text region
 * use objdump-f to see the enty point
 * 
 * from: https://stackoverflow.com/questions/3102232/elf-file-format
 * 
 * test
 * 
 **/