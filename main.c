// written by Vorobyov Maksym
// Saturday November 02 2022

#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "structs.h"
#include "globalVariables.h"
#include "prototypes.h"


void main()
{
    /*gets from file number of cars that were sold*/
    readCarsSoldFromFile();
    /*update in each struct "Car" variable "carsAvailable"*/
    updateCarsAvailable();
    while (true)
        menu();
}

void menu()
{
    system("cls"); // clears terminal
    /* Greeting */
    puts("\n\n--------------------------------------");
    puts("***Welcome to the Car Sales office!***");
    puts("--------------------------------------\n");
    printf("a. Buy cars\n"
           "b. View Sales Stats\n"
           "c. View Cars Stock\n"
           "x. Exit\n"
           " - Please choose one: ");

    /*clears buffer*/
    fflush(stdin);
    char switchChoice = getchar();
    switch (tolower(switchChoice))
    {
    case 'a':
        buyCar();
        await();
        break;
    case 'b':
        viewSales();
        await();
        break;
    case 'c':
        viewCarsStock();
        await();
        break;
    case 'x':
        puts("\n*Thanks for using our service*");
        exit(0);
        break;
    default:
        puts("\n*Make sure your choice is correct*\n");
        break;
    }
}

void buyCar()
{
    /*clears terminal*/
    system("cls");
    /*create an instance of the struct*/
    Client client;

    viewCarModels(cars, carOptions);
    short modelChoice = chooseModel();
    /*exit*/
    if (modelChoice == -1)
    {
        return;
    }

    /*validate cars needed*/
    while (true)
    {
        /*ask cars needed*/
        printf(" - How many cars would you like to buy?(0 to exit) Amount: ");
        scanf("%hd", &client.carsNeeded);

        /*Checks if input makes sense*/
        if (client.carsNeeded < 0)
        {
            printf("\n*Make sure you entered a correct number*\n");
        }
        else if (client.carsNeeded == 0)
        {
            return;
        }
        /*check stock*/
        else if (cars[modelChoice].carsAvailable < client.carsNeeded)
        {
            puts(" - Sorry, there are fewer cars available than you require.\n");
        }
        else
        {
            break;
        }
    }

    /*update total price*/
    client.totalPrice = client.carsNeeded * cars[modelChoice].carPrice;

    /*validate customer name*/
    while (true)
    {
        char isCorrect;
        /*get customer name*/
        printf("\n - What is your name? Name: ");
        fflush(stdin);
        /*use %[^\n]s to capture whole name (more than 1 word)*/
        scanf("%[^\n]s", &client.name);
        printf(" - Your name is: %s\n", client.name);
        printf(" - Is your name correct?(y/n): ");
        fflush(stdin);
        scanf("%c", &isCorrect);
        if (tolower(isCorrect) == 'y')
        {
            break;
        }
    }

    /*validating age*/
    while (true)
    {
        printf("\n - How old are you? Age: ");
        fflush(stdin);
        scanf("%hd", &client.age);
        if (client.age < 18)
        {
            puts("\n - Sorry, you are not old enough to buy a car");
            return;
        }
        else if (client.age > 120)
        {
            puts("\n*Make sure your input is correct*\n");
        }
        else
        {
            break;
        }
    }

    /*validate Car Clun membership*/
    while (true)
    {
        printf("\n - Are you a member of the Car Club?(y/n): ");
        fflush(stdin);
        client.isMember = getchar();
        if (tolower(client.isMember) != 'y' && tolower(client.isMember) != 'n')
        {
            puts("\n*Make sure your input is correct*\n");
        }
        else
        {
            break;
        }
    }

    /*call function to check if discount applicable*/
    applyDiscount(&client);

    /*clears terminal*/
    system("cls");

    /* Present discount outcome */
    switch (client.giveDiscount)
    {
    case true:
        // convert from 0.2 format to 20% format
        client.discountPercentage *= 100;
        // hide decimals, not needed
        printf("\n - Discount of %.0f%% applied!\n", client.discountPercentage);
        break;
    case false:
        printf("\n - Discount not applied.\n");
        break;
    }

    /* Present final outcome */
    printf("\n - Thank you for your custom.\n");
    printf("\n - You have bought %hd cars. Total price to pay is %.2f.\n",
           client.carsNeeded, client.totalPrice);

    /*Update stock*/
    cars[modelChoice].carsSold += client.carsNeeded;
    cars[modelChoice].carsAvailable -= client.carsNeeded;
    /*update information about how many cars were sold in the file*/
    writeCarsSoldToFile();

    /*Write information about sale in the file*/
    writePurchaseToFile(cars[modelChoice].carModel, client.carsNeeded, client.totalPrice,
                  client.discountValue, client.name, client.age);
}

