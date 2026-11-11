#include "parser.hpp"
#include <map>
#include <set>
#include <string>
#include <vector>
#include <stdexcept>
#include <utility>

// Estructuras internas del analizador sintactico.
#include <map>
#include <set>
#include <string>
#include <vector>

struct Production {
    int numero;
    std::string izquierda;
    std::vector<std::string> derecha;
};

class Grammar {
public:
    Grammar();
    bool esNoTerminal(const std::string& simbolo) const;
    const Production* buscar(const std::string& noTerminal, const std::string& anticipacion) const;
    const std::set<std::string>& siguientes(const std::string& noTerminal) const;
    size_t cantidadProducciones() const { return producciones_.size(); }
    size_t cantidadEntradas() const { return entradas_; }
private:
    std::vector<Production> producciones_;
    std::set<std::string> noTerminales_;
    std::map<std::string, std::set<std::string>> primero_;
    std::map<std::string, std::set<std::string>> siguiente_;
    std::map<std::pair<std::string, std::string>, size_t> tas_;
    size_t entradas_ = 0;
    void agregar(int numero, const std::string& izquierda, std::vector<std::string> derecha);
    std::set<std::string> primeroSecuencia(const std::vector<std::string>& secuencia) const;
    void calcularTablas();
};

namespace { const std::string VACIO = "#EPSILON#"; }

void Grammar::agregar(int numero, const std::string& izquierda,
                     std::vector<std::string> derecha) {
    producciones_.push_back({numero, izquierda, std::move(derecha)});
    noTerminales_.insert(izquierda);
}

