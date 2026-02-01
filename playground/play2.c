#include <stdio.h>

int	rem(int a, int p)
{
	int	r;

	if (p == 0)
		return (0);
	if (p < 0)
		p = -p;
	if (a >= 0)
		return (a % p);
	else
		return (a - p * (a / p - 1));
}

int	next_turn(int turn)
{
	return (rem(turn, 3) + 1);
}

int	main(void)
{
	printf("%d\n", next_turn(1));
	printf("%d\n", next_turn(2));
	printf("%d\n", next_turn(3));
	return (0);
}
