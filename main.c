/*
  Jeremy Fonseca | 16.07.21 | ExpirationDate
  Source: https://github.com/jeremy06-1/ExpirationDate.git
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

#include "lib/remaining_days.h"

// Function prototype declarations
void readProductData(struct Product *), printProducts(struct Product);

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
		printf("CURRENT DATE: %i/%i/%i\n", currentDay, currentMonth, currentYear);
		printf("Product [%hi]\n", (counter + 1));
		readProductData(&productsVector[counter]);
	}
	
	// Print data
	clearScreen();
	printf("CURRENT DATE: %i/%i/%i\n", currentDay, currentMonth, currentYear);
	puts("~~~~~~~~~~~~   ~~~~~~~~~~~~~~~     ~~~~~~~~~~~~~~");
	puts("PRODUCT NAME   EXPIRATION DATE     RAMAINING DAYS");
	puts("~~~~~~~~~~~~   ~~~~~~~~~~~~~~~     ~~~~~~~~~~~~~~");
	for (counter = 0; counter < productsNumber; ++counter) {
		printProducts(productsVector[counter]);
	}
	
	return 0;
}

void readProductData(struct Product *infoProd) {
	printf("Name: ");
	scanf("%s", (*infoProd).productName);
	
	do {
		printf("Amount: ");
		scanf("%d", &(*infoProd).productAmount);
		if ((*infoProd).productAmount < 0) printf("Error: Negative number.\n");
	} while((*infoProd).productAmount < 0);
	
	do {
		printf("Expiration Date (dd mm yyyy): ");
		scanf("%d %d %d", \
		&(*infoProd).productExpirationDate.day, \
		&(*infoProd).productExpirationDate.month, \
		&(*infoProd).productExpirationDate.year);
		
		if ((*infoProd).productExpirationDate.day < 0 || \
			((*infoProd).productExpirationDate.day > getMonthDays((*infoProd).productExpirationDate.month, \
			(*infoProd).productExpirationDate.year)) || \
			(*infoProd).productExpirationDate.month < 0 || \
			(*infoProd).productExpirationDate.month > 12 || \
			(*infoProd).productExpirationDate.year < 1900) {
			printf("Error: Invalid date.\n");
		}
		
	} while ((*infoProd).productExpirationDate.day < 0 || \
		(*infoProd).productExpirationDate.month < 0 || \
		((*infoProd).productExpirationDate.day > getMonthDays((*infoProd).productExpirationDate.month, \
		(*infoProd).productExpirationDate.year)) || \
		(*infoProd).productExpirationDate.month > 12 || \
		(*infoProd).productExpirationDate.year < 1900);
	return;
}

void printProducts(struct Product product) {
	char *productName;
	int currentMonth, currentYear, currentDay, expirationDay, expirationMonth, expirationYear;
	time_t now;
	struct tm *currentTime;
	
	now = time(NULL);
	currentTime = localtime(&now);
	
	currentDay = (*currentTime).tm_mday;
	currentMonth = (*currentTime).tm_mon + 1;
	currentYear = (*currentTime).tm_year + 1900;
	
	productName = product.productName;
	expirationDay = product.productExpirationDate.day;
	expirationMonth = product.productExpirationDate.month;
	expirationYear = product.productExpirationDate.year;
	
	if ((expirationYear < currentYear) || \
		((currentDay > expirationDay) && \
		(currentMonth == expirationMonth && \
		(currentYear == expirationYear))) || \
		((expirationMonth < currentMonth) && \
		(expirationYear == currentYear)) ) {
		printf("%-15s%i/%i/%-16i%s\n", \
		productName, \
		expirationDay, \
		expirationMonth, \
		expirationYear, \
		"EXPIRED");
	} else {
		printf("%-15s%i/%i/%-16i%i\n", \
		productName, \
		expirationDay, \
		expirationMonth, \
		expirationYear, \
		getRemainingDays(product));
	}
	return;
}
