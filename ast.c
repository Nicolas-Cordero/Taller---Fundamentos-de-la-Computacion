#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

#define MAX_FUNCIONES 100
#define MAX_VARS 100

typedef struct {
  char *nombre;
  Valor valor;
} VariableLocal;

VariableLocal tabla_variables_locales[MAX_VARS];
int cantidad_variables_locales = 0;

ASTNode *tablaFunciones[MAX_FUNCIONES];
int totalFunciones = 0;

Symbol symbol_table[MAX_SYMBOLS];
int symbol_count = 0;

TablaSimbolos* crearTablaSimbolos() {
    TablaSimbolos* ts = (TablaSimbolos*)malloc(sizeof(TablaSimbolos));
    ts->cantidad = 0;
    return ts;
}

Valor evaluarASTConContexto(ASTNode* nodo, TablaSimbolos* ts_local) {
    Valor resultado;

    if (!nodo) {
        resultado.tipo = TYPE_INT;
        resultado.valor_entero = 0;
        return resultado;
    }

    switch (nodo->tipo) {

        case NUMERO:
            resultado.tipo = TYPE_INT;
            resultado.valor_entero = nodo->numero.valor;
            return resultado;

        case NODE_STR_LITERAL:
            resultado.tipo = TYPE_STRING;
            strncpy(resultado.valor_string, nodo->str_literal.valor, 255);
            resultado.valor_string[255] = '\0';
            return resultado;

        case IDENTIFICADOR: {
            const char* nombre = nodo->identificador.nombre;
            return obtenerVariable(ts_local, nombre);
        }

        case OPERACION: {
            Valor izq = evaluarASTConContexto(nodo->operacion.izq, ts_local);
            Valor der = evaluarASTConContexto(nodo->operacion.der, ts_local);

            if (izq.tipo != TYPE_INT || der.tipo != TYPE_INT) {
                fprintf(stderr, "Error: operación requiere enteros.\n");
                exit(1);
            }

            resultado.tipo = TYPE_INT;
            switch (nodo->operacion.operador) {
                case '+': resultado.valor_entero = izq.valor_entero + der.valor_entero; break;
                case '-': resultado.valor_entero = izq.valor_entero - der.valor_entero; break;
                case '*': resultado.valor_entero = izq.valor_entero * der.valor_entero; break;
                case '/': resultado.valor_entero = (der.valor_entero != 0) ? izq.valor_entero / der.valor_entero : 0; break;
                case '^': {
                    int r = 1;
                    for (int i = 0; i < der.valor_entero; i++) r *= izq.valor_entero;
                    resultado.valor_entero = r;
                    break;
                }
                default:
                    fprintf(stderr, "Operador desconocido.\n");
                    exit(1);
            }
            return resultado;
        }

        case RETURN:
            return evaluarASTConContexto(nodo->retorno.expresion, ts_local);

        case PRINT: {
            Valor val = evaluarASTConContexto(nodo->print.expresion, ts_local);
            if (val.tipo == TYPE_INT)
                printf("%d\n", val.valor_entero);
            else if (val.tipo == TYPE_STRING)
                printf("%s\n", val.valor_string);
            else {
                fprintf(stderr, "Tipo no soportado en printuwu.\n");
                exit(1);
            }
            resultado.tipo = TYPE_INT;
            resultado.valor_entero = 0;
            return resultado;
        }

        case ASIGNACION: {
            const char* nombre = nodo->assign.identificador;
            Valor valor = evaluarASTConContexto(nodo->assign.expr, ts_local);
            insertarVariable(ts_local, nombre, valor);
            return valor;
        }

        case IF_ELSE: {
            Valor cond = evaluarASTConContexto(nodo->ifelse.condicion, ts_local);
            if (cond.tipo != TYPE_INT) {
                fprintf(stderr, "Condición de if debe ser entero.\n");
                exit(1);
            }
            if (cond.valor_entero)
                return evaluarASTConContexto(nodo->ifelse.bloqueIf, ts_local);
            else
                return evaluarASTConContexto(nodo->ifelse.bloqueElse, ts_local);
        }

        case WHILE: {
            while (1) {
                Valor cond = evaluarASTConContexto(nodo->whili.condicion, ts_local);
                if (cond.tipo != TYPE_INT) {
                    fprintf(stderr, "Condición de while debe ser entero.\n");
                    exit(1);
                }
                if (!cond.valor_entero) break;
                evaluarASTConContexto(nodo->whili.bloque, ts_local);
            }
            resultado.tipo = TYPE_INT;
            resultado.valor_entero = 0;
            return resultado;
        }

        case PROGRAMA: {
            ASTNode* actual = nodo;
            while (actual) {
                evaluarASTConContexto(actual->programa.instruccion, ts_local);
                actual = actual->programa.programa;
            }
            resultado.tipo = TYPE_INT;
            resultado.valor_entero = 0;
            return resultado;
        }

        default:
            fprintf(stderr, "Nodo no soportado en evaluarASTConContexto\n");
            exit(1);
    }
}