void viewCarModels(Car *car, short carOptions)
{
    puts("\n***In our store there are a variety of cars presented!***");
    /*present all models in the shop*/
    printf("%-8s%-24s%-16s%-s\n\n","N.","Model","Price","Available");
    for (int i = 0; i < carOptions; i++)
    {
        printf("%-d.\t%-24s%-.2f\t%-hd\n", i + 1, car[i].carModel, car[i].carPrice, car[i].carsAvailable);
    }
}

short chooseModel()
{
    short carChoice;
    while (true)
    {
        fflush(stdin);
        printf("\n - Choose model(Enter 0 to exit): ");
        scanf("%hd", &carChoice);
        puts(""); // for beatiful output
        if (carChoice == 0)
        {
            break;
        }
        else if (carChoice < 1 || carChoice > 9)
        {
            puts("\n*Make sure your choice is correct*\n");
        }
        else if (cars[carChoice - 1].carsAvailable <= 0)
        {
            puts(" - Sorry, this model is out of stock");
        }
        else{
            break;
        }
    }

    return carChoice - 1;
}

void applyDiscount(Client *client)
{
    // check if the customer is eligible for both discounts
    if (tolower(client->isMember) == 'y' && (client->carsNeeded >= DISCOUNT_MULTIBUY_AMOUNT))
    {
        client->giveDiscount = true;
        /*calculate discount value*/
        client->discountValue = client->totalPrice - (client->totalPrice * (1 - DISCOUNT_MAX));
        /*update total price*/
        client->totalPrice -= client->discountValue;
        client->discountPercentage = DISCOUNT_MAX;
    }

    // otherwise, check member discount
    else if (tolower(client->isMember) == 'y')
    {
        client->giveDiscount = true;
        client->discountValue = client->totalPrice - (client->totalPrice * (1 - DISCOUNT_MEMBER_PERCENTAGE));
        client->totalPrice -= client->discountValue;
        client->discountPercentage = DISCOUNT_MEMBER_PERCENTAGE;
    }

    // otherwise, check multibuy discount
    else if (client->carsNeeded >= DISCOUNT_MULTIBUY_AMOUNT)
    {
        client->giveDiscount = true;
        client->discountValue = client->totalPrice - (client->totalPrice * (1 - DISCOUNT_MULTIBUY_PERCENTAGE));
        client->totalPrice -= client->discountValue;
        client->discountPercentage = DISCOUNT_MULTIBUY_PERCENTAGE;
    }
}

void viewCarsStock()
{
    /*clears terminal*/
    system("cls");

    short switchChoice;
    printf("\n1. View Cars Stock\n"
           "2. Sort by remaining number in descending order\n"
           "3. Exit\n"
           " - Please choose one: ");
    scanf("%d", &switchChoice);
    switch (switchChoice)
    {
    case 1:
        /*clears terminal*/
        system("cls");

        viewCarModels(cars, carOptions);
        break;
    case 2:
        /*clears terminal*/
        system("cls");

        /*Create a new array with car models, but sorted in order to not to change the main array*/
        Car *carsSorted = sortCarsStockByAvailableCars(cars, carOptions);
        viewCarModels(carsSorted, carOptions);
        /*free allocated memory*/
        free(carsSorted);
        break;
    case 3:
        return;
        break;
    default:
        puts("\n*Make sure your choice is correct*\n");
        break;
    }
}

