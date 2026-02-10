#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

long long	current_unixtime_ms(void)
{
	struct timeval	tp;

	gettimeofday(&tp, NULL);
	return ((long long)tp.tv_sec * 1000 + (long long)tp.tv_usec / 1000);
}

//スレッド200でやったらどれくらい誤差が出るか調査する。
void	wait_until_2(long long start, long long target)
{
	long long	now;
	long long	remain;
	
	while (1)
	{
		now = current_unixtime_ms();
		remain = target - (now - start);
		if (remain <= 0)
			break ;
		if (remain > 2)
			usleep(1000);
		else
			usleep(500);
	}
}

void	wait_until_1(long long start, long long target)
{
	while (current_unixtime_ms() - start <= target)
		usleep(1000);//1msの誤差が生じる。
}


int	main(int argc, char **argv)
{
	long long	start;

	start = current_unixtime_ms();
	wait_until_1(start, atoi(argv[1]));
	printf("%lld\n", current_unixtime_ms() - start);
}