void insertarVariable(TablaSimbolos* ts, const char* nombre, Valor val) {
    for (int i = 0; i < ts->cantidad; i++) {
        if (strcmp(ts->entradas[i].nombre, nombre) == 0) {
            ts->entradas[i].valor = val;
            return;
        }
    }
    strncpy(ts->entradas[ts->cantidad].nombre, nombre, 63);
    ts->entradas[ts->cantidad].valor = val;
    ts->cantidad++;
}

Valor obtenerVariable(TablaSimbolos* ts, const char* nombre) {
    for (int i = 0; i < ts->cantidad; i++) {
        if (strcmp(ts->entradas[i].nombre, nombre) == 0)
            return ts->entradas[i].valor;
    }
    fprintf(stderr, "Variable no encontrada en entorno local: %s\n", nombre);
    exit(1);
}

void liberarTablaSimbolos(TablaSimbolos* ts) {
    free(ts);
}


int add_symbol(const char* name, VarType type) {
    if (symbol_exists(name)) return 0;
    strncpy(symbol_table[symbol_count].name, name, 63);
    symbol_table[symbol_count].type = type;
    symbol_count++;
    return 1;
}

void asignar_variable_local(char *nombre, Valor valor) {
    for (int i = 0; i < cantidad_variables_locales; i++) {
        if (strcmp(tabla_variables_locales[i].nombre, nombre) == 0) {
            tabla_variables_locales[i].valor = valor;
            return;
        }
    }
    tabla_variables_locales[cantidad_variables_locales].nombre = strdup(nombre);
    tabla_variables_locales[cantidad_variables_locales].valor = valor;
    cantidad_variables_locales++;
}

Valor obtener_variable_local(char *nombre) {
    for (int i = 0; i < cantidad_variables_locales; i++) {
        if (strcmp(tabla_variables_locales[i].nombre, nombre) == 0) {
            return tabla_variables_locales[i].valor;
        }
    }
    fprintf(stderr, "Variable local no encontrada: %s\n", nombre);
    exit(1);
}


VarType get_symbol_type(const char* name) {
    for (int i = 0; i < symbol_count; ++i) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            return symbol_table[i].type;
        }
    }
    fprintf(stderr, "Error: variable '%s' no declarada.\n", name);
    exit(1);
}

int symbol_exists(const char* name) {
    for (int i = 0; i < symbol_count; ++i) {
        if (strcmp(symbol_table[i].name, name) == 0) return 1;
    }
    return 0;
}
ASTNode* crearNodoInput(ASTNode* mensaje) {
    ASTNode* nodo = (ASTNode*)malloc(sizeof(ASTNode));
    nodo->tipo = NODE_INPUT;
    nodo->input.mensaje = mensaje;
    nodo->siguiente = NULL;
    return nodo;
}

ASTNode* listaFunciones = NULL;

ASTNode *crearNodo(ASTNodeType tipo) {
    ASTNode *nuevo = (ASTNode *)malloc(sizeof(ASTNode));
    if (!nuevo) {
        fprintf(stderr, "Error de memoria al crear nodo\n");
        exit(EXIT_FAILURE);
    }
    memset(nuevo, 0, sizeof(ASTNode));
    nuevo->tipo = tipo;
    return nuevo;
}

ASTNode* crearValorEntero(int val) {
    ASTNode* nodo = crearNodo(NUMERO);
    nodo->numero.valor = val;
    nodo->tipo_resultado = TYPE_INT;
    return nodo;
}

ASTNode *buscarFuncionPorNombre(const char *nombre) {
    for (int i = 0; i < totalFunciones; i++) {
        if (strcmp(tablaFunciones[i]->nombre, nombre) == 0) {
            return tablaFunciones[i];
        }
    }
    return NULL;
}

ASTNode* crearValorString(char* texto) {
    ASTNode* nodo = crearNodo(NODE_STR_LITERAL);
    nodo->str_literal.valor = strdup(texto);
    nodo->tipo_resultado = TYPE_STRING;
    return nodo;
}

