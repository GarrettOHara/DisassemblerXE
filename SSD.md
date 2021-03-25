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

The program will be invoked with the following command and arguments:

```
[user@edoras ~]$ led fileA.sl fileB.sl fileC.sl
```

**Environment Requirements**

The environemnt for this program was designed to run on a Linux environemnt and the program will be compiled using either gcc or g++ compiler.

compiler verion:
```
[user@edoras ~]$ gcc --version
gcc (GCC) 4.8.5 20150623 (Red Hat 4.8.5-44)

[user@edoras ~]$ gcc --version
g++ (GCC) 4.8.5 20150623 (Red Hat 4.8.5-44)
```

SDSU's Linux environemnt `edoras` will be used frequently in order to confirm that the program will compile for credit

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
- Submission: April 14th
    - Submission will occur on this date
    