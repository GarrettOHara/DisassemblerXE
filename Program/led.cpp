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
#include <iomanip>
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
struct ObjectFileLine{
    HeaderRecord header;
    DefinitionRecord definition;
    ReferenceRecord reference;
    TextRecord text;
    ModificationRecord modification;
    EndRecord end;

    unsigned int relativeAddress;
    string symbolName;
    string instruction;
    //vector<string> arguments;
    string arguments;
    unsigned int objectCode;


    //Fields For header record
    string controlSectionName;
    unsinged int actualAddress;
    unsided int length;

    //Text Record fields
    
};

struct HeaderRecord{

};
struct DefinitionRecord{

};
struct ReferenceRecord{

};
struct TextRecord{

};
struct ModificationRecord{

};
struct EndRecord{

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

/*
    Idea on indetifying what is in each column
    - we already have every string in a 2D vector, but not all the indexes and types match up
    - so we are going to compare each line at certain indexes and determine if there is a space or a char 
    - and depending on that we should know what goes where into the struct values, probs with a lot of if statments unfortantely
    - index 8 = labels, index 16/17 = instruction, index 25/26 = arguments, index 51 = address
*/
 void printIndexes(){
     printf("Testing Indexes\n");
     printf("-------------------------\n");
     std::string line;
     for(int i = 3; i < 5; i++){
        std::string line (sourceCode[i]);
        cout << "Char at index 8, labels: " << line.at(8) << endl;
        cout << "Char at index 16, instructions: "<< line.at(16) << endl;
        cout << "Char at index 17, instructions: " << line.at(17) << endl;
        cout << "Char at index 25, arguments: " << line.at(25) << endl;
        cout << "Char at index 26, arguments: " << line.at(26) << endl;
        cout << "Char at index 51, object code: " << line.at(51) << endl;
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
    printf("\nESTAB:\n------------------------\n");
    map<string, ESTABdata>::iterator it;
    for(it = ESTAB.begin(); it != ESTAB.end(); it++){
        if(it->first == "")
            continue;
        else if (it->second.instruction == ""){
            cout << it->first
                 << setw(10)
                 << setfill(' ')
                 << setw(5)
                 << setfill('0')
                 << " "
                 << it->second.address
                 << setw(3)
                 << it->second.length
                 << endl;
        }
        else if(it->second.length == 0){
            cout << setw(10)
                 << it->first
                 << setw(5)
                 << it->second.instruction
                 << endl;
        }
    }
    printf("\n------------------------\n\n");
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
    cout << "Contents of vect:" << vec[0] << endl;
    if(vec.size() == 0 || vec[0] == ".")
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
    if(vec[2] == "=C'EOF'"){
        unsigned int val = ESTAB[programName].length;
        unsigned int length = 29;//atoi(vec[2].c_str());
        cout << "\n\n" << setfill('0')
             << val
             << setw(3)
             << length << endl;
        ESTAB[programName].length = length - val;
    }
    
    return;
}

//we generate one line of the text record
//should return new index
//created 3/10 by viv
int generatedTextRecord(int index, vector<string> sourceCode, vector<vector<string>> tokenized){

        //will need some sort of counter to make sure we are not going over size IE
        int counter;

        //store rel address
        stringstream relAddr(instruct[0]);
        int x = 0;
        relAddr >> x;
        structData.relativeAddress = x;

        //what will our loop condition be?
        while(//counter < IE (hex))
        {
            //if line has a symbol name then index 1 = symnbol name and index 2 = instruction
            //
            string line;
            line = sourceCode[index];
            if(!(line.at(8) == ' '))
            {
                structData.symbolName = tokenized[index][1];
                structData.instruction = tokenized[index][2];
                structData.arguments = tokenized[index][3];
                //not always the case for this need to check bounds
                if(instruct.size() > 3){
                    stringstream temp(tokenized[index][4]);
                    unsigned int x = 0;
                    temp >> x;
                    structData.objectCode = x;
                }
            }
                
            else{
                structData.instruction = tokenized[index][1]; //if no second column instruction is at index 1 of instruct vector
                structData.arguments = tokenized[index][2]; 
                //check for bounds
                if(instruct.size() > 3){
                    stringstream temp(tokenized[index][3]);
                    unsigned int x = 0;
                    temp >> x;
                    structData.objectCode = x;
                }
            }
            //also need to be filling the TextRecord struct of every loop
            //increment index 
            index++;
        }
        return index;   
    }
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
 * Note 4/8: not making multiple instances of this struct, creating one and overriding it each time,
 * wherever you initialize you can also use it in the scope of the function, 
 * but I wonder if we can still access it if we pass the struct to other functions, maybe have the function return a struct?
 * Note 4/9: make new function for header record (needs starting address and length of program)
 * //2D vector to compare
 **/
Instruction instructionParse(vector<string> sourceCode, vector<vector<string>> tokenized){
    struct Instruction structData; //objectfileline

    //call header, definiton, ref record here

    //line 4 is where program starts, estab take getting taken care of separately
    //do avoid hard coding int 4, find where First starts
    //3/10: this will need to be a nested for loop?
    for(int i = 4; i < sourceCode.size(); i++){
        //if first item in index = END directive, special case, else first item = rel address
        string str = "END";
        if(str.compare(instruct[0]) == 0){
            generateEndRecord(instruct);
            break;
        }
        //generateTextRecord(index, sourceCode, tokenized )
    return structData;    
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
 * 
 *      
 **/
int readFile(const char* input){
    map<string,Instruction> instructionMap;  
    //make vector instead of map that holds instruction struct objects
    struct Instruction tempStruct;
    ifstream file(input);
    string line;
    vector<string> temp;
    if (file.is_open()) {
        while (getline(file, line)) {
            temp = split(line, ' ');
            lines.push_back(temp);
            sourceCode.push_back(line);
            instructionParse(temp);
            generateESTAB(temp,line);
        }
    }
    printESTAB();
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
    printIndexes();
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
 **/