ASTNode* crearNodoOperacionRel(char operador, ASTNode* izquierda, ASTNode* derecha) {
    ASTNode* nodo = crearNodo(NODE_OPERACION_REL);
    nodo->operacion_rel.operador_rel = operador;
    nodo->operacion_rel.izquierda = izquierda;
    nodo->operacion_rel.derecha = derecha;
    return nodo;
}

ASTNode* crearNodoVariable(char* nombre) {
    ASTNode* nodo = crearNodoIdentificador(nombre);
    nodo->tipo_resultado = get_symbol_type(nombre);
    return nodo;
}

ASTNode *crearNodoPrograma(ASTNode *instruccion, ASTNode *programa) {
    ASTNode *nodo = crearNodo(PROGRAMA);
    nodo->programa.instruccion = instruccion;
    nodo->programa.programa = programa;
    return nodo;
}

ASTNode *crearNodoParametros(ASTNode *param, ASTNode *sig) {
    ASTNode *nodo = crearNodo(PARAMETROS);
    nodo->parametros.param = param;
    nodo->parametros.sig = sig;
    return nodo;
}

ASTNode *crearNodoArgumentos(ASTNode *arg, ASTNode *sig) {
    ASTNode *nodo = crearNodo(ARGUMENTOS);
    nodo->argumentos.arg = arg;
    nodo->argumentos.sig = sig;
    return nodo;
}

ASTNode *crearNodoFuncion(char *nombre, ASTNode *parametros, ASTNode *cuerpo) {
    ASTNode *nodo = crearNodo(FUNCION);
    nodo->funcion.nombre = strdup(nombre);
    nodo->funcion.parametros = parametros;
    nodo->funcion.cuerpo = cuerpo;
    return nodo;
}

ASTNode *crearNodoPrint(ASTNode *expresion) {
    ASTNode *nodo = crearNodo(PRINT);
    nodo->print.expresion = expresion;
    return nodo;
}

ASTNode* crearNodoAsignacion(char *id, ASTNode *expr) {
    if (!symbol_exists(id)) {
        fprintf(stderr, "Error: variable '%s' no declarada.\n", id);
        exit(1);
    }

    VarType tipo_var = get_symbol_type(id);
    if (tipo_var != expr->tipo_resultado) {
        fprintf(stderr, "Error: no se puede asignar valor de tipo incompatible a variable '%s'.\n", id);
        exit(1);
    }

    ASTNode* nodo = crearNodo(ASIGNACION);
    nodo->assign.identificador = strdup(id);
    nodo->assign.expr = expr;
    nodo->tipo_resultado = tipo_var; // útil para validar cosas como x = (y = 2)
    return nodo;
}



ASTNode *crearNodoIfElse(ASTNode *cond, ASTNode *bloqueIf, ASTNode *bloqueElse) {
    ASTNode *nodo = crearNodo(IF_ELSE);
    nodo->ifelse.condicion = cond;
    nodo->ifelse.bloqueIf = bloqueIf;
    nodo->ifelse.bloqueElse = bloqueElse;
    return nodo;
}

ASTNode *crearNodoWhile(ASTNode *cond, ASTNode *bloque) {
    ASTNode *nodo = crearNodo(WHILE);
    nodo->whili.condicion = cond;
    nodo->whili.bloque = bloque;
    return nodo;
}

ASTNode* crearNodoReturn(ASTNode* expr, VarType tipo_esperado) {
    if (expr->tipo_resultado != tipo_esperado) {
        fprintf(stderr, "Error: tipo de retorno incompatible. Se esperaba %s, pero se obtuvo %s.\n",
            tipo_esperado == TYPE_INT ? "int" : "string",
            expr->tipo_resultado == TYPE_INT ? "int" : "string");
        exit(1);
    }

    ASTNode* nodo = crearNodo(RETURN);
    nodo->retorno.expresion = expr;
    nodo->tipo_resultado = tipo_esperado;
    return nodo;
}

ASTNode *crearNodoOperacion(char op, ASTNode *izq, ASTNode *der) {
    ASTNode *nodo = crearNodo(OPERACION);
    nodo->operacion.operador = op;
    nodo->operacion.izq = izq;
    nodo->operacion.der = der;
    if (izq->tipo_resultado != TYPE_INT || der->tipo_resultado != TYPE_INT) {
    fprintf(stderr, "Error: operación aritmética requiere dos operandos de tipo int. Se obtuvo %s y %s.\n",
        izq->tipo_resultado == TYPE_INT ? "int" : "string",
        der->tipo_resultado == TYPE_INT ? "int" : "string");
    exit(1);
    }
    nodo->tipo_resultado = TYPE_INT;
    return nodo;
}

