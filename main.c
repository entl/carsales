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
void writePurchase(short numberOfCars, float totalPrice, float discountValue, char customerName[100], short customerAge);

/*Variables*/
unsigned short carsAvailable = CARS_MAX, carsNeeded = 0, carsSold = 0;

/*tracks sale number*/
typedef struct {
    char name[100];
    unsigned short age;
    short numberOfCars;
    float totalPrice;
    short discountPercentage;
    float discountValue;
    bool giveDiscount;
    bool isMember;
} Client;

int numberOfSales = 0;

/*Car models*/
char carModels[][100] = {
    "Toyota Corolla", "Toyota RAV4", "Honda CR-V",
    "Ford F-150", "Honda Civic", "Mercedes C Class",
    "Aston Martin Vantage", "Ferrari SF90", "Nissan GTR"};
float carPrices[] = {
    25000.0f, 30000.0f, 32500.0f,
    37800.0f, 24000.0f, 39000.0f,
    63777.0f, 110000.0f, 100000.0f};
short carsStock[] = {
    20, 15, 16,
    13, 19, 10,
    7, 4, 5};

void main() {
    while (true)
        menu();
}

void menu() {
    // system("cls"); // clears terminal
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
    Client client;
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

    client.totalPrice = carsNeeded * CAR_PRICE;

    /*get customer name*/
    puts("What is your name? Name: ");
    fflush(stdin);
    /*use %[^\n]s to capture whole name (more than 1 word)*/
    scanf("%[^\n]s", &client.name);
    puts("How old are you? Age: ");
    fflush(stdin);
    scanf("%hd", &client.age); // needs validation

    puts("Are you a member of the Car Club?(y/n)");
    fflush(stdin);
    client.isMember = getchar();

    // checks if client have both discounts
    if (tolower(client.isMember) == 'y' && (carsNeeded >= DISCOUNT_MULTIBUY_AMOUNT)) {
        client.giveDiscount = 1;
        /*calculate discount value*/
        client.discountValue = client.totalPrice - (client.totalPrice * (1 - DISCOUNT_MAX));
        /*update total price*/
        client.totalPrice -= client.discountValue;
        client.discountPercentage = DISCOUNT_MAX;
    }

    // otherwise, check member discount
    else if (tolower(client.isMember) == 'y') {
        client.giveDiscount = 1;
        client.discountValue = client.totalPrice - (client.totalPrice * (1 - DISCOUNT_MEMBER_PERCENTAGE));
        client.totalPrice -= client.discountValue;
        client.discountPercentage = DISCOUNT_MEMBER_PERCENTAGE;
    }

    // otherwise, check multibuy discount
    else if (carsNeeded >= DISCOUNT_MULTIBUY_AMOUNT) {
        client.giveDiscount = 1;
        client.discountValue = client.totalPrice - (client.totalPrice * (1 - DISCOUNT_MULTIBUY_PERCENTAGE));
        client.totalPrice -= client.discountValue;
        client.discountPercentage = DISCOUNT_MULTIBUY_PERCENTAGE;
    }

    /* Present discount outcome */
    switch (client.giveDiscount) {
    case true:
        // convert from 0.2 format to 20% format
        client.discountPercentage *= 100;
        // hide decimals, not needed
        printf("\nDiscount of %.0f%% applied!\n", client.discountPercentage);
        break;
    case false:
        printf("\nDiscount not applied.\n");
        break;
    }

    /* Present final outcome */
    printf("\nThank you for your custom.\n");
    printf("\nYou have bought %hd cars. Total price to pay is %.2f.",
           carsNeeded, client.totalPrice);

    writePurchase(carsNeeded, client.totalPrice, client.discountValue, client.name, client.age);

    /*Update stock*/
    carsSold += carsNeeded;
    carsAvailable -= carsNeeded;

    printf("\nPress any key to continue...\n");
    fflush(stdin);
    getchar();
}

void viewSales() {
    system("cls");
    printf("*Sale information*\n\n");
    printf("Cars\tTotal Price\tDiscount Value\tName\nAge\n");
    /*
    
    implement reading from file
    
    */
    printf("\nPress any key to continue...\n");
    fflush(stdin);
    getchar();
    return;
}

void writePurchase(short numberOfCars, float totalPrice, float discountValue, char customerName[100], short customerAge) {
    FILE *fpt;

    fpt = fopen("salesData.csv", "w+");
    fprintf(fpt, "%hd, %f, %f, %s, %hd\n", numberOfCars, totalPrice, discountValue, customerName, customerAge);
}

void writeCarsData(){
    
}


void readFile() {
    FILE *fpt;

    fpt = fopen("", "r");
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