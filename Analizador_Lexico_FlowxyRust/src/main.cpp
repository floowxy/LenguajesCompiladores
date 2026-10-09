#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "lexer.hpp"
#include "symbol_table.hpp"

using namespace std;

int main(int argc, char* argv[]){
    SymbolTable tablaSimbolos;
    Lexer lexer(tablaSimbolos);
    string codigo = "";

    if (argc < 2) {
        cout << "--- MODO INTERACTIVO ---\n";
        cout << "Escribe o pega el codigo fuente aqui.\n";
        cout << "(Escribe 'FIN' en una linea nueva y presiona Enter para terminar y analizar):\n\n";
        
        string linea;
        while (getline(cin, linea)) {
            if (linea == "FIN") {
                break;
            }
            codigo += linea + "\n";
        }
        cout << "\n--- INICIANDO ANALISIS LEXICO ---\n";
    } else {
        ifstream archivo(argv[1]);
        if (!archivo.is_open()) {
            cerr << "Error al abrir el archivo: " << argv[1] << endl;
            return 1;
        }

        stringstream buffer;
        buffer << archivo.rdbuf();
        codigo = buffer.str();
        cout << "--- ANALISIS LEXICO DE: " << argv[1] << " ---\n";
    }

    vector<Token> resultado = lexer.analizar(codigo);

    cout<<"TOKEN\tDESCRIPCION\t\tLEXEMA\n";
    for(auto t:resultado){
        cout<<t.codigo<<"\t"<<t.descripcion<<"\t\t"<<t.lexema<<"\n";
    }

    cout<<"\n--- TABLA DE SIMBOLOS ---\n";
    for(auto id:tablaSimbolos.obtener_tabla()){
        cout<<"1000\tIdentificador\t\t"<<id<<"\n";
    }

    if (argc < 2) {
        cout << "\nPresione Enter para salir...";
        cin.get();
    }

    return 0;
}
