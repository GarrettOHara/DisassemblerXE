/**
 * This Program was written by:
 * 
 * Garrett O'Hara cssc3724 RedId: 822936303
 * Vivian Reyes  cssc3713 RedId: 821862269
 * 
 * CS 530 | Professor Lenoard | March 2021
 **/
#include <stdio.h>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <map>
#include <vector>
#include <set>
#include <algorithm>

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
 * Object File Line is the Parent Struct of all of the 
 * differnt types of object file liens there can be
 * 
 * each object record posesses different attributes and needs
 * its own struct
 **/
struct ObjectFileLine{
    struct HeaderRecord{
        string controlSectionName;
        unsigned int actualAddress;
        unsigned int length;
    };
    struct DefinitionRecord{
        vector<string> symbols;
        vector<unsigned int> addresses;
    };
    struct ReferenceRecord{
        vector<string> symbols;
    };
    struct TextRecord{
        unsigned int relativeAddress;
        string symbolName;
        string instruction;
        vector<string> arguments;
        vector<unsigned int> objectCodes;
    };
    struct ModificationRecord{
        unsigned int length;
        unsigned int nibbles;
        string mod;
    };
    struct EndRecord{
        unsigned int address;
    };
    struct HeaderRecord header;
    struct DefinitionRecord definition;
    struct ReferenceRecord reference;
    struct TextRecord text;
    struct ModificationRecord modification;
    struct EndRecord end;

    unsigned int relativeAddress;   
};

/**
 * This vector holds all of the instructions in a source code file
 * - Each element is an Instruction struct
 **/
vector<ObjectFileLine> listingFile;

/**
 * This Unordered map Data Structure holds the Symbol name as the key
 * and the address as the value
 **/
map<string,ESTABdata> ESTAB;

/**
 * This vector is used to memorize the insertion order within the ESTAB
 **/
vector<string> insertionOrder;

/**
 * This 2D data structure is used store all of the instructions as
 * tokenized strings. Each vector is an instruction line. Each vector is 
 * stored in the main vector calle `lines` 
 **/
vector<vector<string> > lines;
/**
 * This vector stores all of the lines of the source code as strings
 **/
vector<string> testSourceCode;

/**
 * This variable updates the global memory scope when reading listing files
 **/
