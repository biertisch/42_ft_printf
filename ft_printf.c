/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:47:26 by beatde-a          #+#    #+#             */
/*   Updated: 2025/04/30 12:17:19 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ft_printf.h"
#include <limits.h>
#include <stdio.h>

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	update_len(int res, int *len)
{
	if (res < 0)
		return (-1);
	*len += res;
	return (*len);
}

int	convert(va_list args, char type)
{
	if (type == '%')
		return (print_char('%'));
	if (type == 'c')
		return (print_char(va_arg(args, int)));
	if (type == 's')
		return (print_str(va_arg(args, char *)));
	if (type == 'd' || type == 'i')
		return (print_int(va_arg(args, int)));
	if (type == 'u')
		return (print_uint(va_arg(args, unsigned int)));
	if (type == 'x' || type == 'X')
		return (print_hex(va_arg(args, unsigned int), type));
	if (type == 'p')
		return (print_ptr(va_arg(args, void *)));
	return (ft_printf("%%%c", type));
}

int	lone_percent(const char *format)
{
	int	i;

	i = 0;
	while (format[i])
	{
		if (format[i] == '%' && format [i - 1] != '%'
			&& !format[i + 1])
			return (1);
		i++;
	}
	return (0);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		len;
	int		res;
	int		i;

	if (!format)
		return (-1);
	len = 0;
	i = 0;
	va_start(args, format);
	if (lone_percent(format))
		return (-1);
	while (format[i])
	{
		if (format[i] == '%')
			res = convert(args, format[++i]);
		else
			res = print_char(format[i]);
		if (update_len(res, &len) < 0)
			return (-1);
		i++;
	}
	va_end(args);
	return (len);
}

void print_test(const char *desc, int ft_ret, int og_ret)
{
    printf("TEST: %s\n", desc);
    printf("Return ft: %d | og: %d\n", ft_ret, og_ret);
    printf("--------------------------------------------------\n");
}

int main(void)
{
	int ft_ret, og_ret;
	char *null_str = NULL;
	void *null_ptr = NULL;

	// NO PLACEHOLDER
	ft_ret = ft_printf("ft: Hello, world!\n");
	og_ret = printf("og: Hello, world!\n");
	print_test("No placeholder", ft_ret, og_ret);

	// %%
	ft_ret = ft_printf("ft: 100%%\n");
	og_ret = printf("og: 100%%\n");
	print_test("Literal percent %%", ft_ret, og_ret);

	// CHAR
	ft_ret = ft_printf("ft: %c\n", 'A');
	og_ret = printf("og: %c\n", 'A');
	print_test("Char: ASCII 'A'", ft_ret, og_ret);

	ft_ret = ft_printf("ft: %c\n", 200);
	og_ret = printf("og: %c\n", 200);
	print_test("Char: extended ASCII 200", ft_ret, og_ret);

	// STRING
	ft_ret = ft_printf("ft: %s\n", "str");
	og_ret = printf("og: %s\n", "str");
	print_test("String: Normal", ft_ret, og_ret);

	ft_ret = ft_printf("ft: %s\n", "");
	og_ret = printf("og: %s\n", "");
	print_test("String: empty", ft_ret, og_ret);

	ft_ret = ft_printf("ft: %s\n", null_str);
	og_ret = printf("og: %s\n", null_str);
	print_test("String: NULL", ft_ret, og_ret);

	// POINTER
	ft_ret = ft_printf("ft: %p\n", (void *)"ptr");
	og_ret = printf("og: %p\n", (void *)"ptr");
	print_test("Pointer: Non-null", ft_ret, og_ret);

	ft_ret = ft_printf("ft: %p\n", null_ptr);
	og_ret = printf("og: %p\n", null_ptr);
	print_test("Pointer: NULL", ft_ret, og_ret);

	// DECIMAL
	ft_ret = ft_printf("ft: %d %d %d\n", 0, 42, -42);
	og_ret = printf("og: %d %d %d\n", 0, 42, -42);
	print_test("Signed int (%%d): 0, +42, -42", ft_ret, og_ret);

	ft_ret = ft_printf("ft: %i %i %i\n", 0, INT_MAX, INT_MIN);
	og_ret = printf("og: %i %i %i\n", 0, INT_MAX, INT_MIN);
	print_test("Signed int (%%i): 0, INT_MAX, INT_MIN", ft_ret, og_ret);

	// UNSIGNED
	ft_ret = ft_printf("ft: %u\n", UINT_MAX);
	og_ret = printf("og: %u\n", UINT_MAX);
	print_test("Unsigned: UINT_MAX", ft_ret, og_ret);

	ft_ret = ft_printf("ft: %u\n", -1);
	og_ret = printf("og: %u\n", -1);
	print_test("Unsigned: -1 (wraparound)", ft_ret, og_ret);

	// HEX
	ft_ret = ft_printf("ft: %x %X\n", 255, 255);
	og_ret = printf("og: %x %X\n", 255, 255);
	print_test("Hex (lower and upper): 255", ft_ret, og_ret);

	ft_ret = ft_printf("ft: %x %X\n", 0, 0);
	og_ret = printf("og: %x %X\n", 0, 0);
	print_test("Hex (lower and upper): 0", ft_ret, og_ret);

	// MIXED
	ft_ret = ft_printf("ft: %c %s %d %u %x %p %%\n",
		'c', "str", -1, 42949, 255, (void *)"ptr");
	og_ret = printf("og: %c %s %d %u %x %p %%\n",
		'c', "str", -1, 42949, 255, (void *)"ptr");
	print_test("Mixed", ft_ret, og_ret);

	// INVALID CASES
	ft_ret = ft_printf("ft: %r\n");
	// og_ret = printf("og: %r\n"); // cc without -Werror
	print_test("Invalid placeholder (%%r)", ft_ret, og_ret);

	ft_ret = ft_printf("ft: %d %d\n", 42);
	// og_ret = printf("og: %d %d\n", 42); // cc without -Werror
	print_test("Too few arguments", ft_ret, og_ret);

	ft_ret = ft_printf("ft: hello %");
	// og_ret = printf("og: hello %"); // cc without -Werror
	print_test("Format ending with one %%", ft_ret, og_ret);

	ft_ret = ft_printf(NULL);
	// og_ret = printf(NULL); // cc without -Werror
	print_test("Format: NULL", ft_ret, og_ret);

	return (0);
}
