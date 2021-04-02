/**
 * This Program was written by:
 * 
 * Garrett O'Hara cssc3724 RedId: 822936303
 * Vivian Reyes  cssc3713 RedId: 821862269
 * 
 * CS 530 | Lenoard
 **/
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
//trying to include regex c++ lib #include <boost/regex.hpp>

using namespace std;


/**
 * Non initialized values could run into a runtime error
 * because there is no constructor
 **/
struct ESTABstruct{
    string controlSection;
    unsigned int address;
    unsigned int length;    
};

/**
 * This Struct holds the data associated with each instruction
 **/
struct Instruction{
    unsigned int relativeAddress;
    string symbolName;
    string instruction;
    string arguments;
    unsigned int objectCode;
};

/**
 * This unordered map/Hash map Data Structure will hold all of the instructions
 * in the source program
 * 
 * KEY:     realtive address in the program
 * 
 * Issues with relative address as the key:
 * - some instructions dont have an address
 * - some instructions do not increment the memory location (creates duplicates of memory)
 * Both of these issues break the map in a fundemental manner
 * 
 * VALUE:   each value is an instruction struct obj
 **/
map<string,Instruction> instructions;

/**
 * This Unordered map Data Structure holds the Symbol name as the key
 * and the address as the value
 * 
 * ISSUE: The integer needs be be expressed as a HEX value
 * 
 * Store a STRUCT as a value
 **/
map<string,ESTABstruct> ESTAB;           // This map emulates a HashTable

/**
 * This 2D data structure is used store all of the instructions as
 * tokenized strings. Each vector is an instruction line. Each vector is 
 * stored in the main vector calle `lines` 
 **/
vector<vector<string>> lines = {};      // This 2D vector holds the instructions

// std::regex re(","); NEED to change the char delim to a regex obj for multiple deliminators
vector<string> split(const string str, char delim) {
    vector<string> result;
    istringstream ss{str};
    string token;
    while (getline(ss, token, delim)) {
        if (!token.empty()) {
            result.push_back(token);
        }
    }
    return result;
}

int printInstructions(){
        for(const vector<string> i : lines){
        for(const string j : i){
            cout << j << endl;
        }
        cout<<"\n";
    }
    return 0;
}

void populateESTAB(){
    /**
     * This function needs to parse the lines vector for
     * EXTDEF and EXTREF then adds all the symbols defined here
     * to the ESTAB with the addresses realative to the program.
     * 
     * When the symbol is parsed in the first pass of the linker, the 
     * actual address is updated in the ESTAB
     **/
    return;
}

//Notes: need to figure when we start a new text record (max size or format 4?)
/**
 * Header Record: H, string of program name, starting address, length of program
 * Text Record: T, starting address of text record, length of text record line, object codes
 * Modification Record: M, relocation address, length, flag, segment name
 * End Record: E, starting address (only first control section/main program has end record)
 */
void writeObjectFile(){
    /**
     * How to do this? each record type could be a new funciton
     * 
     * Create Head record
     * 
     * Create Text record
     * 
     * Create Mod record
     * 
     * Create End record
     **/
    return;
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
    ifstream file(input);
    string line;
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
        cout << "Error: Please specify a listing file to parse" << endl;
        return 0;
    } else if(argc > 4){
        cout << "Error: Please provide 1-4 listing files to parse" << endl;
        return 0;
    }

    for(int i = 1; i < argc; i++){
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
 * 
 * Helpful Resources: 
 * Page 143 in the textbook (159 of PDF)
 * Shows the "toString" of the ESTAB which can also be refered to as load map
 * 
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
 * 
 * To Do's:
 *  - understand how texts records are generated (and header, mod, and end records)
 *  - how to read and store important values from listing files/source code
 *  - Garret idea: working on split function that will split ever line into strings and each 
 * line will be stored in a vector of vectors 
 *  - let's start brainstorming what exactly the object file needs
 * 
 * 
 **/