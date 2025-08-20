#include "cautogui.hpp"
#include <iostream>
#include <fstream>
#include <X11/keysym.h>
#include <vector>
#include <cmath>
#include <sstream>

namespace cautogui
{
	Display *display = nullptr;

	void	init()
	{
		display = XOpenDisplay(nullptr);
		if (!display)
		{
			std::cerr << "Erro ao abrir display X11." << std::endl;
			exit(1);
		}
	}

	bool	onScreen(int x, int y)
	{
		int w = XDisplayWidth(display, DefaultScreen(display));
		int h = XDisplayHeight(display, DefaultScreen(display));
		return x >= 0 && x < w && y >= 0 && y < h;
	}

	std::pair<int, int> position()
	{
		Window root, child;
		int root_x, root_y, win_x, win_y;
		unsigned int mask;
		XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &root, &child, &root_x, &root_y, &win_x, &win_y, &mask);
		return {root_x, root_y};
	}

	KeyCode getKeyCode(const std::string& key)
	{
		KeySym sym = XStringToKeysym(key.c_str());
		return XKeysymToKeycode(display, sym);
	}

	void	press(const std::string& key, int presses)
	{
		KeyCode kc = getKeyCode(key);
		for (int i = 0; i < presses; ++i)
		{
			XTestFakeKeyEvent(display, kc, True, 0);
			XTestFakeKeyEvent(display, kc, False, 0);
			XSync(display, False);
		}
	}

	void	write(const std::string& text)
	{
		for (char c : text)
			press(std::string(1, c));
	}

	std::pair<int, int> center(const std::vector<int>& box)
	{
		if (box.size() != 4) return {-1, -1};
			return {box[0] + box[2]/2, box[1] + box[3]/2};
	}

	void	moveTo(int x, int y)
	{
		XTestFakeMotionEvent(display, -1, x, y, 0);
		XSync(display, False);
	}

	std::pair<int, int> size()
	{
		return {XDisplayWidth(display, DefaultScreen(display)), XDisplayHeight(display, DefaultScreen(display))};
	}

	void	move(int dx, int dy)
	{
		auto pos = position();
		moveTo(pos.first + dx, pos.second + dy);
	}

	void	drag(int dx, int dy)
	{
		XTestFakeButtonEvent(display, 1, True, 0);
		move(dx, dy);
		XTestFakeButtonEvent(display, 1, False, 0);
		XSync(display, False);
	}

	void	click()
	{
		XTestFakeButtonEvent(display, 1, True, 0);
		XTestFakeButtonEvent(display, 1, False, 0);
		XSync(display, False);
	}

	void	doubleClick()
	{
		click();
		click();
	}

	void	scroll(int amount)
	{
		unsigned int button = amount > 0 ? 4 : 5;
		for (int i = 0; i < std::abs(amount); ++i)
		{
			XTestFakeButtonEvent(display, button, True, 0);
			XTestFakeButtonEvent(display, button, False, 0);
		}
		XSync(display, False);
	}

	void	keyDown(const std::string& key)
	{
		KeyCode kc = getKeyCode(key);
		XTestFakeKeyEvent(display, kc, True, 0);
		XSync(display, False);
	}

	void	keyUp(const std::string& key)
	{
		KeyCode kc = getKeyCode(key);
		XTestFakeKeyEvent(display, kc, False, 0);
		XSync(display, False);
	}

	void	hotkey(const std::vector<std::string>& keys)
	{
		for (const auto& k : keys) keyDown(k);
		for (auto it = keys.rbegin(); it != keys.rend(); ++it) keyUp(*it);
	}

	void	help(void)
	{
		printf("Funções de cautogui (em C, usando X11):\n\n");

		printf("init(void): Inicializa o display X11. Sem parâmetros. Deve ser chamada primeiro.\n\n");

		printf("onScreen(int x, int y): Verifica se ponto (x, y) está na tela. Retorna bool (true/false).\n"
		   "- x: Coordenada X (int).\n"
		   "- y: Coordenada Y (int).\n\n");

		printf("position(void): Retorna posição atual do mouse como struct Point {x, y}. Sem parâmetros.\n\n");

		printf("press(const char* key, int presses): Pressiona tecla 'key' 'presses' vezes.\n"
		   "- key: Tecla como string (ex: \"c\").\n"
		   "- presses: Número de pressões (int, default 1).\n\n");

		printf("write(const char* text): Escreve texto simulando teclas.\n"
		   "- text: Texto a escrever (string).\n\n");

		printf("center(const struct Box* box): Retorna centro de uma caixa como struct Point.\n"
		   "- box: Struct Box {x, y, width, height}.\n\n");

		printf("locateOnScreen(const char* imagePath): Localiza imagem na tela (básico). Retorna struct Box.\n"
		   "- imagePath: Caminho da imagem PPM (string).\n\n");

		printf("screenshot(const char* filename): Captura tela e salva como PPM.\n"
		   "- filename: Nome do arquivo (string).\n\n");

		printf("moveTo(int x, int y): Move mouse para (x, y).\n"
		   "- x: Coordenada X (int).\n"
		   "- y: Coordenada Y (int).\n\n");

		printf("size(void): Retorna tamanho da tela como struct Point {width, height}. Sem parâmetros.\n\n");

		printf("move(int dx, int dy): Move mouse relativo (dx, dy).\n"
		   "- dx: Deslocamento X (int).\n"
		   "- dy: Deslocamento Y (int).\n\n");

		printf("drag(int dx, int dy): Arrasta mouse relativo (dx, dy).\n"
		   "- dx: Deslocamento X (int).\n"
		   "- dy: Deslocamento Y (int).\n\n");

		printf("click(void): Clique esquerdo. Sem parâmetros.\n\n");

		printf("doubleClick(void): Clique duplo esquerdo. Sem parâmetros.\n\n");

		printf("scroll(int amount): Rola mouse (positivo sobe).\n"
		   "- amount: Quantidade de rolagem (int).\n\n");

		printf("keyDown(const char* key): Pressiona tecla sem soltar.\n"
		   "- key: Tecla (string).\n\n");

		printf("keyUp(const char* key): Solta tecla.\n"
		   "- key: Tecla (string).\n\n");

		printf("hotkey(const char** keys, int num_keys): Combinação de teclas.\n"
		   "- keys: Array de teclas (const char**).\n"
		   "- num_keys: Número de teclas (int).\n\n");

		printf("help(void): Exibe esta ajuda. Sem parâmetros.\n");
	}
}
