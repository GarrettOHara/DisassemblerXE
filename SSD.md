# Software Design Document
## Assignment 2 | Linkage Editor for XE Machine
## CS 530 Lenoard

**Team**

Vivian Reyes
&nbsp;&nbsp;&nbsp;
               cssc3713    RedId: 821862269\
Garrett O'Hara cssc3724    RedId: 822936303

**Overview & Goals**

Our goal is to create a Linkage Editor. This program will complile an XE program and resolve all external sybols by using a ESTAB as explained in Chapter 3.

- The program will be named `led` and will take CLI arguments of 3 separate files.
- The program will parse the files and generate executable object code for an XE machine.
- The program will output the Object code in a file named `ObjectRecord` and the ESTAB in a separate file named `ESTAB`.

# Compile / View program output

## Using g++ compiler
The program will be invoked with the following command and arguments:

```
[user@edoras ~]$ g++ led.cpp [LISTING FILE ARGUMENTS]
```

## Using Makefile
```
[user@edoras ~]$ make
```
- This will generate an executable `led` where you can pass listing files as CLI arguments: 
```
[user@edoras ~]$ ./led [LISTING FILES (1-3 supported)]
```
- To clean project space and remove object files use:
```
[user@edoras ~]$ make clean
```
- View program output with any CLI tool `vim`, `emacs`, `nl`
- For this example lets use `cat`:
```
[user@edoras ~]$ cat Adder.obj Writer.obj ESTAB.st
```
- or to see output one at a time use:
```
[user@edoras ~]$ cat [FILENAME]
```
# System Requirements

**Environment Requirements**

The environemnt for this program was designed to run on a Linux environemnt and the program will be compiled using either gcc or g++ compiler.

compiler verion:
```
[user@edoras ~]$ gcc --version
gcc (GCC) 4.8.5 20150623 (Red Hat 4.8.5-44)

[user@edoras ~]$ gcc --version
g++ (GCC) 4.8.5 20150623 (Red Hat 4.8.5-44)
```
**note**: *any linux distrubution with `gcc/g++` with version 4.8.5 or equivalent will suffice*

**Libraries used**
```
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
```


SDSU's Linux environemnt `edoras` will be used frequently in order to confirm that the program will compile for credit

# Program Checkpoints/ Description

**Checkpoints**

Project RoadMap
- Preparation: March 20th
    - Both members of th group will have their local environemnts configured with the same gcc/g++ compiler that is on the edoras remote environemnt
    - A repository for version control will be hosted on GitHub
        - Both team members will have their local environments set up for version control for easy file transfer
- Outline and Project Blueprint Submission: March 24
    - SDD file created and turned in
    - General understanding of partner workflow
- Stage A: April 4th
    - Create a program named `led` that accepts 3 CLI arguments with user input error
    - `led` will compile and parse file contents
    - One test file will be produced from each group member
- Stage B: April 10th
    - `led` will generate Object code for a single file and create a local SYMTAB
    - Understanding/approach will be established for multi-file linakge with a global ESTAB
- Stage C: April 12th
    - Program will be finsihed and edge cases and bugs will be accounted for
- Submission: April 17th
    - Submission will occur on this date

**Program Flow**

The Linkage editior parses each argument provided from the CLI (listing files) and generates an External Symbol Table from these files

The ESTAB is utalized to help generate the object code for the program.

Another pass on the files is taken to generate each line in the Object Code File.

**ESTAB**

- An unorder `map` data structure with a global scope is instantiated which holds all of the necessary data for the ESTAB
    - `Key`: the distinct name for the object (Either program name or symbol name)
    - `Value`: A custom `struct` where we store certain values as fields
        - controlSection name (only appplies to instructions)
        - instruction name (only appplies to instructions)
        - address
        - length (only applies to program)

**Object Code**
- File with [FILENAME].obj is created and Header Record is written to the file
- The file is opened in append mode for the Definition Record to be appended
- The file is opened in append mode for the Reference Record to be appended
- The file is opened in append mode for the Text Record(s) to be appended
- The file is opened in append mode for the Modification Record(s) to be appended
- The file is opened in append mode for the End record to be appended. 


# Function Documention

*split*
- **Arguments:** `string`, `char`
- **Description:** The function takes a string and deliminates/tokenizes it by the character provided
- **Return:** `vector<string>`

*splitString*
- **Arguments:** `string`
- **Description**: This function tokenizes the string by any character besides aplhaNumberic
- **Return**: `vector<string>`

*printInstruction*
- **Arguemnts:**
- **Description** prints tokenized/deliminated source code by whitespace
- **Returns:** `void`

*printsSourceCode*
- **Arguments:**: 
- **Description**: emultes `cat` prints entire source code unmodified
- **Returns**: `void`

*printESTAB*
- **Arguments:**
- **Description:** writes contents of ESTAB to text file named `ESTAB.st`
- **Returns:** `void`

*parseESTAB*
- **Arguments**
- **Description:** parses ESTAB to ensure that all values are within correct memory scope of control section
- **Thows**: Exeption if condition is not met (out of bounds memory request)
- **Returns**: `void`

*generateESTAB*
- **Arguments:** `vector<string>`, `string`
- **Description:** generates ESTAB in global map of ESTAB instance. uses a `vector<string>` to memorize input order for proper ESTAB generation (makes sure values are in correct order)
- **Rrtuns**: `void`

*generateHeaderRecord*
- **Arguments:** `vector<vector<string>>` `string`
- **Description:** creates a on object code file from the listing file provided with a `.obj` extenstion. The header record is written to this file
- **Returns**: `void`

*generateDefinitionRecord*
- **Arguments:** `vector<vector<string>>`, `string`
- **Description:** opens object file created by the header record in append mode. Appends the Definition record to the file
- **Returns**: `void`

*generateReferenceRecord*
- **Arguments:** `vector<vector<string>>`, `string`
- **Description:** opens object file created by the header record in append mode. Appends the reference record to the file
- **Returns**: `void`

*generateTextRecord*
- **Arguments:** `vector<vector<string>>`, `string`
- **Description:** opens object file created by the header record in append mode. Appends the text record to the file
- **Returns**: `void`

*generateModificationRecord*
- **Arguments:** `vector<vector<string>>`, `string`
- **Calls:** calls auxilary funtions to help handle functiom
    - modRecordAux: when external reference encountered
    - modRecordFormat4: when format 4 instruction encoutered
- **Description:** opens object file created by the header record in append mode. Appends the mod record to the file
- 

*generateEndRecord*
- **Arguments:** `vector<vector<string>>`, `string`
- **Description:** opens object file created by the header record in append mode. Appends the end record to the file
- **Returns**: `void`

*readESTAB*
- **Arguments:** reads source code line by line for ESTAB generation
- **Calls:** calles generateESTAB and provides line and tokenizes instruction and also calls parseESTAB to make sure all references are within scope
    - generateESTAB
    - parseESTAB
- **Description**: reades each line of source code and calls generateESTAB
- **Returns**: `void`

*readFileObjectFile*
- **Arguments**: reads source code and tokenizes every line in the file, then is the driver for createing the modification record
- **Calls:** calles all the functions to generate object code file
    - generateHeaderRecord
    - generateDefinitionRecord
    - generateReferenceRecord
    - generateTextRecord
    - generateModificationRecord
    - generateEndRecord
- **Description:** creates the object file for each argument from CLI
- **Returns**: `void`

*main*
- Driver function