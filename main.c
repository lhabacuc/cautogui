#include "cautogui.h"
#include <stdio.h>

int	main()
{
	init();
	struct Point pos = position();
	printf("Posição: %d, %d\n", pos.x, pos.y);
	move(10, 10);
	click();
	return 0;
}
