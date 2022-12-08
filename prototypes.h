/*funtions prototypes*/
void menu();

void buyCar();

/**
 * @brief outputs table of models/price/quantity based on a given array
 *
 * @param cars array of structs "Car"
 * @param carOptions number of values in the array
 */
void viewCarModels(Car *cars, short carOptions);
short chooseModel();

/**
 * @brief Based on cars needed and membership applies a discount
 *
 * @param client (pointer to struct)
 */
void applyDiscount(Client *client);

void viewCarsStock();
/**
 * @brief sorting cars by the remaining number in descending order
 * 
 * @param cars - array of "Car" stuctures
 * @param numberOfCars - number of car models
 * @return Car* - returns a pointer to the array inside a function
 */
Car *sortCarsStockByAvailableCars(Car *cars, short numberOfCars);

void viewSales();

/**
 * @brief outputs all sales of selected model
 *
 * @param sales - pointer to the array of Client structs - "sales"
 * @param numOfLines - number of lines in file "salesData" == number of values in array "sales"
 */
void viewSalesPerModel(Client *sales, short numOfLines);

/**
 * @brief outputs all sales of a customer based on an input name
 *
 * @param sales - pointer to the array of Client structs - "sales"
 * @param numOfLines - number of lines in file "salesData" == number of values in array "sales"
 */
void viewSalesPerCustomer(Client *sales, short numOfLines);
void viewAllSales(Client *sales, short numOfLines);
void viewSalesSorted(Client *sales, short numOfLines);
void bubbleSortSaleByCarModel(Client *sales, short numOfLines);
void bubbleSortBySaleTotalPrice(Client *sales, short numOfLines);
void bubbleSortSaleByQuantityPurchased(Client *sales, short numOfLines);
void viewSalesPerCustomer(Client *sales, short numOfLines);

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
void writePurchaseToFile(char carModel[100], short carsNeeded, float totalPrice, float discountValue, char customerName[100], short customerAge);

/**
 * @brief reads number of lines in file "salesData.csv"
 *
 * @param fileName
 * @return short -> numberOfLines
 */
short getNumberOfLinesFromFile(char fileName[]);

/**
 * @brief reads information about sales from file "salesData.csv".
 *        Using "fscanf" function formats data and assigns its to according value in array of "Client" structs
 *
 * @param numOfLines number of lines in the file
 * @param sales passing a pointer to the existing array of structs to manipulate with the data directly
 */
void readPurchasesFromFile(short numOfLines, Client *sales);
void writeCarsSoldToFile();
void getCarsSoldFromFile();
void updateCarsAvailable();
void await();