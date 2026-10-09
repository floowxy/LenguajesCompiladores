#include "symbol_table.hpp"

void SymbolTable::insertar(const std::string& lexema) {
    tabla.insert(lexema);
}

const std::unordered_set<std::string>& SymbolTable::obtener_tabla() const {
    return tabla;
}

void SymbolTable::limpiar() {
    tabla.clear();
}
