#!/bin/bash
flex scanner.l
bison -d parser.y
gcc -o cewe main.c ast.c parser.tab.c lex.yy.c -lm
echo "Compilación finalizada. Ejecuta con: ./cewe < test.cewe"
