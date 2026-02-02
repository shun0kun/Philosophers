#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int	rem(int a, int b)
{
	if (b == 0)
		return (0);
	if (b < 0)
		b = -b;
	if (a < 0)
	{
		if (a % b == 0)
			return (0);
		else
			return (a - b * (a / b - 1));
	}
	return (a % b);
}

int	next_turn(int turn, int range)
{
	return (rem(turn + 1, range));
}

int	main(void)
{
	for (int i = -10; i < 10; i++)
		printf("%d -> %d\n", i, next_turn(i, 3));
	return (0);
}
