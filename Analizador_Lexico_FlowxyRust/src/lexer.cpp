#include "lexer.hpp"
#include <cctype>
#include <unordered_map>

using namespace std;

unordered_map<string,int> palabrasReservadas = {
    {"sea",100}, {"constante",101}, {"mutable",102}, {"clase",103},
    {"implementa",104}, {"este",105}, {"entero16",106}, {"entero32",107},
    {"entero64",108}, {"natural16",109}, {"natural32",110}, {"natural64",111},
    {"flotante16",112}, {"flotante32",113}, {"flotante64",114}, {"caracter",115},
    {"cadena",116}, {"booleano",117}, {"verdadero",118}, {"falso",119},
    {"leer",120}, {"mostrar",121}, {"si",122}, {"sino",123},
    {"mientras",124}, {"para",125}, {"fun",126}, {"retornar",127},
    {"mainfun",128}
};

unordered_map<string,int> operadores = {
    {"-",200}, {"!",201}, {"+",202}, {"*",203}, {"/",204},
    {"%",205}, {"<",206}, {"<=",207}, {">",208}, {">=",209},
    {"==",210}, {"!=",211}, {"&&",212}, {"||",213}, {"=",214},
    {"+=",215}, {"-=",216}, {"*=",217}, {"/=",218}, {"%=",219},
    {"++",220}, {"--",221}
};

unordered_map<char,int> delimitadores = {
    {'(',300}, {')',301}, {'{',302}, {'}',303},
    {'[',304}, {']',305}, {':',306}, {';',307},
    {'.',308}, {',',309}
};

// Los patrones del informe usan letras ASCII (A-Z, a-z) y digitos (0-9).
static bool esLetra(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static bool esDigitoASCII(char c) {
    return c >= '0' && c <= '9';
}

static bool esAlfanumerico(char c) {
    return esLetra(c) || esDigitoASCII(c);
}

Lexer::Lexer(SymbolTable& tabla) : tablaSimbolos(tabla) {}

bool Lexer::esDigito(char c) {
    return esDigitoASCII(c);
}

Token Lexer::reconocerIdentificador(const string& entrada, int &i) {
    string lexema;
    bool incorrecto = false;

    // Consumir la secuencia de identificador hasta un separador.
    // Los guiones bajos incorrectos pertenecen a este mismo lexema.
    while (i < static_cast<int>(entrada.size()) &&
           (esAlfanumerico(entrada[i]) || entrada[i] == '_')) {
        if (entrada[i] == '_' &&
            (lexema.empty() || lexema.back() == '_')) {
            incorrecto = true;
        }
        lexema += entrada[i++];
    }

    if (!lexema.empty() && lexema.back() == '_') {
        incorrecto = true;
    }

    if (incorrecto) {
        return {911, "Error lexico", lexema};
    }
    if (palabrasReservadas.count(lexema)) {
        return {palabrasReservadas.at(lexema), "Palabra reservada", lexema};
    }

    tablaSimbolos.insertar(lexema);
    return {1000, "Identificador", lexema};
}

Token Lexer::reconocerNumero(const string& entrada, int &i) {
    string lexema;

    while (i < static_cast<int>(entrada.size()) && esDigito(entrada[i])) {
        lexema += entrada[i++];
    }

    // El punto solo pertenece al numero si le sigue un digito.
    // En otro caso se deja disponible como delimitador.
    if (i + 1 < static_cast<int>(entrada.size()) &&
        entrada[i] == '.' && esDigito(entrada[i + 1])) {
        lexema += entrada[i++];
        while (i < static_cast<int>(entrada.size()) && esDigito(entrada[i])) {
            lexema += entrada[i++];
        }
    }

    return {2000, "Literal numerico", lexema};
}

Token Lexer::reconocerCadena(const string& entrada, int &i) {
    string lexema;
    lexema += entrada[i++];

    // No se absorben las lineas siguientes si falta la comilla final.
    while (i < static_cast<int>(entrada.size()) &&
           entrada[i] != '"' && entrada[i] != '\n') {
        lexema += entrada[i++];
    }

    if (i < static_cast<int>(entrada.size()) && entrada[i] == '"') {
        lexema += entrada[i++];
        return {4000, "Literal de cadena", lexema};
    }

    return {911, "Error lexico", lexema};
}

Token Lexer::reconocerCaracter(const string& entrada, int &i) {
    string lexema;
    lexema += entrada[i++];

    // Caso valido: comilla, un caracter, comilla.
    if (i + 1 < static_cast<int>(entrada.size()) &&
        entrada[i] != '\n' && entrada[i] != '\'' &&
        entrada[i + 1] == '\'') {
        lexema += entrada[i++];
        lexema += entrada[i++];
        return {3000, "Literal de caracter", lexema};
    }

    // Caso invalido: encapsular hasta la comilla de cierre, pero
    // detenerse ante blancos, delimitadores u operadores para salvarlos.
    while (i < static_cast<int>(entrada.size()) &&
           !isspace(static_cast<unsigned char>(entrada[i])) &&
           !delimitadores.count(entrada[i]) &&
           !operadores.count(string(1, entrada[i]))) {
        char c = entrada[i++];
        lexema += c;
        if (c == '\'') break;
    }

    return {911, "Error lexico", lexema};
}

Token Lexer::reconocerOperador(const string& entrada, int &i) {
    string lexema(1, entrada[i]);

    if (i + 1 < static_cast<int>(entrada.size())) {
        string doble = lexema + entrada[i + 1];
        if (operadores.count(doble)) {
            i += 2;
            return {operadores.at(doble), "Operador", doble};
        }
    }

    i++;
    if (operadores.count(lexema)) {
        return {operadores.at(lexema), "Operador", lexema};
    }
    return {911, "Error lexico", lexema};
}

vector<Token> Lexer::analizar(const string& entrada) {
    vector<Token> tokens;
    int i = 0;

    while (i < static_cast<int>(entrada.size())) {
        if (isspace(static_cast<unsigned char>(entrada[i]))) {
            i++;
            continue;
        }

        if (entrada[i] == '/' && i + 1 < static_cast<int>(entrada.size()) &&
            entrada[i + 1] == '/') {
            while (i < static_cast<int>(entrada.size()) && entrada[i] != '\n') i++;
            continue;
        }

        if (entrada[i] == '"') {
            tokens.push_back(reconocerCadena(entrada, i));
            continue;
        }
        if (entrada[i] == '\'') {
            tokens.push_back(reconocerCaracter(entrada, i));
            continue;
        }
        if (esLetra(entrada[i])) {
            tokens.push_back(reconocerIdentificador(entrada, i));
            continue;
        }
        if (esDigito(entrada[i])) {
            tokens.push_back(reconocerNumero(entrada, i));
            continue;
        }

        string temp(1, entrada[i]);
        if (operadores.count(temp) || entrada[i] == '&' || entrada[i] == '|') {
            tokens.push_back(reconocerOperador(entrada, i));
            continue;
        }
        if (delimitadores.count(entrada[i])) {
            tokens.push_back({delimitadores.at(entrada[i]), "Delimitador", temp});
            i++;
            continue;
        }

        // Caracter que no forma parte de ningun patron reconocido.
        // Se emite un error aislado y el analisis continua.
        tokens.push_back({911, "Error lexico", temp});
        i++;
    }

    return tokens;
}
