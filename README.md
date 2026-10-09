# Lenguajes y Compiladores - UNMSM

Repositorio del curso **Lenguajes y Compiladores** (Facultad de Ingeniería de Sistemas e Informática - Escuela Profesional de Ciencias de la Computación, UNMSM).

---

## Estructura General del Repositorio

```text
LenguajesCompiladores/
├── docs/                                     # Informes técnicos y especificaciones del curso
│   ├── 01_Definicion_Lenguaje_FlowxyRust.pdf # Definición del lenguaje FlowxyRust
│   ├── 02_Laboratorio2_Gramatica.pdf         # Laboratorio 2: Gramática del lenguaje
│   └── 03_Laboratorio4_Analizador_Lexico.pdf # Laboratorio 4: Analizador léxico
│
├── Analizador_Lexico_FlowxyRust/             # Implementación del analizador léxico en C++17
│   ├── include/                              # Interfaces públicas (.hpp)
│   ├── src/                                  # Implementación (.cpp)
│   ├── examples/                             # Programa fuente de prueba
│   ├── tests/                                # Casos de prueba adicionales
│   ├── Makefile                              # Automatización de compilación y pruebas
│   ├── .gitignore                            # Exclusión de binarios y temporales
│   └── README.md                             # Documentación técnica del analizador
│
└── README.md                                 # Este archivo
```

---

## Documentación e Informes (`docs/`)

Los informes oficiales del proyecto se encuentran organizados en la carpeta [`docs/`](docs/):

1. **[01_Definicion_Lenguaje_FlowxyRust.pdf](docs/01_Definicion_Lenguaje_FlowxyRust.pdf):**
   - Especificación formal del lenguaje, tipos de datos, estructuras de control, funciones y paradigma.
2. **[02_Laboratorio2_Gramatica.pdf](docs/02_Laboratorio2_Gramatica.pdf):**
   - Definición de la gramática formal del lenguaje (BNF/EBNF) y reglas de producción.
3. **[03_Laboratorio4_Analizador_Lexico.pdf](docs/03_Laboratorio4_Analizador_Lexico.pdf):**
   - Definición detallada de tokens, palabras reservadas, operadores, delimitadores, autómatas finitos y diseño de la tabla de símbolos.

---

## Proyectos

- [**Analizador Léxico (FlowxyRust)**](Analizador_Lexico_FlowxyRust/): Analizador modular en C++17 que escanea el código fuente carácter por carácter, genera tokens con sus respectivos códigos numéricos y construye la tabla de símbolos para los identificadores.
