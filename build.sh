#!/bin/bash
set -e

echo "Limpiando compilaciones anteriores..."
rm -f parser.tab.c parser.tab.h lex.yy.c cewe

echo "Generando parser con Bison..."
bison -d parser.y

echo "Generando lexer con Flex..."
flex scanner.l

echo "Compilando..."
gcc -o cewe parser.tab.c lex.yy.c ast.c main.c -Wall

echo "Compilación exitosa. Ejecuta con: ./cewe archivo.cewe"