
# Lenguaje CEWE - Compilador Didáctico en C, Flex y Bison

Este proyecto corresponde a la implementación completa de un compilador para un lenguaje de programación propio y personalizado llamado **CEWE**, desarrollado como parte del curso *Fundamentos de la Computación* en la Universidad Católica del Norte.

## ✨ Características del Lenguaje CEWE

- Sintaxis personalizada y lúdica (e.g. `printuwu`, `whiliwi`, `ifiwi`).
- Soporte para:
  - Declaración de funciones (`funciwi`)
  - Entrada/salida (`printuwu`, `inputuwu`)
  - Condicionales (`ifiwi`, `elsewe`)
  - Ciclos (`whiliwi`)
  - Operaciones aritméticas y cálculo de potencias con bucle
- Evaluación a través de Árbol de Sintaxis Abstracta (AST)

## 🧩 Estructura del Proyecto

```
.
├── ast.c / ast.h           # Lógica de nodos y evaluación del AST
├── parser.y                # Gramática del lenguaje (Bison)
├── scanner.l               # Reglas léxicas (Flex)
├── main.c                  # Punto de entrada del compilador
├── build.sh                # Script de compilación automatizada
├── programa.cewe           # Ejemplo funcional de calculadora
├── documentacion_cewe.tex  # Documentación técnica extensa
├── informe_cewe.tex        # Informe teórico del compilador CEWE
```

## ⚙️ Requisitos

- Sistema operativo GNU/Linux
- Flex, Bison y GCC
- Bash Shell

## 📦 Instalación

```bash
sudo apt update
sudo apt install flex bison gcc build-essential
```

## 🚀 Compilación

```bash
chmod +x build.sh
./build.sh
```

Este script compila todos los archivos y genera el binario `./cewe`.

## 🧪 Ejecución

```bash
./cewe programa.cewe
```

## 📚 Documentación

Se han generado dos archivos `.tex` listos para compilar en LaTeX:

- `informe_cewe.tex`: describe todos los conceptos teóricos del curso aplicados al proyecto.
- `documentacion_cewe.tex`: explica cómo instalar, compilar y ejecutar el lenguaje CEWE, además de detallar el funcionamiento de cada archivo fuente.

## 👨‍💻 Autor

- **Nicolás Gonzalo Cordero Varas**
- RUT: 20.543.155-1
- Profesor: José Luis Veas
- Universidad Católica del Norte, 2025

---

¡Gracias por revisar este trabajo! Para más información, consulta los archivos LaTeX incluidos.