unsigned int memoryLocation = 0;
unsigned int MEMORY;

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
    printf("-------------------------\n");
	return 0;
}
 void printSourceCode(){
    printf("Source Code:\n");
    printf("-------------------------\n");
	for(int i = 0; i < testSourceCode.size(); i++){
        cout << testSourceCode[i];
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
 void printIndexes(vector<string> sourceCode){
     printf("Testing Indexes\n");
     printf("-------------------------\n");
     string line;
     for(int i = 3; i < 5; i++){
        string line (sourceCode[i].c_str());
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

void printESTAB(){
    ofstream ESTABfile;
    ESTABfile.open("ESTAB.st");
    for(int i = 0; i < insertionOrder.size(); i++){
        string str = insertionOrder[i];
        if(str == "")
            continue;
        else if (ESTAB[str].instruction == ""){
            unsigned int len = ESTAB[str].length;
            unsigned int addy = ESTAB[str].address;

            stringstream streamA;
            streamA << hex << addy;
            string address( streamA.str() );

            stringstream streamB;
            streamB << hex << len;
            string length( streamB.str() );


            ESTABfile << str
                 << "       "
                 << setw(6)
                 << setfill('0')
                 << address
                 << " "
                 << setw(6)
                 << length
                 << endl;
        }
        else if(ESTAB[str].instruction != ""){
            stringstream stream;
            stream << hex << ESTAB[str].address;
            string address( stream.str());

            ESTABfile << " "
                 << setw(10)
                 << setfill(' ')
                 << str
                 << " "
                 << setw(6)
                 << setfill('0')
                 << address
                 << endl;
        }
    }
    return;
}

void parseESTAB(){
    map<string,ESTABdata>::iterator it;
    for(it = ESTAB.begin(); it != ESTAB.end(); it++){
        string reference = it->first;
        
        string controlSection = ESTAB[reference].controlSection;
        string instruction = ESTAB[reference].instruction;
        unsigned int address = ESTAB[reference].address;
        unsigned int length = ESTAB[reference].length;

        if(instruction == "")
            continue;
        else {
            unsigned int lowerBound = ESTAB[controlSection].address;
            unsigned int upperBound = ESTAB[controlSection].length + lowerBound;

            if(address < lowerBound || address > upperBound){
                string exceptionMessage = "ERROR: You are accessing illegal memory.";
                throw(exceptionMessage);
            }
        }
    }
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
    
    if(vec.size() == 0 || vec[0] == ".")
        return;
    else if(vec.size() < 3)     //When encounting the end record
        return;
    else if(vec[2] == "START"){
        unsigned int address;
        unsigned int length;
        istringstream converter(vec[0].c_str());
        converter >> hex >> address;
        istringstream cast(vec[2].c_str());
        cast >> hex >> length;

        programName = vec[1];
        data.controlSection = programName;
        data.address = address + memoryLocation;
        ESTAB[programName] = data;
        insertionOrder.push_back(programName);
    }
    else if(vec[1] == "EXTDEF"){

        vector<string> temp = split(vec[2], ',');
        for(vector<string>::size_type i = 0; i != temp.size(); i++){
            unsigned int address;
            istringstream converter(vec[0].c_str());
            converter >> hex >> address;

            data.address = address;
            data.controlSection = programName;
            data.instruction = temp[i];
            ESTAB[temp[i]] = data;

            /*
             * Insert in vector if not already present
             */
            if(find(insertionOrder.begin(), insertionOrder.end(), temp[i]) != insertionOrder.end()){
                return;            
            } else {
                insertionOrder.push_back(temp[i]);
            }
        }
        

    }
    else if(vec[2] == "WORD" || vec[2] == "RESW"){
        string arg = vec[1];
        if(ESTAB.count(arg)){
            unsigned int address;
            istringstream converter(vec[0].c_str());
            converter >> hex >> address;

            data.controlSection = programName;
            data.address = address + memoryLocation;
            data.instruction = arg;
            ESTAB[arg] = data;
            if(find(insertionOrder.begin(), insertionOrder.end(), arg) != insertionOrder.end()){
                return;
            } else {
                insertionOrder.push_back(arg);
            }
        }
    }
    else if(vec[2] == "=C'EOF'"){
        unsigned int length = atoi(vec[0].c_str());
        istringstream converter(vec[0].c_str());
        converter >> hex >> length;
        
        ESTAB[programName].length = length+3;
        MEMORY = length+3;
        if(memoryLocation == 0)
            memoryLocation = length+3;
        else
            memoryLocation += length;
    }
    return;
}

void generateHeaderRecord(vector<string> sourceCode, vector<vector<string> > tokenized, string file){
    //string temp = file;
    string temp = file.substr(0, file.find(".",0));
    temp += ".obj";


    ofstream objectFile;
    objectFile.open(temp.c_str());

    for(int i = 0; i < tokenized.size(); i++){

        if(tokenized[i][0] == ".")
            continue;
        else if(tokenized[i].size() < 3)     //When encounting the end record
            continue;
        else if(tokenized[i][2] == "START"){
            string reference = tokenized[i][1];
            unsigned int address = ESTAB[reference].address;
            unsigned int length = ESTAB[reference].length;
            
            stringstream stream;
            stream << hex << address;
            string addy(stream.str());
            stream << hex << length;
            string len(stream.str());

            objectFile << "H"
                 << "^"
                 << reference
                 << "^"
                 << setfill('0')
                 << setw(6)
                 << addy
                 << "^"
                 << setw(6)
                 << len
                 << endl;
        }
    }
    objectFile.close();
    return;
}

void generateDefinitionRecord(vector<string> sourceCode, vector<vector<string> > tokenized){
    
}

//we generate one line of the text record
//should return new index
//created 3/10 by viv
int generateTextRecord(int index, vector<string> sourceCode, vector<vector<string> > tokenized){
    ObjectFileLine data;
    //will need some sort of counter to make sure we are not going over size IE
    int counter;

    //store rel address
    unsigned int relAddress = atoi((tokenized.at(index).at(0).c_str()));
    data.text.relativeAddress = relAddress;

    //what will our loop condition be?
    //counter < IE (hex))
    while(true)
    {
        vector<string> instruct = tokenized.at(index);
        //if line has a symbol name then index 1 = symnbol name and index 2 = instruction
        //
        string line = sourceCode[index];
        if(line.at(8) != ' ')
        {
            data.text.symbolName = tokenized[index][1];
            data.text.instruction = tokenized[index][2];
            data.text.arguments.push_back(tokenized[index][3]);
            //not always the case for this need to check bounds
            if(instruct.size() > 3){
                stringstream temp(tokenized[index][4]);
                unsigned int x = 0;
                temp >> x;
                data.text.objectCodes.push_back(x);
            }
        }
            
        else{
            data.text.instruction = tokenized[index][1]; //if no second column instruction is at index 1 of instruct vector
            data.text.arguments.push_back(tokenized[index][2]);
            //check for bounds
            if(instruct.size() > 3){
                stringstream temp(tokenized[index][3]);
                unsigned int x = 0;
                temp >> x;
                data.text.objectCodes.push_back(x);
            }
        }
        //also need to be filling the TextRecord struct of every loop
        //increment index 
        index++;
    }
    return index;   
}

void generateModificationRecord(vector<string> sourceCode, vector<vector<string> > tokenized, ofstream objectFile){
    
}

void generateEndRecord(vector<string> sourceCode, vector<vector<string> > tokenized, string file){
    string temp = file.substr(0, file.find(".",0));
    temp += ".obj";

    ofstream objectFile;
    objectFile.open(temp.c_str(), ios_base::app);

    bool twoPass = false;
    string symbol;
    for(int i = 0; i < tokenized.size(); i++){

        if(tokenized[i][0] != "END")
            continue;
        else if(tokenized[i].size() == 1)
            objectFile << "E" << endl;
        else {
            symbol = tokenized[i][1];
            twoPass = true;
        }
    }
    if(twoPass){
        string address;
        for(int i = 0; i < tokenized.size(); i++){

            if(tokenized[i][2] == symbol)
                address = tokenized[i][0];
            if(tokenized[i][0] == "END"){
                objectFile << "E"
                           << "^"
                           << address
                           << endl;
            }
        }
    }
    objectFile.close();
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
ObjectFileLine instructionParse(vector<string> sourceCode, vector<vector<string> > tokenized){
    struct ObjectFileLine structData;

    //call header, definiton, ref record here

    //line 4 is where program starts, estab take getting taken care of separately
    //do avoid hard coding int 4, find where First starts
    //3/10: this will need to be a nested for loop?
    for(int i = 4; i < sourceCode.size(); i++){
        //if first item in index = END directive, special case, else first item = rel address
        // string str = "END";
        // if(str.compare(tokenized.at(i).at(0) == 0)){
        //     generateEndRecord(sourceCode, tokenized);
        //     break;
        // }
        if(tokenized.at(i).at(0) == "END"){
            //generateEndRecord(sourceCode, tokenized);
            break;
        }
        //generateTextRecord(index, sourceCode, tokenized);
    }
        
    return structData;    
}
int readFileESTAB(const char* input){
    ifstream file(input);
    string line;
    vector<string> temp;
    if (file.is_open()) {
        while (getline(file, line)) {
            temp = split(line, ' ');
            lines.push_back(temp);
            testSourceCode.push_back(line);
            generateESTAB(temp,line);
        }
    }
    try{
        parseESTAB();
    } catch(string error){
        cout << error << endl;
        exit(0);
    }

    printESTAB();
    file.close();
    return 0;
}

void readFileObjectFile(const char* input){
    map<string,ObjectFileLine> instructionMap;

    string temp = input;
    string fileName = temp.substr(0,temp.find(".",0));

    ifstream file(input);
    string line;
    vector<vector<string> > tokenized;
    vector<string> sourceCode;
    if (file.is_open()) {
        while (getline(file, line)) {
            vector<string> temp;
            temp = split(line, ' ');
            tokenized.push_back(temp);
            sourceCode.push_back(line);
        }
    }
    file.close();

    // string temp = input;
    // temp += ".obj";

    // ofstream objectFile;
    // objectFile.open(temp.c_str());

    generateHeaderRecord(sourceCode, tokenized, input);
    generateEndRecord(sourceCode, tokenized, input);
    // instructionParse(sourceCode, tokenized);
    // //Call the generate Object file function here
    // printIndexes(sourceCode);
    //objectFile.close();
    return;
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
        readFileESTAB(argv[i]);
        readFileObjectFile(argv[i]);
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
 **/