#include <stdio.h>
#include <stdbool.h>

bool	is_num_char(const char c)
{
	return (c >= '0' && c <= '9');
}

bool	safe(const char *s, int is_neg)
{
	static char	*int_limit[] = {"2147483647", "2147483648"};
	int			i;
	char		diff;

	i = 0;
	while (int_limit[is_neg][i])
	{
		diff = s[i] - int_limit[is_neg][i];
		if (diff < 0)
			return (true);
		if (diff > 0)
			return (false);
		i++;
	}
	return (true);
}

bool	is_int_str(const char *s)
{
	int		i;
	int		is_neg;

	if (!s || s[0] == '\0')
		return (false);
	if (s[0] == '0' && s[1] != '\0')
		return (false);
	if (s[0] == '-' && (s[1] == '0' || s[1] == '\0'))
		return (false);
	is_neg = 0;
	if (s[0] == '-')
	{
		s = s + 1;
		is_neg = 1;
	}
	i = 0;
	while (s[i])
		if (is_num_char(s[i++]) == false)
			return (false);
	if (i > 10)
		return (false);
	if (i == 10 && safe(s, is_neg) == false)
		return (false);
	return (true);
}


int	main(void)
{
	char	*arr[] = {"0", "0", "-1", "2147483647", "2147483648", "-2147483648", "-2147483649", "--1", "-0", "0", "00", "01", "12a", "",
		"a", "123456", "-342164", "+123", "001", "0000", "0010", "1", "11111111111111111111", "1111111111", "11111111111", "-123", "-01", NULL};

	for (int i = 0; arr[i]; i++)
		printf("%s (%s)\n", arr[i], is_int_str(arr[i]) ? "OK" : "NG");
	return (0);
}
