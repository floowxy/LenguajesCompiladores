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
    {"++",220}, {"--",221},
    {"&",911}, {"|",911}
};

unordered_map<char,int> delimitadores = {
    {'(',300}, {')',301}, {'{',302}, {'}',303},
    {'[',304}, {']',305}, {':',306}, {';',307},
    {'.',308}, {',',309}
};

Lexer::Lexer(SymbolTable& tabla) : tablaSimbolos(tabla) {}

bool Lexer::esDigito(char c){
    return isdigit(c);
}

Token Lexer::reconocerIdentificador(const string& entrada, int &i){
    string lexema;
    int estado = 1;

    while(i < entrada.size()){
        char c = entrada[i];
        if(estado == 1){
            if(isalnum(c)){ lexema += c; i++; }
            else if(c == '_'){ estado = 2; lexema += c; i++; }
            else { break; }
        } else if(estado == 2){
            if(isalnum(c)){ estado = 1; lexema += c; i++; }
            else { break; }
        }
    }

    if(estado == 2 || (i < entrada.size() && entrada[i] == '_')){
        while(i < entrada.size() && (isalnum(entrada[i]) || entrada[i] == '_')){
            lexema += entrada[i]; i++;
        }
        return {911, "Error lexico", lexema};
    }

    if(palabrasReservadas.count(lexema)){
        return {palabrasReservadas[lexema], "Palabra reservada", lexema};
    }

    tablaSimbolos.insertar(lexema);
    return {1000, "Identificador", lexema};
}

Token Lexer::reconocerNumero(const string& entrada, int &i){
    string lexema;
    int estado = 1;

    while(i < entrada.size()){
        char c = entrada[i];
        if(estado == 1){
            if(isdigit(c)){ lexema += c; i++; }
            else if(c == '.'){ estado = 2; lexema += c; i++; }
            else { break; }
        } else if(estado == 2){
            if(isdigit(c)){ estado = 3; lexema += c; i++; }
            else { break; }
        } else if(estado == 3){
            if(isdigit(c)){ lexema += c; i++; }
            else if (c == '.') { estado = 99; lexema += c; i++; break; }
            else { break; }
        }
    }

    if(estado == 2 || estado == 99){
        while(i < entrada.size() && (isalnum(entrada[i]) || entrada[i] == '.')){
            lexema += entrada[i]; i++;
        }
        return {911, "Error lexico", lexema};
    }

    return {2000, "Literal numerico", lexema};
}

Token Lexer::reconocerCadena(const string& entrada,int &i){
    string lexema;
    lexema += entrada[i];
    i++;
    while(i < entrada.size() && entrada[i]!='"'){
        lexema += entrada[i];
        i++;
    }
    if(i >= entrada.size()){
        return {911, "Error lexico", lexema};
    }
    lexema += '"';
    i++;
    return {4000, "Literal de cadena", lexema};
}

Token Lexer::reconocerCaracter(const string& entrada,int &i){
    string lexema;
    lexema += entrada[i++]; // lee la primera comilla
    
    if (i < entrada.size() && entrada[i] != '\'') {
        lexema += entrada[i++]; // lee el carácter
        
        if (i < entrada.size() && entrada[i] == '\'') {
            lexema += entrada[i++]; // lee la segunda comilla
            return {3000, "Literal de caracter", lexema};
        }
    }
    
    // Si llegamos aqui, es un error lexico por caracter mal formado
    while (i < entrada.size() && entrada[i] != '\'' && !isspace(entrada[i])) {
        lexema += entrada[i++];
    }
    if (i < entrada.size() && entrada[i] == '\'') {
        lexema += entrada[i++];
    }
    return {911, "Error lexico", lexema};
}

Token Lexer::reconocerOperador(const string& entrada,int &i){
    string lexema;
    lexema += entrada[i];
    if(i+1 < entrada.size()){
        string doble = lexema + entrada[i+1];
        if(operadores.count(doble)){
            lexema = doble;
            i +=2;
            return {operadores[lexema], "Operador", lexema};
        }
    }
    i++;
    int tokenCode = operadores[lexema];
    if(tokenCode == 911) return {911, "Error lexico", lexema};
    return {tokenCode, "Operador", lexema};
}

vector<Token> Lexer::analizar(const string& entrada){
    vector<Token> tokens;
    int i=0;

    while(i < entrada.size()){
        if(isspace(entrada[i])){ i++; continue; }
        if(entrada[i]=='/' && i+1<entrada.size() && entrada[i+1]=='/'){
            while(i<entrada.size() && entrada[i]!='\n') i++;
            continue;
        }
        if(entrada[i]=='"'){ tokens.push_back(reconocerCadena(entrada,i)); continue; }
        if(entrada[i]=='\''){ tokens.push_back(reconocerCaracter(entrada,i)); continue; }
        if(isalpha(entrada[i])){ tokens.push_back(reconocerIdentificador(entrada,i)); continue; }
        if(esDigito(entrada[i])){ tokens.push_back(reconocerNumero(entrada,i)); continue; }
        
        string temp(1,entrada[i]);
        if(operadores.count(temp)){ tokens.push_back(reconocerOperador(entrada,i)); continue; }
        if(delimitadores.count(entrada[i])){
            tokens.push_back({delimitadores[entrada[i]], "Delimitador", temp});
            i++;
            continue;
        }
        tokens.push_back({911, "Error lexico", temp});
        i++;
    }
    return tokens;
}
