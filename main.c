#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Constants */
#define DISCOUNT_MULTIBUY_PERCENTAGE 0.20f
#define DISCOUNT_MULTIBUY_AMOUNT 3
#define DISCOUNT_MEMBER_PERCENTAGE 0.25f
#define DISCOUNT_MAX 0.35f

typedef struct
{
    char name[100];
    char carModel[100];
    unsigned short age;
    short carsNeeded;
    float totalPrice;
    float discountPercentage;
    float discountValue;
    short giveDiscount;
    char isMember;
} Client;

typedef struct
{
    char carModel[100];
    float carPrice;
    unsigned short carsAvailable;
    unsigned short carsSold;
} Car;

/*funtions prototypes*/
void menu();
void buyCar();
void viewSales();
void showModels();
short chooseModel();
/**
 * @brief Based on cars needed and membership applies a discount
 *
 * @param client (pointer to struct)
 */
void applyDiscount(Client *client);

/**
 * @brief writes the information about purchase into the file "salesData.csv"
    if it is first run of code, creates file "salesData.cvs"
 *
 * @param carModel
 * @param carsNeeded
 * @param totalPrice
 * @param discountValue
 * @param customerName
 * @param customerAge
 */
void writePurchase(char carModel[100], short carsNeeded, float totalPrice, float discountValue, char customerName[100], short customerAge);
void readPurchases();
void writeCarsSold();
void readCarsSold();
void updateCarsAvailable();

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
short carOptions = sizeof(cars) / sizeof(cars[0]);

void main()
{
    /*gets from file number of cars that were sold*/
    readCarsSold();
    /*update in each class variable "carsAvailable"*/
    updateCarsAvailable();
    while (true)
        menu();
}

void menu()
{
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
    switch (tolower(switchChoice))
    {
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

void buyCar()
{
    /*create an instance of the class*/
    Client client;

    showModels();
    short modelChoice = chooseModel();

    /*ask cars needed*/
    puts("How many cars would you like to buy? Amount: ");
    scanf("%hd", &client.carsNeeded);

    /*check stock*/
    if (cars[modelChoice].carsAvailable < client.carsNeeded)
    {
        printf("Sorry, there are fewer cars available than you require.\n");
        printf("\nPress any key to continue...\n");
        fflush(stdin);
        getchar();
        return;
    }
    /*Checks if input makes sense*/
    if (client.carsNeeded == 0)
    {
        printf("\n*Make sure you entered a correct number*\n");
        printf("\nPress any key to continue...\n");
        fflush(stdin);
        getchar();
        return;
    }

    client.totalPrice = client.carsNeeded * cars[modelChoice].carPrice;

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

    /*call function to check if discount applicable*/
    applyDiscount(&client);

    /* Present discount outcome */
    switch (client.giveDiscount)
    {
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
           client.carsNeeded, client.totalPrice);

    /*Update stock*/
    cars[modelChoice].carsSold += client.carsNeeded;
    cars[modelChoice].carsAvailable -= client.carsNeeded;
    /*update information about how many cars were sold in the file*/
    writeCarsSold();

    /*Write information about sale in the file*/
    writePurchase(cars[modelChoice].carModel, client.carsNeeded, client.totalPrice,
                  client.discountValue, client.name, client.age);

    printf("\nPress any key to continue...\n");
    fflush(stdin);
    getchar();
}

void showModels(){
    puts("\n***In our store there are a variety of cars presented!***");
    /*present all models in the shop*/
    printf("N.\tModel\t\t\tPrice\t\tAvailable\n\n");
    for (int i = 0; i < carOptions; i++)
    {
        printf("%-d.\t%-24s%-.2f\t%-hd\n", i + 1, cars[i].carModel, cars[i].carPrice, cars[i].carsAvailable);
    }
}

short chooseModel()
{
    short carChoice;
    fflush(stdin);
    printf("\nChoose car: ");
    scanf("%hd", &carChoice);
    return carChoice - 1;
}

void applyDiscount(Client *client)
{
    // check if the customer is eligible for both discounts
    if (tolower(client->isMember) == 'y' && (client->carsNeeded >= DISCOUNT_MULTIBUY_AMOUNT))
    {
        client->giveDiscount = 1;
        /*calculate discount value*/
        client->discountValue = client->totalPrice - (client->totalPrice * (1 - DISCOUNT_MAX));
        /*update total price*/
        client->totalPrice -= client->discountValue;
        client->discountPercentage = DISCOUNT_MAX;
    }

    // otherwise, check member discount
    else if (tolower(client->isMember) == 'y')
    {
        client->giveDiscount = 1;
        client->discountValue = client->totalPrice - (client->totalPrice * (1 - DISCOUNT_MEMBER_PERCENTAGE));
        client->totalPrice -= client->discountValue;
        client->discountPercentage = DISCOUNT_MEMBER_PERCENTAGE;
    }

    // otherwise, check multibuy discount
    else if (client->carsNeeded >= DISCOUNT_MULTIBUY_AMOUNT)
    {
        client->giveDiscount = 1;
        client->discountValue = client->totalPrice - (client->totalPrice * (1 - DISCOUNT_MULTIBUY_PERCENTAGE));
        client->totalPrice -= client->discountValue;
        client->discountPercentage = DISCOUNT_MULTIBUY_PERCENTAGE;
    }
}

void viewSales()
{
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

void writePurchase(char carModel[100], short carsNeeded, float totalPrice, float discountValue, char customerName[100], short customerAge)
{
    FILE *fpt;
    if (access("salesData.csv", F_OK) != 0)
    {
        fpt = fopen("salesData.csv", "w+");
        // fprintf(fpt, "CarModel,NumberOfCars,TotalPrice,DiscountValue,Name,Age\n");
        fprintf(fpt, "%s,%hd,%.2f,%.2f,%s,%hd\n", carModel, carsNeeded, totalPrice, discountValue, customerName, customerAge);
    }
    else
    {
        fpt = fopen("salesData.csv", "a+");
        fprintf(fpt, "%s,%hd,%.2f,%.2f,%s,%hd\n", carModel, carsNeeded, totalPrice, discountValue, customerName, customerAge);
    }
    fclose(fpt);
}

void readPurchases()
{
    FILE *fpt;
    fpt = fopen("salesData.csv", "r");
    fclose(fpt);
}

void writeCarsSold()
{
    FILE *fpt;
    fpt = fopen("carsSold.csv", "w");
    for (int i = 0; i < carOptions; i++)
    {
        fprintf(fpt, "%hd,", cars[i].carsSold);
    }
    fclose(fpt);
}

void readCarsSold()
{
    FILE *fpt;
    fpt = fopen("carsSold.csv", "r");
    for (int i = 0; i < carOptions; i++)
    {
        /*get number from file and assign it to appropriate model*/
        fscanf(fpt, "%hd", &cars[i].carsSold);
        /*skip ',' to get next number*/
        fscanf(fpt, ",");
    }
    fclose(fpt);
}

void updateCarsAvailable(){
    for (int i = 0; i < carOptions; i++)
    {
        cars[i].carsAvailable -= cars[i].carsSold;
    }
    
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