Grammar::Grammar() {
// Producciones P1..P185 del informe ADP LL(1) de FlowxyRust.
    agregar(1, "S", {"Declaraciones_Globales", "Clases", "Implementaciones", "Funciones", "Funcion_Principal"});
    agregar(2, "Declaraciones_Globales", {"Constante", "Declaraciones_Globales"});
    agregar(3, "Declaraciones_Globales", {"Declaracion_Variable_Vector", "Declaraciones_Globales"});
    agregar(4, "Declaraciones_Globales", {});
    agregar(5, "Constante", {"constante", "id", ":", "Tipo_Dato", "=", "Valor", ";"});
    agregar(6, "Declaracion_Variable_Vector", {"sea", "Mutabilidad", "id", ":", "Contenido_Declaracion", ";"});
    agregar(7, "Contenido_Declaracion", {"Tipo_Dato", "Inicializacion"});
    agregar(8, "Contenido_Declaracion", {"[", "Tipo_Dato", ";", "literal_entero", "]", "Inicializacion_Vector"});
    agregar(9, "Mutabilidad", {"mutable"});
    agregar(10, "Mutabilidad", {});
    agregar(11, "Inicializacion", {"=", "Expresion"});
    agregar(12, "Inicializacion", {});
    agregar(13, "Inicializacion_Vector", {"=", "Valor_Vector"});
    agregar(14, "Inicializacion_Vector", {});
    agregar(15, "Valor_Vector", {"[", "Valores", "]"});
    agregar(16, "Valores", {"Valor", "Continuacion_Valores"});
    agregar(17, "Continuacion_Valores", {",", "Valor", "Continuacion_Valores"});
    agregar(18, "Continuacion_Valores", {});
    agregar(19, "Tipo_Dato", {"Entero"});
    agregar(20, "Tipo_Dato", {"Decimal"});
    agregar(21, "Tipo_Dato", {"Natural"});
    agregar(22, "Tipo_Dato", {"cadena"});
    agregar(23, "Tipo_Dato", {"caracter"});
    agregar(24, "Tipo_Dato", {"booleano"});
    agregar(25, "Entero", {"entero16"});
    agregar(26, "Entero", {"entero32"});
    agregar(27, "Entero", {"entero64"});
    agregar(28, "Decimal", {"flotante16"});
    agregar(29, "Decimal", {"flotante32"});
    agregar(30, "Decimal", {"flotante64"});
    agregar(31, "Natural", {"natural16"});
    agregar(32, "Natural", {"natural32"});
    agregar(33, "Natural", {"natural64"});
    agregar(34, "Tipo_Entero", {"Entero"});
    agregar(35, "Tipo_Entero", {"Natural"});
    agregar(36, "Literal", {"Numero"});
    agregar(37, "Literal", {"literal_cadena"});
    agregar(38, "Literal", {"literal_caracter"});
    agregar(39, "Literal", {"verdadero"});
    agregar(40, "Literal", {"falso"});
    agregar(41, "Numero", {"literal_entero"});
    agregar(42, "Numero", {"literal_decimal"});
    agregar(43, "Valor", {"Literal"});
    agregar(44, "Valor", {"-", "Numero"});
    agregar(45, "Clases", {"Clase", "Clases"});
    agregar(46, "Clases", {});
    agregar(47, "Clase", {"clase", "id", "{", "Atributos", "}"});
    agregar(48, "Atributos", {"Declaracion_Atributo", "Continuacion_Atributos"});
    agregar(49, "Continuacion_Atributos", {"Declaracion_Atributo", "Continuacion_Atributos"});
    agregar(50, "Continuacion_Atributos", {});
    agregar(51, "Declaracion_Atributo", {"sea", "id", ":", "Tipo_Dato", ";"});
    agregar(52, "Implementaciones", {"Implementacion", "Implementaciones"});
    agregar(53, "Implementaciones", {});
    agregar(54, "Implementacion", {"implementa", "id", "{", "Metodos", "}"});
    agregar(55, "Metodos", {"Metodo", "Continuacion_Metodos"});
    agregar(56, "Continuacion_Metodos", {"Metodo", "Continuacion_Metodos"});
    agregar(57, "Continuacion_Metodos", {});
    agregar(58, "Metodo", {"fun", "Mutabilidad", "id", "(", "Parametros", ")", "Tipo_Retorno", "{", "Sentencias", "}"});
    agregar(59, "Funciones", {"Funcion", "Funciones"});
    agregar(60, "Funciones", {});
    agregar(61, "Funcion", {"fun", "id", "(", "Parametros", ")", "Tipo_Retorno", "{", "Sentencias", "}"});
    agregar(62, "Funcion_Principal", {"mainfun", "(", ")", "{", "Sentencias", "}"});
    agregar(63, "Parametros", {"Parametro", "Continuacion_Parametros"});
    agregar(64, "Parametros", {});
    agregar(65, "Parametro", {"id", ":", "Tipo_Dato"});
    agregar(66, "Continuacion_Parametros", {",", "Parametro", "Continuacion_Parametros"});
    agregar(67, "Continuacion_Parametros", {});
    agregar(68, "Tipo_Retorno", {":", "Tipo_Dato"});
    agregar(69, "Tipo_Retorno", {});
    agregar(70, "Sentencias", {"Instruccion", "Sentencias"});
    agregar(71, "Sentencias", {});
    agregar(72, "Instruccion", {"Constante"});
    agregar(73, "Instruccion", {"Declaracion_Sea"});
    agregar(74, "Instruccion", {"Instruccion_Id"});
    agregar(75, "Instruccion", {"Instruccion_Este"});
    agregar(76, "Instruccion", {"Entrada"});
    agregar(77, "Instruccion", {"Salida"});
    agregar(78, "Instruccion", {"Retorno"});
    agregar(79, "Instruccion", {"Condicional"});
    agregar(80, "Instruccion", {"Mientras"});
    agregar(81, "Instruccion", {"Para"});
    agregar(82, "Declaracion_Sea", {"sea", "Mutabilidad", "id", ":", "Contenido_Declaracion_Local", ";"});
    agregar(83, "Contenido_Declaracion_Local", {"Contenido_Declaracion"});
    agregar(84, "Contenido_Declaracion_Local", {"id", "Inicializacion_Objeto"});
    agregar(85, "Inicializacion_Objeto", {"=", "Valor_Objeto"});
    agregar(86, "Inicializacion_Objeto", {});
    agregar(87, "Valor_Objeto", {"id", "{", "Valores_Atributos", "}"});
    agregar(88, "Valores_Atributos", {"Valor_Atributo", "Continuacion_Valores_Atributos"});
    agregar(89, "Valor_Atributo", {"id", ":", "Expresion"});
    agregar(90, "Continuacion_Valores_Atributos", {",", "Valor_Atributo", "Continuacion_Valores_Atributos"});
    agregar(91, "Continuacion_Valores_Atributos", {});
    agregar(92, "Instruccion_Id", {"id", "Continuacion_Instruccion_Id"});
    agregar(93, "Continuacion_Instruccion_Id", {"Modificacion", ";"});
    agregar(94, "Continuacion_Instruccion_Id", {"[", "Expresion", "]", "Modificacion_Basica", ";"});
    agregar(95, "Continuacion_Instruccion_Id", {"(", "Argumentos", ")", ";"});
    agregar(96, "Continuacion_Instruccion_Id", {".", "id", "Continuacion_Instruccion_Miembro"});
    agregar(97, "Instruccion_Este", {"este", ".", "id", "Continuacion_Instruccion_Miembro"});
    agregar(98, "Continuacion_Instruccion_Miembro", {"Modificacion_Basica", ";"});
    agregar(99, "Continuacion_Instruccion_Miembro", {"(", "Argumentos", ")", ";"});
    agregar(100, "Modificacion", {"=", "Contenido_Asignacion"});
    agregar(101, "Modificacion", {"Operador_Asignacion_Compuesta", "Expresion"});
    agregar(102, "Modificacion", {"++"});
    agregar(103, "Modificacion", {"--"});
    agregar(104, "Modificacion_Basica", {"++"});
    agregar(105, "Modificacion_Basica", {"--"});
    agregar(106, "Modificacion_Basica", {"=", "Expresion"});
    agregar(107, "Modificacion_Basica", {"Operador_Asignacion_Compuesta", "Expresion"});
    agregar(108, "Operador_Asignacion_Compuesta", {"+="});
    agregar(109, "Operador_Asignacion_Compuesta", {"-="});
    agregar(110, "Operador_Asignacion_Compuesta", {"*="});
    agregar(111, "Operador_Asignacion_Compuesta", {"/="});
    agregar(112, "Operador_Asignacion_Compuesta", {"%="});
    agregar(113, "Contenido_Asignacion", {"Valor_Vector"});
    agregar(114, "Contenido_Asignacion", {"id", "Asignacion_Tras_Id"});
    agregar(115, "Contenido_Asignacion", {"Unaria_Sin_Id_Inicial", "Continuacion_Expresion"});
    agregar(116, "Asignacion_Tras_Id", {"{", "Valores_Atributos", "}"});
    agregar(117, "Asignacion_Tras_Id", {"Continuacion_Identificador", "Continuacion_Expresion"});
    agregar(118, "Unaria_Sin_Id_Inicial", {"Literal"});
    agregar(119, "Unaria_Sin_Id_Inicial", {"este", ".", "id", "Llamada_Metodo_Opcional"});
    agregar(120, "Unaria_Sin_Id_Inicial", {"(", "Expresion", ")"});
    agregar(121, "Unaria_Sin_Id_Inicial", {"-", "Expresion_Unaria"});
    agregar(122, "Unaria_Sin_Id_Inicial", {"!", "Expresion_Unaria"});
    agregar(123, "Continuacion_Expresion", {"Continuacion_Producto", "Continuacion_Suma_Resta", "Continuacion_Comparacion", "Continuacion_Igualdad", "Continuacion_Conjuncion", "Continuacion_Disyuncion"});
    agregar(124, "Entrada", {"leer", "(", "Destino", ")", ";"});
    agregar(125, "Salida", {"mostrar", "(", "Expresion", ")", ";"});
    agregar(126, "Destino", {"id", "Continuacion_Destino"});
    agregar(127, "Destino", {"este", ".", "id"});
    agregar(128, "Continuacion_Destino", {"[", "Expresion", "]"});
    agregar(129, "Continuacion_Destino", {".", "id"});
    agregar(130, "Continuacion_Destino", {});
    agregar(131, "Retorno", {"retornar", "Valor_Retorno_Opcional", ";"});
    agregar(132, "Valor_Retorno_Opcional", {"Expresion"});
    agregar(133, "Valor_Retorno_Opcional", {});
    agregar(134, "Condicional", {"si", "(", "Condicion", ")", "{", "Sentencias", "}", "Sino_Opcional"});
    agregar(135, "Sino_Opcional", {"sino", "{", "Sentencias", "}"});
    agregar(136, "Sino_Opcional", {});
    agregar(137, "Condicion", {"Expresion"});
    agregar(138, "Mientras", {"mientras", "(", "Condicion", ")", "{", "Sentencias", "}"});
    agregar(139, "Para", {"para", "(", "Inicializacion_Para", ";", "Condicion", ";", "Actualizacion_Para", ")", "{", "Sentencias", "}"});
    agregar(140, "Inicializacion_Para", {"sea", "mutable", "id", ":", "Tipo_Entero", "=", "Expresion"});
    agregar(141, "Inicializacion_Para", {"id", "=", "Expresion"});
    agregar(142, "Actualizacion_Para", {"id", "Modificacion_Basica"});
    agregar(143, "Expresion", {"Disyuncion_Logica"});
    agregar(144, "Disyuncion_Logica", {"Conjuncion_Logica", "Continuacion_Disyuncion"});
    agregar(145, "Continuacion_Disyuncion", {"||", "Conjuncion_Logica", "Continuacion_Disyuncion"});
    agregar(146, "Continuacion_Disyuncion", {});
    agregar(147, "Conjuncion_Logica", {"Comparacion_Igualdad", "Continuacion_Conjuncion"});
    agregar(148, "Continuacion_Conjuncion", {"&&", "Comparacion_Igualdad", "Continuacion_Conjuncion"});
    agregar(149, "Continuacion_Conjuncion", {});
    agregar(150, "Comparacion_Igualdad", {"Comparacion_Orden", "Continuacion_Igualdad"});
    agregar(151, "Continuacion_Igualdad", {"==", "Comparacion_Orden", "Continuacion_Igualdad"});
    agregar(152, "Continuacion_Igualdad", {"!=", "Comparacion_Orden", "Continuacion_Igualdad"});
    agregar(153, "Continuacion_Igualdad", {});
    agregar(154, "Comparacion_Orden", {"Suma_Resta", "Continuacion_Comparacion"});
    agregar(155, "Continuacion_Comparacion", {"<", "Suma_Resta", "Continuacion_Comparacion"});
    agregar(156, "Continuacion_Comparacion", {"<=", "Suma_Resta", "Continuacion_Comparacion"});
    agregar(157, "Continuacion_Comparacion", {">", "Suma_Resta", "Continuacion_Comparacion"});
    agregar(158, "Continuacion_Comparacion", {">=", "Suma_Resta", "Continuacion_Comparacion"});
    agregar(159, "Continuacion_Comparacion", {});
    agregar(160, "Suma_Resta", {"Multiplicacion_Division_Modulo", "Continuacion_Suma_Resta"});
    agregar(161, "Continuacion_Suma_Resta", {"+", "Multiplicacion_Division_Modulo", "Continuacion_Suma_Resta"});
    agregar(162, "Continuacion_Suma_Resta", {"-", "Multiplicacion_Division_Modulo", "Continuacion_Suma_Resta"});
    agregar(163, "Continuacion_Suma_Resta", {});
    agregar(164, "Multiplicacion_Division_Modulo", {"Expresion_Unaria", "Continuacion_Producto"});
    agregar(165, "Continuacion_Producto", {"*", "Expresion_Unaria", "Continuacion_Producto"});
    agregar(166, "Continuacion_Producto", {"/", "Expresion_Unaria", "Continuacion_Producto"});
    agregar(167, "Continuacion_Producto", {"%", "Expresion_Unaria", "Continuacion_Producto"});
    agregar(168, "Continuacion_Producto", {});
    agregar(169, "Expresion_Unaria", {"-", "Expresion_Unaria"});
    agregar(170, "Expresion_Unaria", {"!", "Expresion_Unaria"});
    agregar(171, "Expresion_Unaria", {"Operando"});
    agregar(172, "Operando", {"Literal"});
    agregar(173, "Operando", {"id", "Continuacion_Identificador"});
    agregar(174, "Operando", {"este", ".", "id", "Llamada_Metodo_Opcional"});
    agregar(175, "Operando", {"(", "Expresion", ")"});
    agregar(176, "Continuacion_Identificador", {"[", "Expresion", "]"});
    agregar(177, "Continuacion_Identificador", {"(", "Argumentos", ")"});
    agregar(178, "Continuacion_Identificador", {".", "id", "Llamada_Metodo_Opcional"});
    agregar(179, "Continuacion_Identificador", {});
    agregar(180, "Llamada_Metodo_Opcional", {"(", "Argumentos", ")"});
    agregar(181, "Llamada_Metodo_Opcional", {});
    agregar(182, "Argumentos", {"Expresion", "Continuacion_Argumentos"});
    agregar(183, "Argumentos", {});
    agregar(184, "Continuacion_Argumentos", {",", "Expresion", "Continuacion_Argumentos"});
    agregar(185, "Continuacion_Argumentos", {});    calcularTablas();
}