Car *sortCarsStockByAvailableCars(Car *cars, short numberOfCars)
{   
    /*
    allocating memory for the array to make it possible 
    to access it with the pointer
    carsTemp is needed to keep the original array without changes
    */
    Car *carsTemp = malloc(sizeof(Car) * numberOfCars);
    /*assigning values from initial array to temp*/
    for (int i = 0; i < numberOfCars; i++)
    {
        carsTemp[i] = cars[i];
    }

    /*  
        explanation of "numberOfCars - i - 1"
        https://www.youtube.com/watch?v=YqzNgaFQEh8&list=LL&index=4
        work principle:
        
        [3, 2, 1]   i = 0, j = 0, j < 3-i-1 = 3-0-1 = 2
        3 > 2       
        [2, 3, 1]   i = 0, j = 1, j < 2
        3 > 1
        [2, 1, 3]   i = 0, j = 2, j < 2 (end of loop)
        Now "3" is in its place, so we no longer need to compare it.
        This was predefined in "j < length - i - 1" condition, so now we break out this loop to outer one.
        
        [2, 1, 3]   i = 1, j = 0, j < 3-i-1 = 3-1-1 = 1
        2 > 1
        [1, 2, 3]   i = 1, j = 1, j < 1 (end of loop)
    */
    for (int i = 0; i < numberOfCars; i++)
    {
        for (int j = 0; j < numberOfCars - i - 1; j++)
        {
            if (carsTemp[j].carsAvailable < carsTemp[j + 1].carsAvailable)
            {
                /*swapping structures inside the array*/
                Car temp = carsTemp[j + 1];
                carsTemp[j + 1] = carsTemp[j];
                carsTemp[j] = temp;
            }
        }
    }

    return carsTemp;
}

void viewSales()
{
    /*clears terminal*/
    system("cls");

    short numOfLines = readNumberOfLinesFromFile(SALESDATAFILE);
    short switchChoice;
    Client sales[numOfLines];
    readPurchasesFromFile(numOfLines, sales); // passing sales by reference
    printf("\n1. Display total sales for each model\n"
           "2. Display what each customer bought\n"
           "3. View all sales\n"
           "4. Sort sales\n"
           "5. Exit\n"
           " - Please choose one: ");
    fflush(stdin);
    scanf("%hd", &switchChoice);

    switch (switchChoice)
    {
    case 1:
        viewSalesPerModel(sales, numOfLines);
        break;
    case 2:
        viewSalesPerCustomer(sales, numOfLines);
        break;
    case 3:
        viewAllSales(sales, numOfLines);
        break;
    case 4:
        viewSalesSorted(sales, numOfLines);
        break;
    case 5:
        return;
        break;
    default:
        puts("\n*Make sure your choice is correct*\n");
        break;
    }
}

void viewAllSales(Client *sales, short numOfLines)
{
    printf("\n%-20s%-12s%-16s%-16s%-24s%-s\n","Model","Quantity","Total Price","Discount","Customer Name", "Age");
    for (int i = 0; i < numOfLines; i++)
    {
        printf("%-16s\t%-hd\t%-8.2f\t%-8.2f\t%-20s\t%-hd\n", sales[i].carModel, sales[i].carsNeeded, sales[i].totalPrice, sales[i].discountValue, sales[i].name, sales[i].age);
    }
}

void viewSalesSorted(Client *sales, short numOfLines)
{
    system("cls");

    short switchChoice;
    printf("\n1. Sort by car model\n"
           "2. Sort by total price\n"
           "3. Sort by quantity\n"
           "4. Exit\n"
           " - Please choose one: ");
    fflush(stdin);
    scanf("%hd", &switchChoice);
    switch (switchChoice)
    {
    case 1:
        puts("Model sort");
        bubbleSortSaleByCarModel(sales, numOfLines);
        viewAllSales(sales, numOfLines);
        break;
    case 2:
        bubbleSortBySaleTotalPrice(sales, numOfLines);
        viewAllSales(sales, numOfLines);
        break;
    case 3:
        bubbleSortSaleByQuantityPurchased(sales, numOfLines);
        viewAllSales(sales, numOfLines);
        break;
    case 4:
        break;
    default:
        puts("\n*Make sure your choice is correct*\n");
        break;
    }
}

