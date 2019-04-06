/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcrisp <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/17 10:45:50 by jcrisp            #+#    #+#             */
/*   Updated: 2018/09/20 19:09:30 by jcrisp           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/ft_printf.h"
#include <stdio.h>
#include <limits.h>

void isFlag(char *str_cap, va_list ap, int *count)
{
  // Argument type object ot help with printing all modifications
  t_argu arg;
  int i;
  int m;
  // The next argument to be checked
  char next_a;

  m = 0;
  i = 0;
  printf("\nStr Cap: %s-c %c \n",str_cap, str_cap[i]);

  // Till str_cap finds another % or till end of str_cap
  // Use a string to capture all modifications instead of directly printing.
  // SAMPLE: %5.3d
  while (str_cap[i] != '%' || !str_cap[i])
  {
    // Given a respective flag function
      // Add spacing iterate through a number till not number or '.'
      if (ft_isdigit(str_cap[i])){
          next_a = ft_space(str_cap, i, arg);
          // Move to lastest position in string.
          while (ft_isdigit(str_cap[i]))
            i++;
          // Allocate for modifiable container.
          arg.cont = malloc(arg.space_size + 1);
      }
      if ( str_cap[i] == 's')
        pf_putstr(va_arg(ap, char*), count);
      else if (str_cap[i]== 'c')
        pf_putchar(va_arg(ap, int), count);
      else if (str_cap[i] == 'd' || str_cap[i] == 'i')
        pf_putnbr(va_arg(ap, int), count);
      else if (str_cap[i] == 'p'){
        pf_putstr("0x", count);
        pf_atoib(va_arg(ap, int64_t), 16, count);
      }
      else if (str_cap[i] == 'o')
        pf_atoib(va_arg(ap, int64_t), 8, count);
      else if (str_cap[i] == 'u')
        pf_atoib(va_arg(ap, int64_t), 10, count);
      else if (str_cap[i] == 'x')
        pf_atoib(va_arg(ap, int64_t), 16, count);
      else if (str_cap[i] == 'X')
        pf_atoib_cap(va_arg(ap, int64_t), 16, count);
      else if (str_cap[i] == '.')
        arg.precision = 1;
      else if (str_cap[i] == '*')
        arg.multiple = 1;
      else if (str_cap[i] == '+')
        arg.sign = 1;
      else if (str_cap[i] == '-')
        arg.leftAlign = 1; 
      i++;
  }

}

int b_printf(char* str, ...)
{
	va_list valist;
	int len;
	int count;

	count = 0;
	va_start(valist, str);
	while(*str)
	{
		if (*str == '%')
		{
      str += 1;
      isFlag(str, valist, &count);
      // if ((len = print_stdout(&format, arg)) == PRINTF_FAILURE)
      // break ;
      // count += 1;
		}
		else
    {
			pf_putchar(*str, &count);
      count += 1;
    }
    str += 1;
	}
  va_end(valist);
  return count;
}

int   main(void)
{
  char            *str = "hello";
  char            chr = 'g';
  unsigned int    unum = -444;
  unsigned int    xnum = -500;
  unsigned int    onum = -467;

  printf("\t\t\t---> my   length: %d <---\n", b_printf("%+-d %s %c %x  ~random-words./\'\\ %o %u %p\n", SHRT_MAX, str, chr, xnum, onum, unum, str));
  printf("\t\t\t---> real length: %d <---\n", printf("%.30f %s %c %-#x  ~random-words./\'\\ %#o %u %p\n",22/(7 * 1.0), str, chr, xnum, onum, unum, str));
  printf("\n");
  int x = 120;
  //const char * y = "monkeys";
  printf ("<%d> is not justified.\n", x);

  // printf ("<%5d> is right-justified.\n", x);
  // printf ("<%-5d> The minus sign makes it left-justified.\n", x);
  // printf ("'%s' is not justified.\n", y);
  // printf ("'%10s' is right-justified.\n", y);
  // printf ("'%-10s' is left-justified using a minus sign.\n", y);
  return (0);
}