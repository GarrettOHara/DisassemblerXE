# XE LINK-EDITOR
The XE link-editor program shall open SIC/XE assembler listing files (refer to fig
2.3 in your text) and generate executable object file(s) for the XE machine and the
ESTAB for the project (note, do not build a link-loader!).

## Input

Input - The user will start the program (the program shall be named “led”) and will
provide the listing file(s) as arguments on the command-line each separated by
spaces, i.e.:
“[cssc0000@edoras ~]$ led first.sl second.sl third.sl”

## Output

Output – The SIC (XE variant) object file(s) such as those found in figure 3.9 of
the text. Print the ESTAB in a separate file (name.st) and is such as the ESTAB at
the top of page 143 in the text.