ASTNode *crearNodoNumero(int valor) {
    ASTNode *nodo = crearNodo(NUMERO);
    nodo->numero.valor = valor;
    nodo->tipo_resultado = TYPE_INT;
    return nodo;
}

ASTNode* crearNodoString(char* valor) {
    ASTNode* nodo = crearNodo(NODE_STR_LITERAL);
    nodo->str_literal.valor = strdup(valor);
    nodo->tipo_resultado = TYPE_STRING;
    return nodo;
}

ASTNode *crearNodoIdentificador(char *id) {
    ASTNode *nodo = crearNodo(IDENTIFICADOR);
    nodo->identificador.nombre = strdup(id);
    return nodo;
}

ASTNode *crearNodoDeclaracionFuncion(char *nombre, ASTNode *parametros, ASTNode *cuerpo) {
    ASTNode *nodo = crearNodo(DECLARACION_FUNCION);
    nodo->funcion_decl.nombre = strdup(nombre);
    nodo->funcion_decl.parametros = parametros;
    nodo->funcion_decl.cuerpo = cuerpo;

    nodo->siguiente = listaFunciones;
    listaFunciones = nodo;

    return nodo;
}

ASTNode *crearNodoLlamadoFuncion(char *nombre, ASTNode *argumentos) {
    ASTNode *nodo = crearNodo(LLAMADO_FUNCION);
    nodo->funcion_llamada.nombre = strdup(nombre);
    nodo->funcion_llamada.argumentos = argumentos;
    return nodo;
}

ASTNode *crearNodoLista(ASTNode *actual, ASTNode *siguiente) {
    ASTNode *nodo = crearNodo(LISTA_PARAMETROS); // o LISTA_ARGUMENTOS, es el mismo struct
    nodo->lista.actual = actual;
    nodo->lista.siguiente = siguiente;
    return nodo;
}

ASTNode* crearNodoListaParametros(char* nombre, ASTNode* siguiente) {
    ASTNode* nodo = crearNodo(LISTA_PARAMETROS);
    ASTNode* identificador = crearNodoIdentificador(nombre);
    nodo->lista.actual = identificador;
    nodo->lista.siguiente = siguiente;
    return nodo;
}

ASTNode* crearNodoListaArgumentos(ASTNode* valor, ASTNode* siguiente) {
    ASTNode* nodo = crearNodo(LISTA_ARGUMENTOS);
    nodo->lista.actual = valor;
    nodo->lista.siguiente = siguiente;
    return nodo;
}