bool Grammar::esNoTerminal(const std::string& simbolo) const {
    return noTerminales_.count(simbolo) != 0;
}

std::set<std::string> Grammar::primeroSecuencia(const std::vector<std::string>& secuencia) const {
    std::set<std::string> resultado;
    if (secuencia.empty()) { resultado.insert(VACIO); return resultado; }
    bool todosVacios = true;
    for (const auto& simbolo : secuencia) {
        if (!esNoTerminal(simbolo)) {
            resultado.insert(simbolo);
            todosVacios = false;
            break;
        }
        const auto& conjunto = primero_.at(simbolo);
        for (const auto& x : conjunto) if (x != VACIO) resultado.insert(x);
        if (!conjunto.count(VACIO)) { todosVacios = false; break; }
    }
    if (todosVacios) resultado.insert(VACIO);
    return resultado;
}

void Grammar::calcularTablas() {
    for (const auto& n : noTerminales_) {
        primero_[n] = {};
        siguiente_[n] = {};
    }
    siguiente_["S"].insert("$");
    bool cambio;
    do {
        cambio = false;
        for (const auto& p : producciones_) {
            auto f = primeroSecuencia(p.derecha);
            size_t antes = primero_[p.izquierda].size();
            primero_[p.izquierda].insert(f.begin(), f.end());
            cambio |= primero_[p.izquierda].size() != antes;
        }
    } while (cambio);

    do {
        cambio = false;
        for (const auto& p : producciones_) {
            for (size_t i = 0; i < p.derecha.size(); ++i) {
                const auto& n = p.derecha[i];
                if (!esNoTerminal(n)) continue;
                std::vector<std::string> sufijo(p.derecha.begin()+i+1, p.derecha.end());
                auto f = primeroSecuencia(sufijo);
                size_t antes = siguiente_[n].size();
                for (const auto& x : f) if (x != VACIO) siguiente_[n].insert(x);
                if (f.count(VACIO)) {
                    const auto& sig = siguiente_.at(p.izquierda);
                    siguiente_[n].insert(sig.begin(), sig.end());
                }
                cambio |= siguiente_[n].size() != antes;
            }
        }
    } while (cambio);

    for (size_t i = 0; i < producciones_.size(); ++i) {
        const auto& p = producciones_[i];
        auto f = primeroSecuencia(p.derecha);
        std::set<std::string> entradas;
        for (const auto& x : f) if (x != VACIO) entradas.insert(x);
        if (f.count(VACIO)) {
            const auto& sig = siguiente_.at(p.izquierda);
            entradas.insert(sig.begin(), sig.end());
        }
        for (const auto& terminal : entradas) {
            const auto clave = std::make_pair(p.izquierda, terminal);
            if (tas_.count(clave)) {
                throw std::runtime_error("Conflicto LL(1) en " + p.izquierda + " / " + terminal);
            }
            tas_[clave] = i;
        }
    }
    entradas_ = tas_.size();
    if (producciones_.size() != 185 || entradas_ != 521) {
        throw std::runtime_error("La gramatica y la TAS difieren del informe ADP");
    }
}