void bubbleSortBySaleTotalPrice(Client *sales, short numOfLines)
{
    for (int i = 0; i < numOfLines; i++)
    {
        for (int j = 0; j < numOfLines - i - 1; j++)
        {
            /*  
                explanation of "numberOfCars - i - 1"
                https://www.youtube.com/watch?v=YqzNgaFQEh8&list=LL&index=4
                work principle:
                
                [3, 2, 1]   i = 0, j = 0, j < 3-i-1 = 3-0-1 = 2
                3 > 2       
                [2, 3, 1]   i = 0, j = 1, j < 2
                3 > 1
                [2, 1, 3]   i = 0, j = 2, j < 2 (end of loop)
                Now "3" is in its place, so we no longer need to compare it.
                This was predefined in "j < length - i - 1" condition, so now we break out this loop to outer one.
                
                [2, 1, 3]   i = 1, j = 0, j < 3-i-1 = 3-1-1 = 1
                2 > 1
                [1, 2, 3]   i = 1, j = 1, j < 1 (end of loop)
            */
            if (sales[j].totalPrice < sales[j + 1].totalPrice)
            {
                /*swap whole structure inside the array*/
                Client temp = sales[j + 1];
                sales[j + 1] = sales[j];
                sales[j] = temp;
            }
        }
    }
}

void bubbleSortSaleByQuantityPurchased(Client *sales, short numOfLines)
{
    for (int i = 0; i < numOfLines; i++)
    {
        for (int j = 0; j < numOfLines - i - 1; j++)
        {
            /*  
                explanation of "numberOfCars - i - 1"
                https://www.youtube.com/watch?v=YqzNgaFQEh8&list=LL&index=4
                work principle:
                
                [3, 2, 1]   i = 0, j = 0, j < 3-i-1 = 3-0-1 = 2
                3 > 2       
                [2, 3, 1]   i = 0, j = 1, j < 2
                3 > 1
                [2, 1, 3]   i = 0, j = 2, j < 2 (end of loop)
                Now "3" is in its place, so we no longer need to compare it.
                This was predefined in "j < length - i - 1" condition, so now we break out this loop to outer one.
                
                [2, 1, 3]   i = 1, j = 0, j < 3-i-1 = 3-1-1 = 1
                2 > 1
                [1, 2, 3]   i = 1, j = 1, j < 1 (end of loop)
            */
            if (sales[j].carsNeeded < sales[j + 1].carsNeeded)
            {
                /*swap whole structure inside the array*/
                Client temp = sales[j + 1];
                sales[j + 1] = sales[j];
                sales[j] = temp;
            }
        }
    }
}

void bubbleSortSaleByCarModel(Client *sales, short numOfLines)
{
    for (int i = 0; i < numOfLines; i++)
    {
        for (int j = 0; j < numOfLines - i - 1; j++)
        {
            /*  
                explanation of "numberOfCars - i - 1"
                https://www.youtube.com/watch?v=YqzNgaFQEh8&list=LL&index=4
                work principle:
                
                [3, 2, 1]   i = 0, j = 0, j < 3-i-1 = 3-0-1 = 2
                3 > 2       
                [2, 3, 1]   i = 0, j = 1, j < 2
                3 > 1
                [2, 1, 3]   i = 0, j = 2, j < 2 (end of loop)
                Now "3" is in its place, so we no longer need to compare it.
                This was predefined in "j < length - i - 1" condition, so now we break out this loop to outer one.
                
                [2, 1, 3]   i = 1, j = 0, j < 3-i-1 = 3-1-1 = 1
                2 > 1
                [1, 2, 3]   i = 1, j = 1, j < 1 (end of loop)
            */
            if (strcmp(sales[j].carModel, sales[j + 1].carModel) > 0)
            {
                /*swap whole structure inside the array*/
                Client temp = sales[j + 1];
                sales[j + 1] = sales[j];
                sales[j] = temp;
            }
        }
    }
}

