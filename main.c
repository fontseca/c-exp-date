/*
  Jeremy Fonseca | 16.07.21 | ExpirationDate
  Source: https://github.com/jeremy06-1/ExpirationDate.git
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iso646.h>
#include <inttypes.h>
#include <getopt.h>
#include <string.h>
#include <stdarg.h>

/* The name of the program.  */
#define PROGNAME "expdate"

/* Curren time as time since epoch.  */
static time_t now;

/* Abstract a product.  */
struct product
{
	/* The name of the product.  */
  char name[0x64];

	/* The available quantity of products.  */
  uint32_t quantity;

	/* The price of the product.  */
  double price;

	/* The time (represented as time since epoch)
	   when the product will expire.  */
  time_t expires_at;
};

/* Sends an error message to stderr and exits.  Do not include
   end-of-line character; it is inserted automatically.   */
static void
fatal(char const *const format, ...)
{
	char buffer[0x256];
	va_list ap;
	va_start(ap, format);
	vsprintf(buffer, format, ap);
	va_end(ap);
	fprintf(stderr, "\x1B[1;71m%s:\x1B[0m \x1B[1;31mfatal:\033[0m %s\n",
						PROGNAME, buffer);
	exit(EXIT_FAILURE);
}

/* Sends an warning message to stderr.  Do not include
   end-of-line character; it is inserted automatically.   */
static void
warning(char const *const format, ...)
{
	char buffer[0x256];
	va_list ap;
	va_start(ap, format);
	vsprintf(buffer, format, ap);
	va_end(ap);
	fprintf(stderr, "\x1B[1;71m%s:\x1B[0m \x1B[1;35mwarning:\033[0m %s\n",
						PROGNAME, buffer);
}

/* Prints usage message.  */
static void
usage(uint32_t const status)
{
  if (EXIT_SUCCESS xor status)
  {
    fprintf(stderr, "Try '%s --help' for more information.\n", PROGNAME);
  }
  else
  {
    printf("\
Usage: %s N\n", PROGNAME);

		fputs("\
Calculates the expiration date of a sequence of N products\n\
inserted by the user.\n\
\n", stdout);

    fputs("\
  -h, --help                   display this information\n\
  -v, --version                display current version\n", stdout);
  }

  exit(status);
}

/* Clears (``flushes'') the stadard input's buffer.  */
static void
clear_stdin(void)
{
	char c = fgetc(stdin);
  while (c not_eq 10)
	{
  	c = fgetc(stdin);
	}
}

/* Displays a single product to stdout.  */
static void
display_single_product(struct product *const p)
{
	/* We need the actual current time.  */

	now = time(NULL);

	/* Formatted expiration date.  */
	char expdate[0x64];

	strftime(expdate, sizeof expdate, "%c", localtime(&p->expires_at));

	/* Holds a string of how the product will be displayed.  */
	char const *const format = p->expires_at < now ? "%-15s%-30s%s\n" : "%-15s%-30s%lf\n";

	p->expires_at < now
		? printf(format, p->name, expdate, "EXPIRED")
		: printf(format, p->name, expdate, difftime(p->expires_at, now) / (60 * 60 * 24));
}

/* Reads a product pointed by `p'.  */
static void
read_single_product(struct product *const p)
{
	/* Ask for product's name.  */

	printf("Product name: ");
	fgets(p->name, sizeof p->name, stdin);

	/* We do not want the end-of-line character,
	   so put instead a end-of-string one.  */

	p->name[strcspn(p->name, "\n")] = '\0';

	/* Ask for the quantity of products.  */

	do
	{
		printf("Quantity: ");
		scanf("%d", &p->quantity);

		/* Wrong quantity?  */

		if (0 >= p->quantity)
		{
			warning("Quantity is an invalid number or could not be parsed correctly. Please try again.");
			clear_stdin();
		}
	}
	while(0 >= p->quantity);

	/* Ask for product's price.  */

	do
	{
		printf("Price: ");
		scanf("%lf", &p->price);

		/* Wrong price?  */

		if (0 >= p->price)
		{
			warning("Price is an invalid number or could not be parsed correctly. Please try again.");
			clear_stdin();
		}
	}
	while(0 >= p->price);


	/* Number of entities parsed by scanf.  */
	int32_t n_parsed;

	/* Ask for product's expiration date.  */

	do
	{
		puts("Expiration date.");
		printf("(Use ISO 8601 date format `YYYY-MM-DDThh:mm:ss'. E.g., 2023-01-31T15:50:15): ");

		int32_t y, M, d, h, m, s;

		/* Try to parse our date.  */

		n_parsed = scanf("%4d-%2d-%2dT%2d:%2d:%dZ", &y, &M, &d, &h, &m, &s);

		/* Not what we expect.  */

		if (6 not_eq n_parsed)
		{
			warning("The date inserted could not be parsed correctly. Please try again.");
			clear_stdin();
			continue;
		}

		/* The time the prodcut will expire represented as a
		   broken-down time structure.  */
		struct tm expires_on =
		{
			.tm_year = y - 1900,
			.tm_mon = M - 1,
			.tm_mday = d,
			.tm_hour = h,
			.tm_min = m,
			.tm_sec = s
		};

		/* Store time since epoch in current product.  */

		p->expires_at = mktime(&expires_on);
	}
	while (6 not_eq n_parsed);

	clear_stdin();
}

/* Displays `n' products of `products'.  */
static void
display_products(struct product *const products, size_t const n)
{
	printf("\nCURRENT DATE: %s", ctime(&now));

	puts("~~~~~~~~~~~~   ~~~~~~~~~~~~~~~~~~~~~~~~~     ~~~~~~~~~~~~~~");
	puts("PRODUCT NAME   EXPIRATION DATE               RAMAINING DAYS");
	puts("~~~~~~~~~~~~   ~~~~~~~~~~~~~~~~~~~~~~~~~     ~~~~~~~~~~~~~~");

	uint32_t i = 0;

	for (; i < n; ++i)
	{
		display_single_product(&products[i]);
	}
}

/* Reads `n' products into `products'.  */
static void
read_products(struct product *const products, size_t const n)
{
	int32_t i = 0;

	for (; i < n; ++i)
	{
		printf("Local time is %s\n", ctime(&now));
		printf("Product #%d\n", 1 + i);
		puts("----------");
		read_single_product(&products[i]);
		puts("----------");
	}
}

int32_t
main(int32_t argc, char *argv[])
{
	/* We need the number of products to read.  */

	if (argc < 2)
	{
		usage(EXIT_SUCCESS);  /* need some help?  */
	}

	/* The number of products to ask for.  */
	uint32_t const n_products = atoi(argv[1]);

	if (0 == n_products)
	{
		fatal("The number of products inserted was 0 or could not parse insertion.");
	}
	else if (0 > n_products)
	{
		fatal("Invalid number of products.");
	}
 
	/* Collection of products.  */
	struct product *const products = (struct product *)malloc(sizeof(struct product[n_products]));

  /* Start up time.  */	
	now = time(NULL);

	/* Ask for products to user.  */

	read_products(products, n_products);

	/* Display products.  */

	display_products(products, n_products);

	free(products);
	
	return EXIT_SUCCESS;
}
