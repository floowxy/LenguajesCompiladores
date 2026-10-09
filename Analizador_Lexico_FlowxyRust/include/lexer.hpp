#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>
#include "token.hpp"
#include "symbol_table.hpp"

class Lexer {
private:
    SymbolTable& tablaSimbolos;

    bool esDigito(char c);
    Token reconocerIdentificador(const std::string& entrada, int &i);
    Token reconocerNumero(const std::string& entrada, int &i);
    Token reconocerCadena(const std::string& entrada, int &i);
    Token reconocerCaracter(const std::string& entrada, int &i);
    Token reconocerOperador(const std::string& entrada, int &i);

public:
    Lexer(SymbolTable& tabla);
    std::vector<Token> analizar(const std::string& entrada);
};

#endif