void viewSalesPerModel(Client *sales, short numOfLines)
{
    /*present all models in the shop*/
    viewCarModels(cars, carOptions);
    short modelChoice = chooseModel();
    if (modelChoice == -1)
    {
        return;
    }
    printf("\n%-20s%-12s%-16s%-16s%-24s%-s\n","Model","Quantity","Total Price","Discount","Customer Name", "Age");
    for (int i = 0; i < numOfLines; i++)
    {
        /*compare model choice with presented models*/
        if (strcmp(sales[i].carModel, cars[modelChoice].carModel) == 0)
        {
            printf("%-16s\t%-hd\t%-8.2f\t%-8.2f\t%-20s\t%-hd\n", sales[i].carModel, sales[i].carsNeeded, sales[i].totalPrice, sales[i].discountValue, sales[i].name, sales[i].age);
        }
    }
}

void viewSalesPerCustomer(Client *sales, short numOfLines)
{
    char nameToSearch[100];
    printf("\n - Enter a name which you used for purchase: ");
    fflush(stdin); // clears buffer
    scanf("%[^\n]s", &nameToSearch);
    printf("\n%-20s%-12s%-16s%-16s%-24s%-s\n","Model","Quantity","Total Price","Discount","Customer Name", "Age");
    for (int i = 0; i < numOfLines; i++)
    {
        if (strcmp(sales[i].name, nameToSearch) == 0)
        {
            printf("%-16s\t%-hd\t%-8.2f\t%-8.2f\t%-20s\t%-hd\n", sales[i].carModel, sales[i].carsNeeded, sales[i].totalPrice, sales[i].discountValue, sales[i].name, sales[i].age);
        }
    }
}

void writePurchaseToFile(char carModel[100], short carsNeeded, float totalPrice, float discountValue, char customerName[100], short customerAge)
{
    FILE *fpt;
    /*Check if file exists*/
    if (access(SALESDATAFILE, F_OK) != 0)
    {
        /*create a file*/
        fpt = fopen(SALESDATAFILE, "w+");
        fprintf(fpt, "%s,%hd,%.2f,%.2f,%s,%hd\n", carModel, carsNeeded, totalPrice, discountValue, customerName, customerAge);
    }
    else
    {
        /*add data to the existing file*/
        fpt = fopen(SALESDATAFILE, "a+");
        fprintf(fpt, "%s,%hd,%.2f,%.2f,%s,%hd\n", carModel, carsNeeded, totalPrice, discountValue, customerName, customerAge);
    }
    fclose(fpt);
}

short readNumberOfLinesFromFile(char fileName[])
{
    FILE *fpt;
    fpt = fopen(fileName, "r");
    short numOfLines = 0;
    for (char c = getc(fpt); c != EOF; c = getc(fpt))
        if (c == '\n') // Increment count if this character is newline
            numOfLines++;
    // Close the file
    fclose(fpt);
    return numOfLines;
}

void readPurchasesFromFile(short numOfLines, Client *sales)
{
    FILE *fpt;
    fpt = fopen(SALESDATAFILE, "r");
    short i = 0;
    while (i < numOfLines)
    {
        fscanf(fpt, "%28[^,],%hd,%f,%f,%100[^,],%hd\n", sales[i].carModel, &sales[i].carsNeeded, &sales[i].totalPrice, &sales[i].discountValue, &sales[i].name, &sales[i].age);
        i++;
    }
    fclose(fpt);
}

void writeCarsSoldToFile()
{
    FILE *fpt;
    fpt = fopen(CARSSOLDFILE, "w");
    for (int i = 0; i < carOptions; i++)
    {
        fprintf(fpt, "%hd,", cars[i].carsSold);
    }
    fclose(fpt);
}

void readCarsSoldFromFile()
{
    FILE *fpt;
    fpt = fopen(CARSSOLDFILE, "r");
    for (int i = 0; i < carOptions; i++)
    {
        /*get number from file and assign it to appropriate model*/
        fscanf(fpt, "%hd", &cars[i].carsSold);
        /*skip ',' to get next number*/
        fscanf(fpt, ",");
    }
    fclose(fpt);
}

void updateCarsAvailable()
{
    for (int i = 0; i < carOptions; i++)
    {
        cars[i].carsAvailable -= cars[i].carsSold;
    }
}

void await()
{
    printf("\n*Press any key to continue...*\n");
    fflush(stdin);
    getchar();
    return;
}
