#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* Constants */
#define CAR_PRICE 20500.0f
#define CARS_MAX 10
#define DISCOUNT_MULTIBUY_PERCENTAGE 0.20f
#define DISCOUNT_MULTIBUY_AMOUNT 3
#define DISCOUNT_MEMBER_PERCENTAGE 0.25f
#define DISCOUNT_MAX 0.35f

/*funtions prototypes*/
void menu();
void buyCar();
void viewSales();
void writeFile(short numberOfCars, float totalPrice, float discountValue, char customerName[100], short customerAge);

/*Variables*/
unsigned short carsAvailable = CARS_MAX, carsNeeded = 0, carsSold = 0;
char isMember = false;
bool giveDiscount = false;
/*keeping values during the purchase and then appends to salesInfo array*/
float totalPrice, discountValue, discountPercentage;
/*2d array which tracks sales info such as
cars bought, totalPrice, discountValue*/

// float salesInfo[CARS_MAX][3];

/*stores customer*/
char customerNames[CARS_MAX][200];
short customerAge[CARS_MAX];
/*tracks sale number*/
int numberOfSales = 0;

/*Car models*/
char carModels[][100] = {
    "Toyota Corolla", "Toyota RAV4", "Honda CR-V", 
    "Ford F-150", "Honda Civic", "Mercedes C Class", 
    "Aston Martin Vantage", "Ferrari SF90", "Nissan GTR"
};
float carPrices[] = {
    25000.0f, 30000.0f, 32500.0f,
    37800.0f, 24000.0f, 39000.0f,
    63777.0f, 110000.0f, 100000.0f
};
short carsStock[] = {
    20, 15, 16,
    13, 19, 10,
    7, 4, 5
};

void main() {
    while (true)
        menu();
}

void menu() {
    system("cls"); // clears terminal
    /* Greeting */
    puts("\n\n***Welcome to the Car Sales office!***\n");
    printf("a. Buy cars\n"
           "b. View Sales Stats\n"
           "x. Exit\n"
           "Please choose one: ");

    /*clears buffer*/
    fflush(stdin);
    char switchChoice = getchar();
    switch (tolower(switchChoice)) {
    case 'a':
        buyCar();
        break;
    case 'b':
        viewSales();
        break;
    case 'x':
        puts("\n*Thanks for using our service*");
        exit(0);
        break;
    default:
        puts("Make sure your choice is correct");
        break;
    }
}

