# Software Design Document
## Assignment 2 | Disassembler for XE computer
## CS 530 Lenoard

**Team**


Vivian Reyes   csscxxxx    RedId: XXXXXXXXX\
Garrett O'Hara cssc3724    RedId: 822936303

**Overview & Goals**

Our goal is to create a Linkage Editor. This program will complile a SIC/XE program and resolve all external sybols by using a ESTAB as explained in Chapter 3.

- The program will be named `led` and will take CLI arguments of 3 separate files.
- The program will parse the files and generate executable object code for an XE machine.
- The program will output the Object code in a file named `name??` and the ESTAB in a separate file named `???.st`.

The program will be invoked with the following commands and arguments:

```
edoras~$ led fileA.sl fileB.sl fileC.sl
```

**Environment Requirements**

The environemnt for this program was designed to run on a Linux environemnt.

SDSU's Linux environemnt `edoras` will be used frequently in order to **make sure?** that the program will compile for credit


**Checkpoints**

Project RoadMap

- Outline and Project Blueprint: March 24
    - SDD file created and turned in
    - GitHub repo setup remotely and local environments configured
    - General understanding of partner workflow
- Stage A: April 4th
    - Create a program named `led` that accepts 3 CLI arguments with user input error
    - `led` will compile and parse file contents
- Stage B: April 10th
    - `led` will generate Object code for a single file and create a local SYMTAB
    - Understanding/approach will be established for multi-file linakge with a global ESTAB
- Stage C: April 12th
    - Program will be finsihed and edge cases and bugs will be accounted for
- Submission: April 14th
    - Submission will occur on this date
    