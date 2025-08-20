#ifndef CAUTOGUI_H
#define CAUTOGUI_H

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <stdbool.h>

struct	Point
{
	int	x;
	int	y;
};

struct	Box
{
	int	x;
	int	y;
	int	width;
	int	height;
};

extern Display	*display;

void	init(void);
bool	onScreen(int x, int y);

struct Point	position(void);
struct Point	center(const struct Box* box);
struct Point	size(void);

void	press(const char* key, int presses);
void	write(const char* text);
void	moveTo(int x, int y);
void	move(int dx, int dy);
void	drag(int dx, int dy);
void	click(void);
void	doubleClick(void);
void	scroll(int amount);
void	keyDown(const char* key);
void	keyUp(const char* key);
void	hotkey(const char** keys, int num_keys);
void	help(void);

#endif
