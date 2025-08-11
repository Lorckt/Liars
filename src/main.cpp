#include &quot;ASCII_Engine/AEngine.hpp&quot;
#include &quot;scenes/MainMenuScene.hpp&quot;
#include &lt;iostream&gt;
#include &lt;ctime&gt;
#include &lt;cstdlib&gt;
#include &lt;locale.h&gt; // Necessário para setlocale, que ativa o suporte a caracteres Unicode no Linux

// --- Bloco para configurar o terminal ---
// Inclui os cabeçalhos corretos dependendo do sistema operativo
\#ifdef \_WIN32
\#include \<windows.h\>
\#else
// No Linux, a própria engine (com ncurses) já lida com grande parte da configuração
\#endif
// --- Fim do Bloco de Configuração ---

void setupConsole() {
// Para Linux/macOS, esta função define a "localidade" do programa
// para a do sistema, o que geralmente ativa o suporte a UTF-8,
// permitindo que os símbolos de naipe sejam exibidos corretamente.
setlocale(LC\_ALL, "");

#ifdef _WIN32
// Configuração específica para o terminal do Windows
SetConsoleOutputCP(CP_UTF8);
HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
if (hOut != INVALID_HANDLE_VALUE) {
DWORD dwMode = 0;
if (GetConsoleMode(hOut, &dwMode)) {
dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
SetConsoleMode(hOut, dwMode);
}
}
#endif


}

int main() {
// Inicializa o gerador de números aleatórios com base no tempo atual
// Essencial para que a roleta russa e o embaralhar das cartas sejam diferentes a cada jogo
srand(time(0));

// Chama a função para preparar o terminal para cores e símbolos
setupConsole();

try {
// Cria uma instância da engine do jogo
AEngine engine;

// Adiciona a primeira cena (o menu principal) à engine
engine.addScene(new MainMenuScene());
engine.setCurrentScene("MainMenu");

// Inicia o loop principal do jogo. O programa ficará aqui até o jogador sair.
engine.run();
} catch (const std::exception& e) {
// Em caso de um erro fatal, exibe a mensagem de erro.
// Isto é importante para o debug e para não deixar o terminal num estado "quebrado".
std::cerr << "Ocorreu um erro fatal: " << e.what() << std::endl;
return 1;
}

return 0;


}
