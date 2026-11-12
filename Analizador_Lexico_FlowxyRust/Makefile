
# Compiler and Flags
CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic -Iinclude

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
BIN_DIR = bin

# Source files and Object files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Target executable
TARGET = $(BIN_DIR)/flowxyrust

.PHONY: all clean run test help

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create directories if they do not exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Run interactive mode
run: $(TARGET)
	./$(TARGET)

# Run with example file
test: $(TARGET)
	./$(TARGET) examples/prueba_completa.txt

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Help message
help:
	@echo "Comandos disponibles:"
	@echo "  make         - Compila el proyecto"
	@echo "  make run     - Ejecuta el analizador en modo interactivo"
	@echo "  make test    - Ejecuta el analizador con el archivo de prueba (examples/prueba_completa.txt)"
	@echo "  make clean   - Elimina archivos generados (.o y binarios)"
	@echo "  make help    - Muestra este mensaje de ayuda"