// Evaluación para pruebas simples
int evaluar(ASTNode *nodo) {
    if (!nodo) return 0;

    switch (nodo->tipo) {
        
        case NUMERO:
            return nodo->numero.valor;
            
        case OPERACION: {
            int izq = evaluar(nodo->operacion.izq);
            int der = evaluar(nodo->operacion.der);
            switch (nodo->operacion.operador) {
                case '+': return izq + der;
                case '-': return izq - der;
                case '*': return izq * der;
                case '/': return der != 0 ? izq / der : 0;
                case '^': {
                            int res = 1;
                            for (int i = 0; i < der; i++) res *= izq;
                            return res;
                        }
                default: return 0;
            }
        }

        case NODE_FUNCION_LLAMADA: {
            ASTNode *funcion = buscarFuncionPorNombre(nodo->funcion_llamada.nombre);
            if (!funcion) {
                printf("Error: función '%s' no definida.\n", nodo->funcion_llamada.nombre);
                exit(1);
            }

            // Evaluar argumentos reales (asumimos máximo 2 argumentos por ahora)
            Valor arg1 = evaluarAST(nodo->funcion_llamada.argumentos->lista.actual);

            Valor arg2;
            if (nodo->funcion_llamada.argumentos->lista.siguiente) {
                arg2 = evaluarAST(nodo->funcion_llamada.argumentos->lista.siguiente->lista.actual);
            } else {
                arg2.tipo = TYPE_INT;
                arg2.valor_entero = 0;
            }

            // Simular asignación de parámetros (no implementado aún)
            // Ejecutar cuerpo de la función
            Valor resultado = evaluarAST(funcion->funcion_decl.cuerpo);
            return resultado.valor_entero;
}

        case NODE_FUNCION:
            if (totalFunciones < MAX_FUNCIONES) {
                tablaFunciones[totalFunciones++] = nodo;
            }
            return crearValorInt(0);  // Por ahora, no hace nada más

        case NODE_WHILE: {
            while (1) {
                Valor condicion = evaluarAST(nodo->whili.condicion);

                if (condicion.tipo != TYPE_INT) {
                    fprintf(stderr, "Condición de whiliwi debe ser un entero.\n");
                    exit(EXIT_FAILURE);
                }

                if (!condicion.valor_entero)
                    break;

                evaluarAST(nodo->whili.bloque);
            }

            Valor vacio;
            vacio.tipo = TYPE_INT;
            vacio.valor_entero = 0;
            return 0;
}

        case NODE_OPERACION_REL: {
                Valor izq = evaluarAST(nodo->operacion_rel.izquierda);
                Valor der = evaluarAST(nodo->operacion_rel.derecha);

                if (izq.tipo != der.tipo) {
                    fprintf(stderr, "Error: No se puede comparar tipos distintos (%s vs %s).\n",
                        izq.tipo == TYPE_INT ? "intiwi" : "stringiwi",
                        der.tipo == TYPE_INT ? "intiwi" : "stringiwi");
                    exit(1);
                }

                if (izq.tipo != TYPE_INT) {
                    fprintf(stderr, "Error: Comparaciones solo soportan enteros por ahora.\n");
                    exit(1);
                }

                switch (nodo->operacion_rel.operador_rel) {
                    case '<': return izq.valor_entero < der.valor_entero;
                    case '>': return izq.valor_entero > der.valor_entero;
                    case '=': return izq.valor_entero == der.valor_entero;
                    case '!': return izq.valor_entero != der.valor_entero;
                    default:
                        fprintf(stderr, "Operador relacional desconocido: %c\n", nodo->operacion_rel.operador_rel);
                        exit(1);
                }
            }

        case PRINT: {
            Valor valor = evaluarAST(nodo->print.expresion);
            if (valor.tipo == TYPE_STRING) {
                printf("%s\n", valor.valor_string);
            } else if (valor.tipo == TYPE_INT) {
                printf("%d\n", valor.valor_entero);
            } else {
                fprintf(stderr, "Error: tipo no reconocido en print.\n");
                exit(1);
            }
            return 0;
        }
        
        case INPUT: {
            const char* nombre = nodo->input.identificador;
            VarType tipo = get_symbol_type(nombre);

            if (tipo == TYPE_INT) {
                int valor;
                printf("Ingrese un entero (%s): ", nombre);
                scanf("%d", &valor);
                for (int i = 0; i < symbol_count; ++i) {
                    if (strcmp(symbol_table[i].name, nombre) == 0) {
                        symbol_table[i].int_val = valor;
                        break;
                    }
                }
            } else if (tipo == TYPE_STRING) {
                char buffer[256];
                printf("Ingrese un string (%s): ", nombre);
                scanf(" %[^\n]", buffer);
                for (int i = 0; i < symbol_count; ++i) {
                    if (strcmp(symbol_table[i].name, nombre) == 0) {
                        strncpy(symbol_table[i].str_val, buffer, 255);
                        symbol_table[i].str_val[255] = '\0';
                        break;
                    }
                }
            } else {
                fprintf(stderr, "Error: tipo no soportado en input para '%s'\n", nombre);
                exit(1);
            }

            return 0;
        }
        
        case WHILE: {
            while (evaluar(nodo->whili.condicion)) {
                evaluar(nodo->whili.bloque);
                }
                return 0;
            }

        case NODE_IF_ELSE: {
            Valor condicion = evaluarAST(nodo->ifelse.condicion);

            if (condicion.tipo != TYPE_INT) {
                fprintf(stderr, "Condición de ifiwi debe ser un entero.\n");
                exit(EXIT_FAILURE);
            }

            if (condicion.valor_entero) {
                return evaluar(nodo->ifelse.bloqueIf);
            } else {
                return evaluar(nodo->ifelse.bloqueElse);
            }
        }

        case LLAMADO_FUNCION: {
            const char* nombre = nodo->funcion_llamada.nombre;
            ASTNode* argumentos = nodo->funcion_llamada.argumentos;

            // Buscar función en lista global
            ASTNode* funcion = buscarFuncion(nombre);
            if (!funcion) {
                printf("Error: función '%s' no definida.\n", nombre);
                exit(1);
            }

            // Obtener lista de parámetros y asociarlos
            ASTNode* param_formales = funcion->funcion_decl.parametros;
            ASTNode* param_reales = argumentos;

            while (param_formales && param_reales) {
                const char* nombre_param = param_formales->lista.actual->identificador.nombre;
                ASTNode* valor_real = param_reales->lista.actual;

                // Validación de tipo del argumento real vs parámetro formal
                VarType tipo_formal = get_symbol_type(nombre_param);
                if (tipo_formal != valor_real->tipo_resultado) {
                    fprintf(stderr, "Error: tipo del argumento '%s' no coincide con el parámetro formal.\n", nombre_param);
                    exit(1);
                }

                if (!symbol_exists(nombre_param)) {
                    add_symbol(nombre_param, valor_real->tipo_resultado);
                }

                if (valor_real->tipo_resultado == TYPE_INT) {
                    for (int i = 0; i < symbol_count; ++i) {
                        if (strcmp(symbol_table[i].name, nombre_param) == 0) {
                            symbol_table[i].int_val = evaluar(valor_real);
                            break;
                        }
                    }
                } else if (valor_real->tipo_resultado == TYPE_STRING) {
                    for (int i = 0; i < symbol_count; ++i) {
                        if (strcmp(symbol_table[i].name, nombre_param) == 0) {
                            strcpy(symbol_table[i].str_val, valor_real->str_literal.valor);
                            break;
                        }
                    }
                }

                param_formales = param_formales->lista.siguiente;
                param_reales = param_reales->lista.siguiente;
            }

            if (param_formales || param_reales) {
                printf("Error: número incorrecto de argumentos en función '%s'.\n", nombre);
                exit(1);
            }

            // Ejecutar cuerpo de la función (solo retorno simple por ahora)
            ASTNode* cuerpo = funcion->funcion_decl.cuerpo;
            if (cuerpo->tipo == RETURN) {
                return evaluar(cuerpo->retorno.expresion);
            } else if (cuerpo->tipo == PROGRAMA) {
                ASTNode* actual = cuerpo;
                while (actual) {
                    if (actual->programa.instruccion->tipo == RETURN) {
                        return evaluar(actual->programa.instruccion->retorno.expresion);
                    } else {
                        evaluar(actual->programa.instruccion);
                    }
                    actual = actual->programa.programa;
                }
            }

            return 0;
        }

    


        default:
            return 0;
    }
}

