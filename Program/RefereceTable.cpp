#include <iostream>
#include <fstream>
#include <map>

using namespace std;

class ReferenceTable{
    public:
        //needs to be:       std::hex
        std::map<std::string,int> m;
        ReferenceTable(){
            m["ADD"]=18;
            m["ADDF"]=58;
            m["AND"]=40;
            m["CLEAR"]=4;
            m["COMP"]=28;
            m["COMPF"]=88;
            m["COMPR"]=0000;
            m["DIV"] = 0000;
        }
};

/**
 * This idea is going to shit quickly
 * 
 * also I am not sure anymore if this will be useful
 * 
 * its purpose was supposed to be fast lookups for 
 * XE instructions so we could parse and know if someting
 * was an instruction/keyword in the language or if something
 * was a symbol we need to store int he ESTAB
 **/