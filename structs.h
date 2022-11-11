#include <stdbool.h>
/*stuctures*/
typedef struct
{
    char name[100];
    char carModel[100];
    unsigned short age;
    short carsNeeded;
    float totalPrice;
    float discountPercentage;
    float discountValue;
    bool giveDiscount;
    char isMember;
} Client;

typedef struct
{
    char carModel[100];
    float carPrice;
    unsigned short carsAvailable;
    unsigned short carsSold;
} Car;

/*
 * Car models
 * create an array of structs
 */
Car cars[9] = {
    {"Toyota Corolla", 25000.0f, 20, 0},
    {"Toyota RAV4", 30000.0f, 15, 0},
    {"Honda CR-V", 32500.0f, 16, 0},
    {"Ford F-150", 37800.0f, 13, 0},
    {"Honda Civic", 24000.0f, 19, 0},
    {"Mercedes C Class", 39000.0f, 10, 0},
    {"Aston Martin Vantage", 63777.0f, 7, 0},
    {"Ferrari SF90", 110000.0f, 4, 0},
    {"Nissan GTR", 100000.0f, 5, 0},
};

/*number of car models*/
short carOptions = sizeof(cars) / sizeof(cars[0]);