Valor evaluarAST(ASTNode* nodo) {
    Valor resultado;

    if (!nodo) {
        resultado.tipo = TYPE_INT;
        resultado.valor_entero = 0;
        return resultado;
    }

    switch (nodo->tipo) {

        /* ─────────  INPUT  ───────── */
        case NODE_INPUT: {
            Valor v;
            if (nodo->input.tipo == TYPE_INT) {
                printf("Ingrese un entero: ");
                scanf("%d", &v.valor_entero);
                v.tipo = TYPE_INT;

            } else if (nodo->input.tipo == TYPE_STRING) {
                char buffer[256];
                printf("Ingrese un texto: ");
                scanf(" %[^\n]", buffer);
                strncpy(v.valor_string, buffer, 255);
                v.valor_string[255] = '\0';
                v.tipo = TYPE_STRING;

            } else {
                fprintf(stderr, "Tipo no soportado en inputuwu.\n");
                exit(EXIT_FAILURE);
            }

            /* guarda el valor en la tabla de símbolos si ese
               input se asignó a una variable con identificador */
            if (nodo->input.identificador) {          // por si usas “inputuwu tipo nombre;”
                for (int i = 0; i < symbol_count; ++i) {
                    if (strcmp(symbol_table[i].name, nodo->input.identificador) == 0) {
                        symbol_table[i].type = v.tipo;
                        if (v.tipo == TYPE_INT)
                            symbol_table[i].int_val = v.valor_entero;
                        else
                            strncpy(symbol_table[i].str_val, v.valor_string, 255);
                        break;
                    }
                }
            }
            return v;
        }

        case NODE_LLAMADO_FUNCION: {
                ASTNode *funcion = buscarFuncionPorNombre(nodo->funcion_llamada.nombre);
                if (!funcion) {
                    fprintf(stderr, "Función no definida: %s\n", nodo->funcion_llamada.nombre);
                    exit(1);
                }

                TablaSimbolos *ts_local = crearTablaSimbolos();

                ASTNode *param_formal = funcion->funcion.parametros;
                ASTNode *arg_real = nodo->funcion_llamada.argumentos;

                while (param_formal && arg_real) {
                    Valor val = evaluarAST(arg_real->lista.actual);
                    insertarVariable(ts_local, param_formal->lista.actual->identificador.nombre, val);

                    param_formal = param_formal->lista.siguiente;
                    arg_real = arg_real->lista.siguiente;
                }

                Valor retorno = evaluarASTConContexto(funcion->funcion.cuerpo, ts_local);
                liberarTablaSimbolos(ts_local);
                return retorno;
            }


        case NODE_IF_ELSE: {
                Valor condicion = evaluarAST(nodo->ifelse.condicion);

                if (condicion.tipo != TYPE_INT) {
                    fprintf(stderr, "Condición de ifiwi debe ser un entero.\n");
                    exit(EXIT_FAILURE);
                }

                if (condicion.valor_entero) {
                    return evaluarAST(nodo->ifelse.bloqueIf);
                } else {
                    return evaluarAST(nodo->ifelse.bloqueElse);
                }
            }

        case NODE_WHILE: {
                while (1) {
                    Valor condicion = evaluarAST(nodo->whili.condicion);

                    if (condicion.tipo != TYPE_INT) {
                        fprintf(stderr, "Condición de whiliwi debe ser un entero.\n");
                        exit(EXIT_FAILURE);
                    }

                    if (!condicion.valor_entero)
                        break;

                    evaluarAST(nodo->whili.bloque);
                }

                Valor vacio;
                vacio.tipo = TYPE_INT;
                vacio.valor_entero = 0;
                return vacio;
            }

        case NUMERO:
            resultado.tipo = TYPE_INT;
            resultado.valor_entero = nodo->numero.valor;
            return resultado;

        case NODE_STR_LITERAL:
            resultado.tipo = TYPE_STRING;
            strncpy(resultado.valor_string, nodo->str_literal.valor, 255);
            resultado.valor_string[255] = '\0';
            return resultado;

        case IDENTIFICADOR: {
            const char* nombre = nodo->identificador.nombre;
            for (int i = 0; i < symbol_count; ++i) {
                if (strcmp(symbol_table[i].name, nombre) == 0) {
                    resultado.tipo = symbol_table[i].type;
                    if (resultado.tipo == TYPE_INT)
                        resultado.valor_entero = symbol_table[i].int_val;
                    else
                        strncpy(resultado.valor_string, symbol_table[i].str_val, 255);
                    return resultado;
                }
            }
            fprintf(stderr, "Error: variable '%s' no encontrada.\n", nombre);
            exit(1);
        }

        case OPERACION: {
            Valor izq = evaluarAST(nodo->operacion.izq);
            Valor der = evaluarAST(nodo->operacion.der);

            if (izq.tipo != TYPE_INT || der.tipo != TYPE_INT) {
                fprintf(stderr, "Error: operación requiere enteros.\n");
                exit(1);
            }

            resultado.tipo = TYPE_INT;
            switch (nodo->operacion.operador) {
                case '+': resultado.valor_entero = izq.valor_entero + der.valor_entero; break;
                case '-': resultado.valor_entero = izq.valor_entero - der.valor_entero; break;
                case '*': resultado.valor_entero = izq.valor_entero * der.valor_entero; break;
                case '/': resultado.valor_entero = (der.valor_entero != 0) ? izq.valor_entero / der.valor_entero : 0; break;
                case '^': {
                    int r = 1;
                    for (int i = 0; i < der.valor_entero; i++) r *= izq.valor_entero;
                    resultado.valor_entero = r;
                    break;
                }
                default:
                    fprintf(stderr, "Operador desconocido.\n");
                    exit(1);
            }
            return resultado;
        }

        default:
            fprintf(stderr, "Error: nodo no soportado aún en evaluarAST().\n");
            exit(1);
    }
}

