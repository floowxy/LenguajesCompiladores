#ifndef FLOWXY_PARSER_HPP
#define FLOWXY_PARSER_HPP
#include "token.hpp"
#include <string>
#include <vector>

struct ResultadoSintactico {
  bool aceptado = false;
  int erroresLexicos = 0;
  int erroresSintacticos = 0;
  std::vector<std::string> mensajes;
};

class Parser {
public:
  Parser();
  ResultadoSintactico analizar(const std::vector<Token> &tokens,
                               bool mostrarTraza = false) const;

private:
  static std::string terminal(const Token &token);
};
#endif
