#include "cautogui.h"
#include <stdio.h>

int	main()
{
	init();
	struct Point pos = position();
	printf("Posição: %d, %d\n", pos.x, pos.y);
	moveTo(100, 100);
	click();
	scroll(5);
	return 0;
}
