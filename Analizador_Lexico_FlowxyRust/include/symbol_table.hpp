#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <string>
#include <unordered_set>

class SymbolTable {
private:
    std::unordered_set<std::string> tabla;
public:
    void insertar(const std::string& lexema);
    const std::unordered_set<std::string>& obtener_tabla() const;
    void limpiar();
};

#endif
