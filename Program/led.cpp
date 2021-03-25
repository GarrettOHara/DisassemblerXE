/**
 * This Program was written by:
 * 
 * Garrett O'Hara cssc3724 RedId: 822936303
 * Vivian  Reyes  cssc3713 RedId: 821862269
 * 
 * CS 530 | Lenoard
**/
#include <iostream>
using namespace std;

int main(int argc, char *argv[]){
    if(argc < 1){
        cout << "Error: Please specify a listing file to parse" << endl;
    } else if(argc >  3){
        cout << "Error: Please provide 1-3 listing files to parse" << endl;
    }

    for(int i = 0; i < argc; i++){
        cout << argv[i] << endl;
    }
}