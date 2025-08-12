# Nome do executável final
TARGET = LiarsBar

# Compilador C++
CXX = g++

# Flags de compilação: 
# -std=c++20 para usar recursos modernos
# -include <ranges> e -include <format> para forçar a inclusão dos cabeçalhos em todos os ficheiros
CXXFLAGS = -std=c++20 -g -Wall -include ranges -include format

# Diretório da engine para os includes
ENGINE_DIR = ./ASCII_Engine

# Flags de include: Diz ao compilador para procurar ficheiros .hpp nestas pastas
INCLUDES = -I./src -I$(ENGINE_DIR)

# Bibliotecas a serem lincadas. Para Linux, use -lncurses.
LIBS = -lncurses

# Encontra automaticamente todos os ficheiros .cpp
SOURCES = $(wildcard src/*.cpp src/gameObjects/*.cpp src/scenes/*.cpp $(ENGINE_DIR)/*.cpp $(ENGINE_DIR)/core/*.cpp $(ENGINE_DIR)/gameObjects/*.cpp $(ENGINE_DIR)/input/*.cpp $(ENGINE_DIR)/utils/*.cpp)

# Converte a lista de fontes .cpp para uma lista de ficheiros objeto .o
OBJECTS = $(SOURCES:.cpp=.o)

# Regra principal: compila o alvo
all: $(TARGET)

# Regra para lincar os ficheiros objeto e criar o executável
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

# Regra para compilar ficheiros .cpp em ficheiros .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Regra para limpar os ficheiros compilados
clean:
	rm -f $(TARGET) $(OBJECTS)

# Comando para compilar e rodar
run: all
	./$(TARGET)