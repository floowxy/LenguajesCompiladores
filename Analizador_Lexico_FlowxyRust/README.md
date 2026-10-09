# Analizador Léxico - FlowxyRust

Analizador léxico desarrollado en **C++17** para el lenguaje de programación personalizado **FlowxyRust**, inspirado en la sintaxis y características de Rust pero con palabras clave y construcciones en español.

---

## Estructura del Proyecto

El proyecto sigue una arquitectura modular estándar en C++:

```text
Analizador_Lexico_FlowxyRust/
├── include/                  # Cabeceras (.hpp) con interfaces públicas
│   ├── lexer.hpp             # Definición de la clase Lexer y autómatas
│   ├── symbol_table.hpp      # Definición de la tabla de símbolos
│   └── token.hpp             # Estructura del Token (código, categoría, lexema)
├── src/                      # Código fuente de implementación (.cpp)
│   ├── lexer.cpp             # Lógica de escaneo y transición de estados
│   ├── main.cpp              # Punto de entrada (modo interactivo y por archivo)
│   └── symbol_table.cpp      # Gestión de inserción e inspección de identificadores
├── examples/                 # Ejemplos completos de código en FlowxyRust
│   └── prueba_completa.txt   # Programa de demostración con todas las características
├── tests/                    # Casos de prueba para validación
│   ├── test_basico.txt       # Casos nominales básicos
│   └── test_errores.txt      # Casos con errores léxicos intencionales
├── bin/                      # [Generado al compilar] Ejecutable final (ignorado en git)
├── build/                    # [Generado al compilar] Archivos objeto .o (ignorado en git)
├── Makefile                  # Automatización de compilación y pruebas
├── .gitignore                # Excluye binarios, temporales y carpetas de compilación
└── README.md                 # Documentación técnica y guía de uso
```

---

## Categorías y Códigos de Tokens

| Categoría | Rango de Código | Ejemplos |
| :--- | :---: | :--- |
| **Palabras Reservadas** | `100 - 128` | `sea`, `constante`, `mutable`, `clase`, `implementa`, `este`, `si`, `sino`, `mientras`, `para`, `fun`, `retornar`, `mainfun`, tipos de datos (`entero32`, `booleano`, etc.) |
| **Operadores** | `200 - 221` | `+`, `-`, `*`, `/`, `%`, `==`, `!=`, `<`, `<=`, `>`, `>=`, `&&`, `\|\|`, `=`, `+=`, `-=`, `++`, `--` |
| **Delimitadores** | `300 - 309` | `(`, `)`, `{`, `}`, `[`, `]`, `:`, `;`, `.`, `,` |
| **Identificadores** | `1000` | Nombres de variables, funciones, clases (e.g. `persona`, `obtenerEdad`, `total`) |
| **Literales Numéricos** | `2000` | Enteros y flotantes (e.g. `10`, `3.14`, `20`) |
| **Literales de Carácter** | `3000` | Caracteres entre comillas simples (e.g. `'D'`, `'x'`) |
| **Literales de Cadena** | `4000` | Textos entre comillas dobles (e.g. `"Hola mundo"`) |
| **Error Léxico** | `911` | Tokens mal formados (e.g. `variable__invalida`, `12.34.56`, `&`, `\|`, etc.) |

---

## Requisitos

- Compilador con soporte para **C++17** (`g++` >= 7 o `clang++`)
- Herramienta **make**

---

## Compilación y Ejecución

El proyecto incluye un `Makefile` para facilitar todo el ciclo de desarrollo:

### 1. Compilar el proyecto
```bash
make
```
El ejecutable resultante se genera en `bin/lexer`.

### 2. Ejecutar con archivo de prueba
```bash
make test
```
O ejecutando directamente el binario pasando la ruta del archivo:
```bash
./bin/lexer examples/prueba_completa.txt
```

### 3. Ejecutar en modo interactivo
```bash
make run
```
En este modo puedes escribir o pegar código fuente directamente en la consola. Para finalizar la entrada y procesarla, escribe `FIN` en una línea nueva y presiona Enter.

### 4. Ejecutar pruebas adicionales
```bash
./bin/lexer tests/test_basico.txt
./bin/lexer tests/test_errores.txt
```

### 5. Limpiar archivos generados
```bash
make clean
```

---

## Formato de Salida

El analizador genera dos reportes principales en consola:
1. **Lista de Tokens:** Tabla tabulada que indica el código numérico, la categoría léxica y el lexema reconocido.
2. **Tabla de Símbolos:** Listado único de todos los identificadores válidos encontrados durante el análisis (código `1000`).
