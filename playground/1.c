#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdlib.h>

int	main(void)
{
	struct timeval	tp;

	gettimeofday(&tp, NULL);
	printf("%ld\n", tp.tv_sec);
	printf("%lld[ms]\n", ((long long)tp.tv_sec * 1000000LL + (long long)tp.tv_usec) / 1000LL);
	usleep(1000000);
	gettimeofday(&tp, NULL);
	printf("%lld[ms]\n", ((long long)tp.tv_sec * 1000000LL + (long long)tp.tv_usec) / 1000LL);	
	return (0);
}
//philosopherを１日稼働させる場合はt[ms]はint型で足りる。
//24日以上稼働させる場合はintよりも大きい型が必要になる。
//計算最中は、32bitじゃ足りないので、64bitの型で計算を行う。longは環境によって32bitか64bitか変わるので、long long(64bit)で計算を行う。
