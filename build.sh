#!/bin/bash
# Autor: Nicolás Cordero
# Script de construcción para el compilador CEWE
# Detiene el proceso ante el primer error
set -e

echo "Limpiando generación anterior..."
rm -f parser.tab.c parser.tab.h lex.yy.c cewe

echo "Generando parser con Bison..."
bison -Wcounterexamples -d parser.y          # -d genera parser.tab.h y muestra ejemplos de conflictos

echo "Generando scanner con Flex..."
flex scanner.l             # Produce lex.yy.c

echo "Compilando..."
gcc -o cewe \
    parser.tab.c \
    lex.yy.c \
    ast.c \
    main.c \
    -Wall -Wextra

echo "Compilación exitosa. Ejecuta con:"
echo "   ./cewe calculadora.cewe"