/*
  Jeremy Fonseca | 16.07.21 | ExpirationDate
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#if defined(_WIN32) || defined(_WIN64)
void clearScreen(void) {
	system("cls");
}
#elif defined(__linux__)
void clearScreen(void) {
	system("clear");
}
#endif

// Structures
typedef struct {
	int day, month, year;
} ExpirationDate;

struct Product {
	char productName[15];
	int productAmount;
	ExpirationDate productExpirationDate;
};

// Function prototype declarations
void readProductData(struct Product *infoProd), printProducts(struct Product);
int expirationTime(struct Product prod);

int main(void) {
	short int productsNumber, counter;
	int currentDay, currentMonth, currentYear;
	time_t now;
	struct tm *currentTime;
	
	now = time(NULL);
	currentTime = localtime(&now);
	
	currentDay = currentTime->tm_mday;
	currentMonth = currentTime->tm_mon + 1;
	currentYear = currentTime->tm_year + 1900;
	
	do {
		printf("Insert products number: " );
		scanf("%hi", &productsNumber);
		if (productsNumber < 0) printf("Error: Negative number.\n");
	} while (productsNumber < 0);

	// Create a vector of structures
	struct Product productsVector[productsNumber];
	
	// Get data
	for (counter = 0; counter < productsNumber; ++counter) {
		clearScreen();
		printf("P[%hi]\n", (counter + 1));
		readProductData(&productsVector[counter]);
	}
	
	// Print data
	clearScreen();
	
	printf("[CURRENT DATE: %i/%i/%i]\n", currentDay, currentMonth, currentYear);
	puts("~~~~~~~~~~~~   ~~~~~~~~~~~~~~~     ~~~~~~~~~~~~~~");
	puts("PRODUCT NAME   EXPIRATION DATE     RAMAINING DAYS");
	puts("~~~~~~~~~~~~   ~~~~~~~~~~~~~~~     ~~~~~~~~~~~~~~");
	for (counter = 0; counter < productsNumber; ++counter) {
		printProducts(productsVector[counter]);
	}
	
	/*printf("El producto se vence dentro de %d días",caduca);*/
	
	return 0;
}

void readProductData(struct Product *infoProd) {
	
	printf("Product name: ");
	scanf("%s", (*infoProd).productName);
	
	do {
		printf("Product amount: ");
		scanf("%d", &(*infoProd).productAmount);
		if ((*infoProd).productAmount < 0) printf("Error: Negative number.\n");
	} while((*infoProd).productAmount < 0);
	
	do {
		printf("Expiration date (dd mm yyyy): ");
		scanf("%d %d %d", &(*infoProd).productExpirationDate.day, &(*infoProd).productExpirationDate.month, &(*infoProd).productExpirationDate.year);
		
		if ((*infoProd).productExpirationDate.day < 0 ||
			(*infoProd).productExpirationDate.month < 0 ||
			(*infoProd).productExpirationDate.month > 12 ||
			(*infoProd).productExpirationDate.year < 0) {
			printf("Error: Invalid date.\n");
		}
		
	} while ((*infoProd).productExpirationDate.day < 0 ||
		(*infoProd).productExpirationDate.month < 0 ||
		(*infoProd).productExpirationDate.month > 12 ||
		(*infoProd).productExpirationDate.year < 0);
	return;
}

void printProducts(struct Product product) {
	printf("%-15s%i/%i/%-16i%i\n",
		   product.productName,
		   product.productExpirationDate.day,
		   product.productExpirationDate.month,
		   product.productExpirationDate.year,
		   10);
}

