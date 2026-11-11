#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "lexer.hpp"
#include "parser.hpp"
#include "symbol_table.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    SymbolTable tablaSimbolos;
    Lexer lexer(tablaSimbolos);
    string codigo;

    // Mostrar por defecto cada paso de PILA, ENTRADA y ACCION del ADP.
    bool mostrarTraza = true;
    string rutaArchivo;
    for (int i = 1; i < argc; ++i) {
        const string argumento = argv[i];
        if (argumento == "--traza") {
            mostrarTraza = true;
        } else if (argumento == "--sin-traza") {
            mostrarTraza = false;
        } else if (rutaArchivo.empty()) {
            rutaArchivo = argumento;
        } else {
            cerr << "Argumento no reconocido: " << argumento << '\n';
            return 1;
        }
    }

    const bool interactivo = rutaArchivo.empty();
    if (interactivo) {
        cout << "--- MODO INTERACTIVO ---\n";
        cout << "Escribe o pega el codigo fuente aqui.\n";
        cout << "(Escribe 'FIN' en una linea nueva y presiona Enter para terminar y analizar):\n\n";

        string linea;
        while (getline(cin, linea)) {
            if (linea == "FIN") break;
            codigo += linea + "\n";
        }
        cout << "\n--- INICIANDO ANALISIS LEXICO ---\n";
    } else {
        ifstream archivo(rutaArchivo);
        if (!archivo.is_open()) {
            cerr << "Error al abrir el archivo: " << rutaArchivo << '\n';
            return 1;
        }
        stringstream buffer;
        buffer << archivo.rdbuf();
        codigo = buffer.str();
        cout << "--- ANALISIS LEXICO DE: " << rutaArchivo << " ---\n";
    }

    const vector<Token> tokens = lexer.analizar(codigo);

    cout << "TOKEN\tDESCRIPCION\t\tLEXEMA\n";
    for (const auto& token : tokens) {
        cout << token.codigo << "\t" << token.descripcion << "\t\t" << token.lexema << '\n';
    }

    cout << "\n--- TABLA DE SIMBOLOS ---\n";
    for (const auto& identificador : tablaSimbolos.obtener_tabla()) {
        cout << "1000\tIdentificador\t\t" << identificador << '\n';
    }

    cout << "\n--- INICIANDO ANALISIS SINTACTICO ADP LL(1) ---\n";
    if (mostrarTraza) {
        cout << "\n--- EVOLUCION DE LA PILA: PILA | ENTRADA | ACCION ---\n";
    }
    Parser parser;
    const ResultadoSintactico resultado = parser.analizar(tokens, mostrarTraza);

    if (resultado.aceptado) {
        cout << "RESULTADO: SINTAXIS ACEPTADA\n";
    } else {
        cout << "RESULTADO: SINTAXIS RECHAZADA\n";
        cout << "Errores lexicos: " << resultado.erroresLexicos << '\n';
        cout << "Errores sintacticos: " << resultado.erroresSintacticos << '\n';
        for (const auto& mensaje : resultado.mensajes) {
            cout << "- " << mensaje << '\n';
        }
    }

    if (interactivo) {
        cout << "\nPresione Enter para salir...";
        cin.get();
    }

    return resultado.aceptado ? 0 : 1;
}
