0000    ADD      START    0      .SAMPLE PROGRAM
0000             EXTDEF   COUNT,TABLE,TABLE2
0000             EXTREF   LEN
0000    FIRST    LDX     #10                       05000A
0003             LDA     #0                        010000
0006            +LDB     #TABLE2                   691007F3
000A             BASE     TABLE2
000A    LOOP     ADD      TABLE,X                  1BA016
000D             ADD      TABLE2,X                 1BC000
0010             TIX      COUNT                    2F200D
0013             JLT      LOOP                     3B200A
0016            +STA      TOTAL                    0F100FC3
001A             LDT      TOTAL                    772FA6
001D    WLEN     LDA      LEN                      032000
0020    COUNT    WORD     10
0023    TABLE    RESW     2000
07F3    TABLE2   RESW     2000
0FC3    TOTAL    RESW     1
                 END      ADD
0FC6    *       =C'EOF'                            454F46