const Production* Grammar::buscar(const std::string& nt, const std::string& t) const {
    auto it = tas_.find({nt,t});
    return it == tas_.end() ? nullptr : &producciones_.at(it->second);
}

const std::set<std::string>& Grammar::siguientes(const std::string& nt) const {
    return siguiente_.at(nt);
}

// Algoritmo ADP: pila + TAS calculada desde las producciones anteriores.

#include <iostream>
#include <sstream>
#include <unordered_map>

Parser::Parser() = default;

std::string Parser::terminal(const Token& token) {
    if (token.codigo == 1000) return "id";
    if (token.codigo == 2010) return "literal_entero";
    if (token.codigo == 2020) return "literal_decimal";
    if (token.codigo == 3000) return "literal_caracter";
    if (token.codigo == 4000) return "literal_cadena";
    // Compatibilidad con versiones anteriores del lexer que entregan 2000.
    if (token.codigo == 2000) {
        return token.lexema.find('.') == std::string::npos ? "literal_entero" : "literal_decimal";
    }
    // Reservadas, operadores y delimitadores: el lexema es el terminal.
    if ((token.codigo >= 100 && token.codigo <= 128) ||
        (token.codigo >= 200 && token.codigo <= 221) ||
        (token.codigo >= 300 && token.codigo <= 309)) return token.lexema;
    return "#DESCONOCIDO#";
}

