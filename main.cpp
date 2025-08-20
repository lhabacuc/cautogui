#include "cautogui.hpp"
#include <iostream>

int	main()
{
	cautogui::init();
	auto pos = cautogui::position();
	std::cout << "Posição: " << pos.first << ", " << pos.second << std::endl;
	cautogui::moveTo(100, 100);
	cautogui::click();
	return 0;
}
