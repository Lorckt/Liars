# Nome do executável final que será criado
TARGET = LiarsBar

# Compilador C++ que será usado (g++ é o padrão no Linux)

CXX = g++

# Flags de compilação:

# \-std=c++17: Usa a versão 2017 do C++, que tem recursos modernos.

# \-g: Inclui informações de debug, o que ajuda a encontrar erros.

CXXFLAGS = -std=c++17 -g

# Diretório da engine para que o compilador saiba onde encontrar os seus ficheiros de cabeçalho

ENGINE\_DIR = ./ASCII\_Engine/src

# Flags de include: Diz ao compilador para procurar ficheiros .hpp nestas pastas

INCLUDES = -I./src -I$(ENGINE\_DIR)

# Bibliotecas a serem ligadas (linkadas).

# \-lcurses: É a biblioteca ncurses, essencial para manipulação de terminal no Linux,

# que a Cpp-ASCII-Game-Engine utiliza.

LIBS = -lcurses

# Encontra automaticamente todos os ficheiros .cpp no diretório src e nos seus subdiretórios,

# e também os ficheiros da engine. Não precisa de adicionar ficheiros novos manualmente\!

SOURCES = $(wildcard src/*.cpp src/gameObjects/*.cpp src/scenes/*.cpp $(ENGINE\_DIR)/*.cpp $(ENGINE\_DIR)/Core/\*.cpp)

# Converte a lista de ficheiros .cpp para uma lista de ficheiros objeto .o

OBJECTS = $(SOURCES:.cpp=.o)

# Regra principal: 'make all' ou apenas 'make' irá construir o alvo (TARGET)

all: $(TARGET)

# Regra para ligar (linkar) todos os ficheiros objeto (.o) e criar o executável final

$(TARGET): $(OBJECTS)
$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

# Regra genérica para compilar um ficheiro .cpp num ficheiro objeto .o

# O compilador só irá refazer esta etapa para os ficheiros que foram modificados.

%.o: %.cpp
$(CXX) $(CXXFLAGS) $(INCLUDES) -c $\< -o $@

# Regra para limpar os ficheiros compilados e o executável

clean:
rm -f $(TARGET) $(OBJECTS)

# Regra de conveniência para compilar e executar o jogo com um único comando

run: all
./$(TARGET)