void imprimirAST(ASTNode *nodo, int nivel) {
    if (!nodo) return;

    for (int i = 0; i < nivel; i++) printf("  ");

    switch (nodo->tipo) {
        case PROGRAMA:
            printf("PROGRAMA\n");
            imprimirAST(nodo->programa.instruccion, nivel + 1);
            imprimirAST(nodo->programa.programa, nivel + 1);
            break;
        case PRINT:
            printf("PRINT\n");
            imprimirAST(nodo->print.expresion, nivel + 1);
            break;
        case ASIGNACION:
            printf("ASIGNACION %s\n", nodo->assign.identificador);
            imprimirAST(nodo->assign.expr, nivel + 1);
            break;
        case INPUT:
            printf("INPUT %s\n", nodo->input.identificador);
            break;
        case IF_ELSE:
            printf("IF_ELSE\n");
            imprimirAST(nodo->ifelse.condicion, nivel + 1);
            imprimirAST(nodo->ifelse.bloqueIf, nivel + 1);
            imprimirAST(nodo->ifelse.bloqueElse, nivel + 1);
            break;
        case WHILE:
            printf("WHILE\n");
            imprimirAST(nodo->whili.condicion, nivel + 1);
            imprimirAST(nodo->whili.bloque, nivel + 1);
            break;
        case RETURN:
            printf("RETURN\n");
            imprimirAST(nodo->retorno.expresion, nivel + 1);
            break;
        case OPERACION:
            printf("OPERACION %c\n", nodo->operacion.operador);
            imprimirAST(nodo->operacion.izq, nivel + 1);
            imprimirAST(nodo->operacion.der, nivel + 1);
            break;
        case NUMERO:
            printf("NUMERO %d\n", nodo->numero.valor);
            break;
        case IDENTIFICADOR:
            printf("IDENTIFICADOR %s\n", nodo->identificador.nombre);
            break;
        case FUNCION:
            printf("FUNCION %s\n", nodo->funcion.nombre);
            imprimirAST(nodo->funcion.parametros, nivel + 1);
            imprimirAST(nodo->funcion.cuerpo, nivel + 1);
            break;

        case DECLARACION_FUNCION:
            printf("DECLARACION_FUNCION %s\n", nodo->funcion_decl.nombre);
            imprimirAST(nodo->funcion_decl.parametros, nivel + 1);
            imprimirAST(nodo->funcion_decl.cuerpo, nivel + 1);
            break;

        case LLAMADO_FUNCION:
            printf("LLAMADO_FUNCION %s\n", nodo->funcion_llamada.nombre);
            imprimirAST(nodo->funcion_llamada.argumentos, nivel + 1);
            break;

        case LISTA_PARAMETROS:
            printf("LISTA_PARAMETROS\n");
            imprimirAST(nodo->lista.actual, nivel + 1);
            imprimirAST(nodo->lista.siguiente, nivel + 1);
            break;
        
        case LISTA_ARGUMENTOS:
            printf("LISTA_ARGUMENTOS\n");
            imprimirAST(nodo->lista.actual, nivel + 1);
            imprimirAST(nodo->lista.siguiente, nivel + 1);
            break;

        case NODE_OPERACION_REL:
            printf("OPERACION_REL %c\n", nodo->operacion_rel.operador_rel);
            imprimirAST(nodo->operacion_rel.izquierda, nivel + 1);
            imprimirAST(nodo->operacion_rel.derecha, nivel + 1);
            break;

        default:
            printf("Nodo desconocido\n");
    }
}

