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
#include <sstream>
#include <map>
#include <vector>

//using namespace std;

std::map<std::string,int> ESTAB;                       // This map emulates a HashTable
std::vector<std::vector<std::string>> lines = {};      // This 2D vector holds the instructions


std::vector<std::string> split(const std::string str, char delim) {
    std::vector<std::string> result;
    std::istringstream ss{str};
    std::string token;
    while (getline(ss, token, delim)) {
        if (!token.empty()) {
            result.push_back(token);
        }
    }
    return result;
}

int printInstructions(){
        for(const std::vector<std::string> i : lines){
        for(const std::string j : i){
            std::cout << j << std::endl;
        }
        std::cout<<"\n";
    }
    return 0;
}

/**
 * 
 * This function (readFile) will call two separate functions named:
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
 * 
 * New Idea: (This was acheived with the 2D array/vector: lines)
 * 
 * Since the ReferenceTable is really not practicle and seems dumb
 * We can split each line by a space delininator
 * This way we will have all the information broke into separtate
 * string literals and store them in a list/vector/array of some type
 * 
 * given this sanitized line, we now have valuable information about it like:
 * - how long is the array
 *      This will tell us how many instructions/items are in the line perhaps
 *      understand what kind of instruction it is
 *      EX:
 *          - An array with two we know this is like
 *          00440   END
 * 
 * - We can parse the list for a certain instruction like WORD or RESW
 *      This will tell us if this line declares a Symbol or not
 *      If so, we can store the index of the array of like 1 (0 formatted)
 *      as the key in the ESTAB, or update the address value in the ESTAB
 *      with the local/relative address in the program 
 * 
 *      we will probably just throw all of the labels in the EXTDEF and EXTREF 
 *      lines in the ESTAB, then update the addresses with the actual
 *      addy of the program and the realtive address of he symbol in the program
 **/
int readFile(const char* input){
    std::ifstream file(input);
    std::string line;
    if (file.is_open()) {
        
        while (getline(file, line)) {
            lines.push_back(split(line, ' '));
        }

    }
    file.close();
    return 0;
}

int main(int argc, char *argv[]){
    printf("The size of argc is: %d\n",argc);
    if(argc < 2){
        std::cout << "Error: Please specify a listing file to parse" << std::endl;
        return 0;
    } else if(argc > 4){
        std::cout << "Error: Please provide 1-4 listing files to parse" << std::endl;
        return 0;
    }

    for(int i = 0; i < argc; i++){
        readFile(argv[i]);
    }
    printInstructions();
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
 * Object File
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