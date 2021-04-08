/**
 * This Program was written by:
 * 
 * Garrett O'Hara cssc3724 RedId: 822936303
 * Vivian Reyes  cssc3713 RedId: 821862269
 * 
 * CS 530 | Lenoard
 **/
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
//trying to include regex c++ lib #include <boost/regex.hpp>

using namespace std;

string programName;
/**
 * Non initialized values could run into a runtime error
 * because there is no constructor
 **/
struct ESTABdata{
    string controlSection;
    string instruction;
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
    vector<string> arguments;
    unsigned int objectCode;
};

/**
 * This vector holds all of the instructions in a source code file
 * - Each element is an Instruction struct
 **/
vector<Instruction> listingFile;

/**
 * This Unordered map Data Structure holds the Symbol name as the key
 * and the address as the value
 * 
 * ISSUE: The integer needs be be expressed as a HEX value
 * 
 * Store a STRUCT as a value
 **/
map<string,ESTABdata> ESTAB;           // This map emulates a HashTable

/**
 * This 2D data structure is used store all of the instructions as
 * tokenized strings. Each vector is an instruction line. Each vector is 
 * stored in the main vector calle `lines` 
 **/
vector<vector<string> > lines;
vector<string> sourceCode;

// std::regex re(","); NEED to change the char delim to a regex obj for multiple deliminators
vector<string> split(const string str, char delim) {
    vector<string> result;
    istringstream ss(str);
    string token;

    while (getline(ss, token, delim)) {
        if (!token.empty()) {
            result.push_back(token);
        }
    }
    return result;
}

int printInstructions(){
    printf("Instructions Deliminated:\n");
    printf("-------------------------\n");
	for(int i = 0; i < lines.size(); i++){
		vector<string> temp = lines[i];
		for(int j = 0; j < temp.size(); j++){
			cout << temp[j] << ' ';
		}
		printf("\n");
	}
    printf("-------------------------\n\n");
	return 0;
}
 void printSourceCode(){
    printf("Source Code:\n");
    printf("-------------------------\n");
	for(int i = 0; i < sourceCode.size(); i++){
        cout << sourceCode[i];
        printf("\n");
	}
    printf("-------------------------\n");
	return;
 }

void generateHeaderRecord(vector<string> instruct){
    
}
void generateEndRecord(vector<string> instruct){

}

void printESTAB(){
    /**
     * Order contents in map by address
     * Write to file
     * Call a check proper address function before
     * this to throw an error
     **/
    printf("\nESTAB:------------------------\n");
    map<string, ESTABdata>::iterator it;
    for(it = ESTAB.begin(); it != ESTAB.end(); it++){
        cout << it->first
             << ':'
             //<< it->second
             << endl;
    }
    return;
}

void generateESTAB(vector<string> vec, string instruction){
    /**
     * This function needs to parse the lines vector for
     * EXTDEF and EXTREF then adds all the symbols defined here
     * to the ESTAB with the addresses realative to the program.
     * 
     * When the symbol is parsed in the first pass of the linker, the 
     * actual address is updated in the ESTAB
     **/
    ESTABdata data;
    if(vec[0] == ".")
        return;
    if(vec[2] == "START"){
        programName = vec[1];
        data.controlSection = programName;
        data.address = atoi(vec[0].c_str());
        data.length = atoi(vec[2].c_str());
        ESTAB[programName] = data;
    }
    if(vec[2] == "EXTDEF" || vec[2] == "EXTREF"){
        string arg = vec[1];
        data.controlSection = programName;
        data.address = atoi(vec[0].c_str());
        data.instruction = arg;
        ESTAB[arg] = data;
    }
    if(vec[2] == "WORD" || vec[2] == "RESW"){
        string arg = vec[1];
        data.controlSection = programName;
        data.address = atoi(vec[0].c_str());
        data.instruction = arg;
        ESTAB[arg] = data;
    }
    
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
 * How to address column two of the listing file
 * 
 * Create a cosntant time lookup hashtable to check if the second element in the 
 * vector is a instruction 
 * if its not then its either the prorgam name or a custom (programmer defined) symbol
 * 
 * this will allow us to set the correct field for the struct and store it in the vector
 * 
 **/
void instructionParse(vector<string> instruct){
    Instruction structData;
    for(int i = 0; i < instruct.size(); i++){
        //if first item in index = END directive, special case, else first item = rel address
        string str = "END";
        if(str.compare(instruct[0]) == 0){
            generateEndRecord(instruct);
        }
        else{
            stringstream relAddr(instruct[0]);
            int x = 0;
            relAddr >> x;
            structData.relativeAddress = x;
        }
    }    
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
    vector<string> temp;
    if (file.is_open()) {
        while (getline(file, line)) {
            temp = split(line, ' ');
            lines.push_back(temp);
            sourceCode.push_back(line);
            instructionParse(temp);
//            generateESTAB(temp,line);
        }
    }
//    printESTAB();
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
    printSourceCode();
    return 0;
}

/**
 * This is a notes section
 * 
 * main method must be at bottom. Probably just when not in a class.
 * 
 * ELF/ in the outpusourceCodeddress of the symbol
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