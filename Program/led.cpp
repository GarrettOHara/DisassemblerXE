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
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

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
 * REMOVE THIS AND TRIVIAL PRINT FUNCTIONS AT END
 **/
vector<string> testSourceCode;

/**
 * This variable updates the global memory scope when reading listing files
 **/
unsigned int memoryLocation = 0;
string programName;

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
vector<string> splitString(const string str){
    vector<string> tokens;
    string alpha;
    for (int i=0; i<str.length(); i++)
    {
        if (isdigit(str[i]))
            alpha.push_back(str[i]);
        else if((str[i] >= 'A' && str[i] <= 'Z') ||
                (str[i] >= 'a' && str[i] <= 'z'))
            alpha.push_back(str[i]);
        else{
            if(alpha != ""){
                tokens.push_back(alpha);
                alpha.clear();
            }
        }
    }
    if(str!="")
        tokens.push_back(alpha);
    return tokens;
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
            unsigned int upperBound = ESTAB[controlSection].length+lowerBound;

            if(address < lowerBound || address > upperBound){
                string exceptionMessage = "ERROR: You are "
                    "accessing illegal memory.";
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
            if(find(insertionOrder.begin(), insertionOrder.end(), 
                    temp[i]) != insertionOrder.end()){
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
            if(find(insertionOrder.begin(), insertionOrder.end(), arg) 
                    != insertionOrder.end()){
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
        if(memoryLocation == 0)
            memoryLocation = length+3;
        else
            memoryLocation += length;
    }
    return;
}

void generateHeaderRecord(vector<vector<string> > tokenized, string file){
    string temp = file.substr(0, file.find(".",0));
    temp += ".obj";


    ofstream objectFile;
    objectFile.open(temp.c_str());

    for(int i = 0; i < tokenized.size(); i++){

        if(tokenized[i][0] == ".")       //skip comments
            continue;
        else if(tokenized[i].size() < 3) //skip END record
            continue;
        else if(tokenized[i][2] == "START"){
            string reference = tokenized[i][1];
            unsigned int address = ESTAB[reference].address;
            unsigned int length = ESTAB[reference].length;
            
            stringstream streamA;
            stringstream streamB;
            streamA << hex << address;
            string addy(streamA.str());
            streamB << hex << length;
            string len(streamB.str());

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

void generateDefinitionRecord(vector<vector<string> > tokenized, string file){
    string temp = file.substr(0, file.find(".",0));
    temp += ".obj";

    ofstream objectFile;
    objectFile.open(temp.c_str(), ios_base::app);

    for(int i = 0; i < tokenized.size(); i++){
        if(tokenized[i][0] == ".")
            continue;
        else if(tokenized[i].size() < 3)
            continue;
        else if(tokenized[i][1] == "EXTDEF"){
            objectFile << "D";
            vector<string> symbols = split(tokenized[i][2], ',');
            for(int i = 0; i < symbols.size(); i++){
                unsigned int address = ESTAB[symbols[i]].address;
                
                stringstream stream;
                stream << hex << address;
                string addy(stream.str());

                objectFile << "^"
                           << symbols[i]
                           << "^"
                           << setw(6)
                           << setfill('0')
                           << addy;
            }
            objectFile << endl;
        }
    }
    
    objectFile.close();
    return;
}
void generateReferenceRecord(vector<vector<string> > tokenized, string file){
    string temp = file.substr(0, file.find(".",0));
    temp += ".obj";

    ofstream objectFile;
    objectFile.open(temp.c_str(), ios_base::app);

    for(int i = 0; i < tokenized.size(); i++){
        if(tokenized[i][0] == ".")
            continue;
        else if(tokenized[i].size() < 3)
            continue;
        else if(tokenized[i][1] == "EXTREF"){
            objectFile << "R";
            vector<string> symbols = split(tokenized[i][2], ',');
            for(int i = 0; i < symbols.size(); i++){
                objectFile << "^"
                           << symbols[i];
            }
            objectFile << endl;
        }
    }
    
    objectFile.close();
    return;
}

void generateTextRecord(vector<vector<string> > tokenized, string file){
    string temp = file.substr(0, file.find(".",0));
    temp += ".obj";
    ofstream objectFile;
    objectFile.open(temp.c_str(), ios_base::app);

    const unsigned int textSize = 30;
    unsigned int size=0;
    unsigned int memory=0;
    unsigned int tempMem=0;
    vector<string> codes;
    
    for(int i = 4; i < tokenized.size(); i++){
        
        if(tokenized[i][0] == ".")              //Comment
            continue;
        if(tokenized[i][2] == "WORD" || tokenized[i][2] == "RESW")
            break;
        else if(tokenized[i].size() < 4)       //End Record
            continue;
        
        while(true){

            if(i == tokenized.size())                //if i is out of range
                break;
            if(tokenized[i][2] == "WORD" || tokenized[i][2] == "RESW")
                break;
            if(tokenized[i][0] == "."){              //Comment
                i++; continue;
            }
            else if(tokenized[i].size() < 4){       //End Record
                i++; continue;
            }

            if(tokenized[i].size()==5){             //column 2 present
                string str = tokenized[i][4];
                if(str.size() > 6){
                    if(size+4 <= textSize){
                        codes.push_back(str);
                        size+=4; memory+=4; i++;
                    } else
                        break;
                }else{
                    if(size+3 <= textSize){
                        codes.push_back(str);
                        size+=3; memory+=3; i++;
                    }else
                        break;
                }
            }else{                                  //column 2 not present
                string str = tokenized[i][3];
                if(str.size() > 6){
                    if(size+4 <= textSize){
                        codes.push_back(str);
                        size+=4; memory+=4; i++;
                    }else
                        break;
                }else{
                    if(size+3 <= textSize){
                        codes.push_back(str);
                        size+=3; memory+=3; i++;
                    }else
                        break;
                }
            }
        }
        stringstream streamA;
        streamA << hex << tempMem;
        string MEMORY(streamA.str());

        stringstream streamB;
        streamB << hex << size;
        string SIZE(streamB.str());

        objectFile << "T" 
             << "^" 
             << setw(6) 
             << setfill('0') 
             << MEMORY

             << "^" 
             << setw(2) 
             << setfill('0') 
             << SIZE;

        for(int j = 0; j < codes.size(); j++){
            objectFile << "^"
                 << codes[j];
        }
        objectFile << endl;
        cout << tokenized.size() << ": " << i << endl;
        tempMem = memory;
        size = 0;
        codes.clear();
        i--;
    }

    int programSize = tokenized.size()-1;
    unsigned int programLength = ESTAB[programName].length;
    stringstream streamC;
    streamC << hex << programLength;
    string PROGLENGTH(streamC.str());

    objectFile << "T" << "^"
         << setw(6) << setfill('0') 
         << PROGLENGTH << "^"
         << "03" << "^"
         << setw(6) << setfill('0') 
         << tokenized[programSize][3] 
         << endl;

    objectFile.close();
    return;
}

void modRecordAux(vector<string> symbols, 
        vector<vector<string> > tokenized,
        string file, int i, int index){

    string temp = file.substr(0, file.find(".",0));
    temp += ".obj";    
    ofstream objectFile;
    objectFile.open(temp.c_str(), ios_base::app);

    string str = tokenized[i][index];
    vector<string> arguments = splitString(str);
    
    if(arguments.size() == 0 || arguments[0] == ""){
        return;
    }
    
    for(int j = 0; j < symbols.size(); j++){
        for(int k = 0; k < arguments.size(); k++){
            if(symbols[j] == arguments[k]){

                unsigned int address;
                istringstream converter(tokenized[i][0].c_str());
                converter >> hex >> address;
                address+=1;

                stringstream stream;
                stream << hex << address;
                string addy(stream.str());

                objectFile << "M"
                        << "^"
                        << setw(6)
                        << setfill('0')
                        << addy//tokenized[i][0]
                        << "^"
                        << "05"
                        << "^"
                        << "+"
                        << arguments[k]
                        << endl;
            }
        }
    }
    objectFile.close();
    return;
}
void modRecordFormat4(vector<string> symbols, 
        vector<vector<string> > tokenized,
        string file, int i, int index){

    string temp = file.substr(0, file.find(".",0));
    temp += ".obj";    
    ofstream objectFile;
    objectFile.open(temp.c_str(), ios_base::app);

    string str = tokenized[i][index];
    vector<string> arguments = splitString(str);
    
    if(arguments.size() == 0 || arguments[0] == ""){
        return;
    }

    //if argument is in EXTREF do not print format 4 instrct, 
    //cause already made
    if(find(symbols.begin(), symbols.end(), 
            arguments[0]) != symbols.end()){
        return;     
    }

    unsigned int address;
    istringstream converter(tokenized[i][0].c_str());
    converter >> hex >> address;
    address+=1;

    stringstream stream;
    stream << hex << address;
    string addy(stream.str());

    objectFile << "M"
            << "^"
            << setw(6)
            << setfill('0')
            << addy
            << "^"
            << "05"
            << "^"
            << "+"
            << programName
            << endl;

    objectFile.close();
    return;
}

void generateModificationRecord(vector<vector<string> > tokenized, string file){

    const unsigned int size = 30;
    unsigned int count;
    vector<string> symbols;
    for(int i = 0; i < tokenized.size(); i++){
        if(tokenized[i][0] == ".")              //Comment
            continue;
        else if(tokenized[i].size() < 3)        //End record
            continue;
        else if(tokenized[i][1] == "EXTREF"){   //populate external symbols 
            symbols = split(tokenized[i][2], ',');
        }
        else if(symbols.size() != 0){           //enter if external refs present
            
            if(tokenized[i].size() == 5){                   //Column 2 present
                modRecordAux(symbols,tokenized,file,i,3);
            } else if(tokenized[i].size() == 4){            //Column 2 not present
                modRecordAux(symbols,tokenized,file,i,2);
            }
            int temp = tokenized[i].size()-1;
            if(tokenized[i][temp].size() > 6)        //Format 4 instruction
                modRecordFormat4(symbols,tokenized,file,i,2);

        }
    }
    return;
}

void generateEndRecord(vector<vector<string> > tokenized, string file){
    string temp = file.substr(0, file.find(".",0));
    temp += ".obj";

    ofstream objectFile;
    objectFile.open(temp.c_str(), ios_base::app);

    bool twoPass = false;
    string symbol;
    for(int i = 0; i < tokenized.size(); i++){
        if(tokenized[i][0] == ".")
            continue;
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
        string address = "";
        for(int j = 1; j < tokenized.size(); j++){
            if(tokenized[j][1] == symbol)
                address = tokenized[j][0];
            if(address != ""){
                objectFile << "E"
                           << "^"
                           << address
                           << endl;
                break;
            }
        }
    }
    objectFile.close();
    return;
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

    generateHeaderRecord(tokenized, input);
    generateDefinitionRecord(tokenized, input);
    generateReferenceRecord(tokenized, input);
    generateTextRecord(tokenized, input);
    generateModificationRecord(tokenized, input);
    generateEndRecord(tokenized, input);
    
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
    // printInstructions();
    // printSourceCode();

    return 0;
}

