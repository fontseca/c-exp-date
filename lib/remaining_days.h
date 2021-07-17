/*
  Jeremy Fonseca | 16.07.21 | ExpirationDate | remaining_days.h
  Source: https://github.com/jeremy06-1/ExpirationDate.git
*/

// Function prototypes
int isLeapYear(int), getRemainingDays(struct Product product), getMonthDays(int, int);

// Calculates a leap year
int isLeapYear(int year) {
	if ((year % 400 == 0) || \
		(year % 4 == 0 && year % 100 != 0)) {
		return 1;
	} else {
		return 0;
	}
}

// Calculates the rigth number of days in every month
int getMonthDays(int currentMonth, int currentYear) {
	switch (currentMonth) {
		case 1: case 3: case 5: case 7: case 8: case 10: case 12: return 31; break;
		case 4: case 6: case 9: case 11: return 30; break;
		case 2:
			if (isLeapYear(currentYear)) {
				return 29;
			} else {
				return 28;
			}
		break;
	}
	return 0;
}

int getRemainingDays(struct Product product) {
	int remainingDays = 0, currentYear, currentMonth, currentDay;
	time_t now;
	struct tm *currentTime;
	
	now = time(NULL);
	currentTime = localtime(&now);
	currentDay = (*currentTime).tm_mday;
	currentMonth = (*currentTime).tm_mon + 1;
	currentYear = (*currentTime).tm_year + 1900;
	
	while (1) {
		if ((currentYear == product.productExpirationDate.year) && \
			currentMonth == product.productExpirationDate.month) {
			break;
		}
		
		remainingDays += getMonthDays(currentMonth, currentYear) - currentDay;
		
		if (currentMonth == 12) {
			currentMonth = 1;
			currentYear++;
		} else {
			currentMonth++;
		}
		currentDay = 0;
	}
	
	remainingDays += product.productExpirationDate.day - currentDay;
	return remainingDays;
}
