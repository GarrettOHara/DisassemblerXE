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
#include <map>

using namespace std;

std::map<std::string,int> ESTAB;

/**
 * 
 * This function will call two separate functions named:
 * - constructESTAB
 * - constructObjectFile
 * 
 * These two functions will do exaclty what you think
 * 
 * Data Structures for the ESTAB: HashTable
 *  - In c++ the easiest/standard lib way of implemnation 
 * is through an "unordered map"
 * Documentation:  http://www.cplusplus.com/reference/unordered_map/unordered_map/
 * 
 **/
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
        cout << "Error: Please specify a listing file to parse" << endl;
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
 * main method must be at bottom. Probably just when not in a class.
 * 
 * ELF/ in the output is the text file header, is holds entry addresses 
 * which is the first address in a .text region
 * use objdump-f to see the enty point
 * 
 * from: https://stackoverflow.com/questions/3102232/elf-file-format
 * 
 * Approach:
 * 
 * ESTAB
 * Create a ESTAB in the form of a map or hashtable data structure. 
 * on the first pass of the linker, parse each file and store the value in the 
 * map/hashtable along with the absoulte address
 *  - Absolute address can be calculated by the actual address of the program and 
 *    the relative address of the symbol
 * 
 * Object File:
 * 
 * 
 * Question: 
 *  - How do we know how long a text record line is
 *  - How do we know what kind of instruction something is format 3,4?
 *  - Is calculating the actual address really just the actual address of the
 *    program + the realative address of the symbol
 *  - How do we make the header record for the Object File
 *  - Do we only add the object code for the main program and then put the
 *    modification records for the external symbol references
 * 
 * 
 * Edge Cases: 
 *  - External Symbol that is not defined in the EXDEF is encountered
 *    program must cease execution giving the error (I can see him doing this case)
 *  - What if first pass we notice that the first file in the arguments list isnt main
 *    program?
 *      - I would assume hault execution and sepcify that you need ot have the main program
 *        first would be enough
 **/