ResultadoSintactico Parser::analizar(const std::vector<Token>& tokens, bool mostrarTraza) const {
    Grammar gramatica;
    ResultadoSintactico resultado;
    std::vector<std::string> entrada;
    std::vector<std::string> lexemas;
    for (const auto& token : tokens) {
        if (token.codigo == 911) {
            ++resultado.erroresLexicos;
            resultado.mensajes.push_back("Error lexico: '" + token.lexema + "'");
            continue; // El analizador lexico continua, pero el programa no se acepta.
        }
        entrada.push_back(terminal(token));
        lexemas.push_back(token.lexema);
    }
    entrada.push_back("$");
    lexemas.push_back("$");

    std::vector<std::string> pila = {"$", "S"};
    size_t pos = 0;
    size_t pasos = 0;
    const size_t limite = (entrada.size() + 1) * 1500;
    bool llegoAlFinal = false;
    auto error = [&](const std::string& texto) {
        ++resultado.erroresSintacticos;
        resultado.mensajes.push_back(texto);
    };

    // Formato visual de la traza. Solo modifica la presentacion.
    // Cada bloque representa un movimiento del ADP, antes de ejecutar su accion.
    auto imprimirEnvuelto = [](const std::vector<std::string>& elementos) {
        constexpr size_t ancho = 74;
        std::string linea = "      ";
        for (const auto& elemento : elementos) {
            if (linea.size() > 6 && linea.size() + elemento.size() + 1 > ancho) {
                std::cout << linea << '\n';
                linea = "      ";
            }
            if (linea.size() > 6) linea += ' ';
            linea += elemento;
        }
        std::cout << linea << '\n';
    };

    auto imprimirPaso = [&](size_t numero, const std::string& cima,
                           const std::string& tokenActual,
                           const std::string& accion) {
        if (!mostrarTraza) return;
        constexpr size_t ancho = 78;
        std::cout << '\n' << std::string(ancho, '=') << '\n';
        std::cout << "  PASO " << numero << " / ADP LL(1)" << '\n';
        std::cout << std::string(ancho, '-') << '\n';

        std::cout << "  1) SIMBOLOS A COMPARAR\n";
        std::cout << "      Cima de pila  : " << cima << '\n';
        std::cout << "      Token actual  : " << tokenActual << '\n';

        std::cout << "  2) PILA ANTES DEL MOVIMIENTO (fondo -> cima)\n";
        std::vector<std::string> vistaPila = pila;
        if (!vistaPila.empty()) vistaPila.back() = "[" + vistaPila.back() + "]";
        imprimirEnvuelto(vistaPila);

        std::cout << "  3) ENTRADA QUE FALTA ANALIZAR\n";
        imprimirEnvuelto(std::vector<std::string>(entrada.begin() + pos, entrada.end()));

        std::cout << "  4) ACCION QUE SE APLICA\n";
        std::istringstream lector(accion);
        std::vector<std::string> accionPartes;
        std::string parte;
        while (lector >> parte) accionPartes.push_back(parte);
        imprimirEnvuelto(accionPartes);
        std::cout << std::string(ancho, '=') << '\n';
    };

    if (mostrarTraza) {
        std::cout << "\n" << std::string(78, '#') << '\n';
        std::cout << "#  TRAZA VISUAL V2 - ANALIZADOR SINTACTICO ADP LL(1)\n";
        std::cout << "#  Cada PASO es un movimiento; [SIMBOLO] marca la cima.\n";
        std::cout << "#  El siguiente PASO muestra el resultado del anterior.\n";
        std::cout << std::string(78, '#') << '\n';
    }

    while (!pila.empty() && pos < entrada.size() && pasos < limite) {
        ++pasos;
        const std::string cima = pila.back();
        const std::string actual = entrada[pos];

        if (cima == "$" && actual == "$") {
            llegoAlFinal = true;
            imprimirPaso(pasos, cima, actual, "ACEPTAR: pila y entrada llegaron al simbolo $");
            break;
        }
        if (cima == "$" && actual != "$") {
            imprimirPaso(pasos, cima, actual, "ERROR: token sobrante '" + lexemas[pos] + "'; se descarta");
            error("Token sobrante '" + lexemas[pos] + "' al final del programa");
            ++pos;
            continue;
        }
        if (!gramatica.esNoTerminal(cima)) {
            if (cima == actual) {
                imprimirPaso(pasos, cima, actual, "COINCIDIR: '" + cima + "'; desapilar y avanzar entrada");
                pila.pop_back();
                ++pos;
            } else {
                imprimirPaso(pasos, cima, actual, "ERROR: se esperaba '" + cima +
                            "' antes de '" + lexemas[pos] + "'; retirar terminal esperado");
                error("Se esperaba '" + cima + "' antes de '" + lexemas[pos] + "'");
                pila.pop_back(); // Recuperacion por insercion del terminal faltante.
            }
            continue;
        }
        if (const auto* p = gramatica.buscar(cima, actual)) {
            std::string regla = "P" + std::to_string(p->numero) + ": " + p->izquierda + " ->";
            if (p->derecha.empty()) regla += " lambda (retirar no terminal)";
            else {
                for (const auto& simbolo : p->derecha) regla += " " + simbolo;
            }
            imprimirPaso(pasos, cima, actual, regla);
            pila.pop_back();
            for (auto it = p->derecha.rbegin(); it != p->derecha.rend(); ++it)
                pila.push_back(*it);
        } else {
            const auto& sig = gramatica.siguientes(cima);
            error("No hay produccion para " + cima + " con el token '" + lexemas[pos] + "'");
            if (actual == "$" || sig.count(actual)) {
                imprimirPaso(pasos, cima, actual,
                            "ERROR TAS: sin produccion para " + cima +
                            "; sincronizar retirando el no terminal");
                pila.pop_back();
            } else {
                imprimirPaso(pasos, cima, actual,
                            "ERROR TAS: sin produccion para " + cima +
                            "; descartar token '" + lexemas[pos] + "'");
                ++pos;
            }
        }
    }
    if (pasos >= limite) error("Se alcanzo el limite de pasos de recuperacion");
    resultado.aceptado = llegoAlFinal && resultado.erroresLexicos == 0 && resultado.erroresSintacticos == 0;
    return resultado;
}
