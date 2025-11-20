# ==========================================
# Makefile para o Projeto Metrô NY (C++)
# ==========================================

# Compilador e Flags
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -O2 -I src
# -std=c++17: Usa recursos modernos (filesystem, etc)
# -Wall: Mostra todos os avisos (importante para rigor técnico)
# -O2: Otimização de performance (essencial para Dijkstra/Kruskal)
# -I src: Inclui a pasta src nos headers

# Diretorios
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

# Nome do Executável Final
TARGET  := $(BIN_DIR)/metro_analyzer

# Lista automática de arquivos .cpp e .o
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

# Regra Padrão (apenas digitar 'make')
all: directories $(TARGET)

# Regra para Linkar o Executável
$(TARGET): $(OBJECTS)
	@echo "Linkando o executavel..."
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Sucesso! Executavel criado em: $(TARGET)"

# Regra para Compilar cada Arquivo .cpp em .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compilando $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Cria as pastas de build se não existirem
directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)
	@mkdir -p arquivos_relacionados/saidas_cpp

# Limpa os arquivos gerados (make clean)
clean:
	@echo "Limpando arquivos temporarios..."
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Compila e Roda o programa (make run)
run: all
	@echo "--- Executando o Programa ---"
	./$(TARGET)