void buyCar() {
    if (carsAvailable > 0) {
        printf("\nWe have %hd cars available. Each car costs %.2f.\n\n",
               carsAvailable, CAR_PRICE);
    } else {
        printf("Sorry, there are no more cars available.");
        printf("\nPress any key to continue...\n");
        fflush(stdin);
        getchar();
        return;
    }
    /*ask cars needed*/
    puts("How many cars would you like to buy? Amount: ");
    scanf("%hd", &carsNeeded);

    /*check stock*/
    if (carsAvailable < carsNeeded) {
        printf("Sorry, there are fewer cars available than you require.\n");
        printf("\nPress any key to continue...\n");
        fflush(stdin);
        getchar();
        return;
    }
    /*Checks if input makes sense*/
    if (carsNeeded == 0) {
        printf("\n*Make sure you entered a correct number*\n");
        printf("\nPress any key to continue...\n");
        fflush(stdin);
        getchar();
        return;
    }

    totalPrice = carsNeeded * CAR_PRICE;

    /*get customer name*/
    puts("What is your name? Name: ");
    fflush(stdin);
    /*use %[^\n]s to capture whole name (more than 1 word)*/
    scanf("%[^\n]s", &customerNames[numberOfSales]);
    puts("How old are you? Age: ");
    fflush(stdin);
    scanf("%hd", &customerAge[numberOfSales]); //needs validation

    puts("Are you a member of the Car Club?(y/n)");
    fflush(stdin);
    isMember = getchar();

    // checks if client have both discounts
    if (tolower(isMember) == 'y' && (carsNeeded >= DISCOUNT_MULTIBUY_AMOUNT)) {
        giveDiscount = 1;
        /*calculate discount value*/
        discountValue = totalPrice - (totalPrice * (1 - DISCOUNT_MAX));
        /*update total price*/
        totalPrice -= discountValue;
        discountPercentage = DISCOUNT_MAX;
    }

    // otherwise, check member discount
    else if (tolower(isMember) == 'y') {
        giveDiscount = 1;
        discountValue = totalPrice - (totalPrice * (1 - DISCOUNT_MEMBER_PERCENTAGE));
        totalPrice -= discountValue;
        discountPercentage = DISCOUNT_MEMBER_PERCENTAGE;
    }

    // otherwise, check multibuy discount
    else if (carsNeeded >= DISCOUNT_MULTIBUY_AMOUNT) {
        giveDiscount = 1;
        discountValue = totalPrice - (totalPrice * (1 - DISCOUNT_MULTIBUY_PERCENTAGE));
        totalPrice -= discountValue;
        discountPercentage = DISCOUNT_MULTIBUY_PERCENTAGE;
    }

    /* Present discount outcome */
    switch (giveDiscount) {
    case true:
        // convert from 0.2 format to 20% format
        discountPercentage *= 100;
        // hide decimals, not needed
        printf("\nDiscount of %.0f%% applied!\n", discountPercentage);
        break;
    case false:
        printf("\nDiscount not applied.\n");
        break;
    }

    /* Present final outcome */
    printf("\nThank you for your custom.\n");
    printf("\nYou have bought %hd cars. Total price to pay is %.2f.",
           carsNeeded, totalPrice);

    writeFile(carsNeeded, totalPrice, discountValue, customerNames[numberOfSales], customerAge[numberOfSales]);

    /*Update stock*/
    carsSold += carsNeeded;
    carsAvailable -= carsNeeded;
    /*add info to salesInfo array*/
    // salesInfo[numberOfSales][0] = carsNeeded;
    // salesInfo[numberOfSales][1] = totalPrice;
    // salesInfo[numberOfSales][2] = discountValue;
    numberOfSales++;
    /*update variables to base values*/
    carsNeeded = 0;
    giveDiscount = false;
    isMember = false;
    discountPercentage = 0;
    discountValue = 0;

    printf("\nPress any key to continue...\n");
    fflush(stdin);
    getchar();
}

void viewSales() {
    system("cls");
    printf("*Sale information*\n\n");
    printf("Cars\tTotal Price\tDiscount Value\tName\nAge\n");
    // for (int i = 0; i < sizeof(salesInfo) / sizeof(salesInfo)[0]; i++) {
    //     if (salesInfo[i][0] > 0) {
    //         for (int u = 0; u < sizeof(salesInfo[0]) / sizeof(salesInfo[0][0]); u++) {
    //             printf("%-4.2f\t", salesInfo[i][u]);
    //         }
    //         printf("%s", customerNames[i]);
    //         puts("\n");
    //     }
    // }
    printf("\nPress any key to continue...\n");
    fflush(stdin);
    getchar();
    return;
}

void writeFile(short numberOfCars, float totalPrice, float discountValue, char customerName[100], short customerAge){
    FILE *fpt;

    fpt = fopen("salesData.csv", "w+");
    fprintf(fpt, "%hd, %f, %f, %f, %s, %hd\n", numberOfCars, totalPrice, discountValue, customerName, customerAge);
}

// void bubbleSort() {
//     int arr[] = {2, 3, 1, 7, 5, 2, 9, 0};
//     int length = sizeof(arr) / sizeof(arr[0]);
//     for (int i = 0; i < length; i++) {
//         for (int j = 0; j < length - i - 1; j++) {
//             if (arr[j] < arr[j+1]) {
//                 int temp = arr[j+1];
//                 arr[j+1] = arr[j];
//                 arr[j] = temp;
//             }
//         }
//     }
//     for (int i = 0; i < length; i++) {
//         printf("%d\n", arr[i]);
//     }
//     // printf("%d", length);
// }