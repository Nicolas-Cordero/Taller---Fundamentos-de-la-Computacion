# Declaración de variables
intiwi a;
intiwi b;
intiwi resultado;
intiwi opcion;

# Mostrar menú
printiwi("Selecciona una operación:");
printiwi("1. Sumar");
printiwi("2. Restar");
printiwi("3. Multiplicar");
printiwi("4. Dividir");
printiwi("5. Potencia");

# Leer opción del usuario
opcion = inputuwu(INTIWI);

# Leer operandos
a = inputuwu(INTIWI);
b = inputuwu(INTIWI);

# Funciones
funciwi sumar(intiwi x, intiwi y) {
  returnuwu x + y;
}

funciwi restar(intiwi x, intiwi y) {
  returnuwu x - y;
}

funciwi multiplicar(intiwi x, intiwi y) {
  returnuwu x * y;
}

funciwi dividir(intiwi x, intiwi y) {
  returnuwu x / y;
}

funciwi potencia(intiwi base, intiwi exponente) {
  intiwi resultado;
  resultado = 1;

  whilewe (exponente > 0) {
    resultado = resultado * base;
    exponente = exponente - 1;
  }

  returnuwu resultado;
}

# Lógica del menú
ifiwi (opcion == 1) {
  resultado = sumar(a, b);
  printiwi(resultado);
} elsewe ifiwi (opcion == 2) {
  resultado = restar(a, b);
  printiwi(resultado);
} elsewe ifiwi (opcion == 3) {
  resultado = multiplicar(a, b);
  printiwi(resultado);
} elsewe ifiwi (opcion == 4) {
  resultado = dividir(a, b);
  printiwi(resultado);
} elsewe ifiwi (opcion == 5) {
  resultado = potencia(a, b);
  printiwi(resultado);
} elsewe {
  printiwi("Opción inválida.");
}