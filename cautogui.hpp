#ifndef CAUTOGUI_HPP
#define CAUTOGUI_HPP

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <utility>
#include <string>
#include <vector>

namespace cautogui
{
	extern Display	*display;
	void	init();
	bool	onScreen(int x, int y); 
	std::pair<int, int>	position();
	void	press(const std::string& key, int presses = 1);
	void	write(const std::string& text);
	std::pair<int, int>	center(const std::vector<int>& box);
	void	moveTo(int x, int y);
	std::pair<int, int>	size();
	void	move(int dx, int dy);
	void	drag(int dx, int dy);
	void	click();
	void	doubleClick();
	void	scroll(int amount);
	void	keyDown(const std::string& key);
	void	keyUp(const std::string& key);
	void	hotkey(const std::vector<std::string>& keys);
	void	help();
}

#endif
