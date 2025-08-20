#include "cautogui.h"
#include <stdio.h>

int	main()
{
	init();
	struct Point pos = position();
	printf("Posição: %d, %d\n", pos.x, pos.y);
	move(50, 50);
	write("liedson");
	click();
	return 0;
}