void liberarAST(ASTNode *nodo) {
    if (!nodo) return;

    switch (nodo->tipo) {
        case PROGRAMA:
            liberarAST(nodo->programa.instruccion);
            liberarAST(nodo->programa.programa);
            break;
        case PRINT:
            liberarAST(nodo->print.expresion);
            break;
        case ASIGNACION:
            free(nodo->assign.identificador);
            liberarAST(nodo->assign.expr);
            break;
        case INPUT:
            free(nodo->input.identificador);
            break;
        case IF_ELSE:
            liberarAST(nodo->ifelse.condicion);
            liberarAST(nodo->ifelse.bloqueIf);
            liberarAST(nodo->ifelse.bloqueElse);
            break;
        case WHILE:
            liberarAST(nodo->whili.condicion);
            liberarAST(nodo->whili.bloque);
            break;
        case RETURN:
            liberarAST(nodo->retorno.expresion);
            break;
        case OPERACION:
            liberarAST(nodo->operacion.izq);
            liberarAST(nodo->operacion.der);
            break;
        case IDENTIFICADOR:
            free(nodo->identificador.nombre);
            break;
        case FUNCION:
            free(nodo->funcion.nombre);
            liberarAST(nodo->funcion.parametros);
            liberarAST(nodo->funcion.cuerpo);
            break;
        case DECLARACION_FUNCION:
            free(nodo->funcion_decl.nombre);
            liberarAST(nodo->funcion_decl.parametros);
            liberarAST(nodo->funcion_decl.cuerpo);
            break;

        case LLAMADO_FUNCION:
            free(nodo->funcion_llamada.nombre);
            liberarAST(nodo->funcion_llamada.argumentos);
            break;

        case LISTA_PARAMETROS:
            liberarAST(nodo->lista.actual);
            liberarAST(nodo->lista.siguiente);
            break;
        case LISTA_ARGUMENTOS:
            liberarAST(nodo->lista.actual);
            liberarAST(nodo->lista.siguiente);
            break;
        default: break;
    }

    free(nodo);
}