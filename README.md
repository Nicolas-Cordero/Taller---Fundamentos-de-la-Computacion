
# Lenguaje CEWE - Compilador Didáctico en C, Flex y Bison

Este proyecto corresponde a la implementación completa de un compilador para un lenguaje de programación propio y personalizado llamado **CEWE**, desarrollado como parte del curso *Fundamentos de la Computación* en la Universidad Católica del Norte.

## ✨ Características del Lenguaje CEWE

- Sintaxis personalizada y lúdica (e.g. `forowo`, `whilewe`, `ifiwi`).
- Soporte para:
  - Declaración de funciones (`funciwi`)
  - Entrada/salida (`printiwi`, `inputuwu`)
  - Condicionales (`ifiwi`, `elsewe`)
  - Ciclos (`whilewe`)
  - Operaciones aritméticas y cálculo de potencias con bucle
- Evaluación a través de Árbol de Sintaxis Abstracta (AST)

## 🧩 Estructura del Proyecto

```
.
├── ast.c / ast.h                   # Lógica de nodos y evaluación del AST
├── parser.y                        # Gramática del lenguaje (Bison)
├── scanner.l                       # Reglas léxicas (Flex)
├── main.c                          # Punto de entrada del compilador
├── build.sh                        # Script de compilación automatizada
├── calculadora.cewe               # Calculadora interactiva: suma, resta, mult, div, potencia
├── ejemploSinErrores.cewe         # Programa funcional sin errores (operaciones simples)
├── README.md                       # Documento descriptivo del proyecto
├── .vscode/                        # Configuración del entorno en VS Code
├── version_antigua_presentada/    # Versión original enviada (histórico)
│   └── version_anterior_presentada_en_clases/
├── informe_y_documentacion/
│   ├── Documentacion - Taller Fundamentos.pdf
│   └── Informe Teorico - Taller Fundamentos.pdf
└── programa.cewe (opcional)       # Entrada alternativa si se decide renombrar mainiwi

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
./cewe calculadora.cewe
```

## 📚 Documentación

Se han generado dos archivos `.tex` listos para compilar en LaTeX:

- `informe_cewe.tex`: describe todos los conceptos teóricos del curso aplicados al proyecto.
- `documentacion_cewe.tex`: explica cómo instalar, compilar y ejecutar el lenguaje CEWE, además de detallar el funcionamiento de cada archivo fuente.

## 👨‍💻 Autor

- **Nicolás Gonzalo Cordero Varas**
- RUT: 20.543.155-1
- Docente: José Luis Veas
- Universidad Católica del Norte, 2025

---

