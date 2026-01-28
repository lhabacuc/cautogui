#include "cautogui.h"
#include <stdio.h>
#include <stdlib.h>
#include <X11/keysym.h>
#include <string.h>

Display	*display = NULL;

void	init(void)
{
	display = XOpenDisplay(NULL);
	if (!display)
	{
		fprintf(stderr, "Erro ao abrir display X11.\n");
		exit(1);
	}
}

bool	onScreen(int x, int y)
{
	int	w = XDisplayWidth(display, DefaultScreen(display));
	int	h = XDisplayHeight(display, DefaultScreen(display));
	return (x >= 0 && x < w && y >= 0 && y < h);
}

struct Point	position(void)
{
	Window	root;
	Window	child;
	int	root_x;
	int	root_y;
	int	win_x;
	int	win_y;
	unsigned int	mask;

	XQueryPointer(display, RootWindow(display, DefaultScreen(display)),
			&root, &child, &root_x, &root_y, &win_x, &win_y, &mask);
	struct Point p = {root_x, root_y};
	return (p);
}

KeyCode	getKeyCode(const char* key)
{
	KeySym sym = XStringToKeysym(key);
	return XKeysymToKeycode(display, sym);
}

void	press(const char* key, int presses)
{
	KeyCode kc = getKeyCode(key);
	for (int i = 0; i < presses; ++i)
	{
		XTestFakeKeyEvent(display, kc, True, 0);
		XTestFakeKeyEvent(display, kc, False, 0);
		XSync(display, False);
	}
}

void	write(const char* text)
{
	for (size_t i = 0; i < strlen(text); ++i)
	{
		char c[2] = {text[i], '\0'};
		press(c, 1);
	}
}

struct Point	center(const struct Box* box)
{
	struct Point p = {-1, -1};
	if (box)
	{
		p.x = box->x + box->width / 2;
		p.y = box->y + box->height / 2;
	}
	return (p);
}

void	moveTo(int x, int y)
{
	XTestFakeMotionEvent(display, -1, x, y, 0);
	XSync(display, False);
}

struct Point	size(void)
{
	struct Point s = {XDisplayWidth(display, DefaultScreen(display)), XDisplayHeight(display, DefaultScreen(display))};
	return s;
}

void	move(int dx, int dy)
{
	struct Point pos = position();
	moveTo(pos.x + dx, pos.y + dy);
}

void	drag(int dx, int dy)
{
	XTestFakeButtonEvent(display, 1, True, 0);
	move(dx, dy);
	XTestFakeButtonEvent(display, 1, False, 0);
	XSync(display, False);
}

void	click(void)
{
	XTestFakeButtonEvent(display, 1, True, 0);
	XTestFakeButtonEvent(display, 1, False, 0);
	XSync(display, False);
}

void	doubleClick(void)
{
	click();
	click();
}

void	scroll(int amount)
{
	unsigned int button = amount > 0 ? 4 : 5;
	for (int i = 0; i < abs(amount); ++i)
	{
		XTestFakeButtonEvent(display, button, True, 0);
		XTestFakeButtonEvent(display, button, False, 0);
	}
	XSync(display, False);
}

void	keyDown(const char* key)
{
	KeyCode kc = getKeyCode(key);
	XTestFakeKeyEvent(display, kc, True, 0);
	XSync(display, False);
}

void	keyUp(const char* key)
{
	KeyCode kc = getKeyCode(key);
	XTestFakeKeyEvent(display, kc, False, 0);
	XSync(display, False);
}

void	hotkey(const char** keys, int num_keys)
{
	for (int i = 0; i < num_keys; ++i) keyDown(keys[i]);
	for (int i = num_keys - 1; i >= 0; --i) keyUp(keys[i]);
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
