#!/bin/bash

# Salir si hay errores
set -e

echo " Compilando Cewe..."

# Generar scanner y parser
flex scanner.l
bison -d parser.y

# Compilar con GCC
gcc -o cewe main.c ast.c parser.tab.c lex.yy.c -Wall

echo "Compilación exitosa. Ejecuta con: ./cewe archivo.cewe"