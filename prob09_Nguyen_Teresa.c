#include <stdio.h>  // You may need to #include another header file, too.
#include <stdlib.h>

// Presents a list of customers who require a fuel oil delivery on that day and prints a delivery schedule to the terminal.
// After printing the delivery schedule, the code prints the list of customers with the estimated amount of fuel oil that
// remains in each customer's tank at the end of the forecast period


// Put the definitions of the ForecastRecord and CustomerRecord structures
// before main, so that all functions below can see them.

#define NDAYS 12
#define STREETNAMEMAX 12
#define FILENAMEMAX 256
#define MONTHNAMEMAX 10

// Use typedef to define ForecastRecord and CustomerRecord as new data types.

typedef struct {
    char month[MONTHNAMEMAX];
    int day_of_month;
    int t_high;
    int t_low;
} ForecastRecord;

typedef struct {
    int building_number;
    char streetName[STREETNAMEMAX];
    int fuel_tank_capacity;
    double estimated_number_of_gallons;
    double building_kFact;
} CustomerRecord;


//=================================================================================
// MAIN PROGRAM
//=================================================================================

int main() 
{
    // Declarations

    int num_customers, scheduled_del;
    double t_diff, HDD, deliver_gallons;
    char filename[FILENAMEMAX];
    FILE *forecastFile=NULL;
    FILE *customerFile=NULL;
    ForecastRecord forecast[NDAYS];
    CustomerRecord *customers=NULL;

    // Function prototypes (given in the problem statement)
    int doesCustNeedDelivery(CustomerRecord);
    void updateCustEstFuelInTank(CustomerRecord*, double, double);

    //-----------------------------------------------------------------------------
    // Read the forecast file.
    //-----------------------------------------------------------------------------

    // Prompt for the name of the forecast file, and read characters into the 
    // filename array.

    printf("Enter the name of the forecast file (or drag it here):\n");
    scanf("%s", filename);
  
    // Open the forecast file and make sure it opened properly.
    // /Users/teresanguyen/Documents/Yale_Classes/ENAS130/Homework/build-prob09_starter_code-Qt_6_6_1_for_macOS-Debug/prob09_winter_forecast.dat

    forecastFile = fopen(filename, "r");
    if (forecastFile == NULL)
    {
        printf("Couldn't open file %s.\n", filename);
        return 1;
    }

    // Read data from the forecast file and store in the forecast array.

    for (int i = 0; i < NDAYS; i++)
    {
        fscanf(forecastFile, "%s %d %d %d", &forecast[i].month[0], &forecast[i].day_of_month, &forecast[i].t_high, &forecast[i].t_low);
    }

    printf("Data for %d forecast days were read in.\n\n", NDAYS);


    // Close the forecast file, since we're done reading from it.

    fclose(forecastFile);

    //-----------------------------------------------------------------------------
    // Read the customers file.
    //-----------------------------------------------------------------------------

    // Prompt for the name of the customers file, and read characters into the
    // filename array.

    printf("Enter the name of the customers file (or drag it here):\n");
    scanf("%s", filename);

  
    // Open the customers file and make sure it opened properly.
    // /Users/teresanguyen/Documents/Yale_Classes/ENAS130/Homework/build-prob09_starter_code-Qt_6_6_1_for_macOS-Debug/prob09_customer_list.dat

    customerFile = fopen(filename, "r");
    if (customerFile == NULL)
    {
        printf("Could not read %s\n", filename);
        return 1;
    }

    // Read 1st line of the customers file (number of customers).

    fscanf(customerFile, "%d", &num_customers);

    // Now allocate space for exactly that number of customers.

    customers = (CustomerRecord*) malloc(num_customers*sizeof(CustomerRecord));

    // Make sure the memory was actually available.

    if (customers == NULL) // same as: if (&sqrtData[0] == NULL)
    {
        printf("Could not allocate memory!\n");
        return 1;
    }

    // Read data from the rest of the customers file and store in the customers 
    // array.

    for (int i = 0; i < num_customers; i++)
    {
        fscanf(customerFile, "%d %s %d %lf %lf", &customers[i].building_number, &customers[i].streetName[0],
               &customers[i].fuel_tank_capacity, &customers[i].estimated_number_of_gallons,
               &customers[i].building_kFact);
    }

    printf("Data for %d customers were read in.\n\n", num_customers);

    // Close the customers file, since we're done reading from it.

    fclose(customerFile);

    //-----------------------------------------------------------------------------
    // Generate the delivery schedule and update estimated fuel info.
    //-----------------------------------------------------------------------------

    printf("\nDELIVERY SCHEDULE\n");

    // Loop over the days in the forecast...

    for (int i = 0; i < NDAYS; i++)
    {
        scheduled_del = 0;
        // Print month and day to the terminal.
        printf("\n%s %d:\n", forecast[i].month, forecast[i].day_of_month);

        // Determine the number of heating degree days for this day.
        t_diff = 65 - ((forecast[i].t_high + forecast[i].t_low) / 2.);
        if (t_diff > 0)
        {
            HDD = t_diff;
        }
        else
        {
            HDD = 0;
        }

        // Loop over the customers...
        for (int j = 0; j < num_customers; j++)
        {
            if (doesCustNeedDelivery(customers[j]))
            {
                deliver_gallons = customers[j].fuel_tank_capacity * 0.70;
                printf("%d %s  \t(%5.1lf)\n", customers[j].building_number, customers[j].streetName, deliver_gallons);
                scheduled_del++;
            }
            else
            {
                deliver_gallons = 0.0;
            }

            updateCustEstFuelInTank(&customers[j], deliver_gallons, HDD);

        // Loops over all the customers: first it determines whether the customer needs a delivery. If the customer needs a delivery,
        // then we will calculate how many gallons to deliver and print the building number, street name, and the number of gallons.
        // If the customer does not need a delivery, deliver gallons is set to 0.
        // The estimated fuel in tank is then updated using the function "updateCustEstFuelInTank"

        // End of loop over the customers...

        }

        // In the previous loop, if the customer needed a delivery, then scheduled_del++ so it would not be 0.
        // If the if statement did not run and only the else did, then the scheduled_del will stay as 0, showing how there were no fuel deliveries
        if (scheduled_del == 0)
        {
            printf("No fuel deliveries scheduled for this day.\n");
        }

    // End of loop over the days in the forecast...
    }

    //-----------------------------------------------------------------------------
    // Print the status of each customer's tank at the end of the forecast period.
    //-----------------------------------------------------------------------------

    printf("\n\nESTIMATED FUEL IN TANK AT END OF FORECAST PERIOD\n\n");

    for (int i = 0; i < num_customers; i++)
    {
        printf("%d %s:  \t%5.1lf gallons\n", customers[i].building_number, customers[i].streetName, customers[i].estimated_number_of_gallons);
    }

    printf("\n");

    // Deallocate the memory and reset the pointer.

    free(customers);
    customers = NULL;

    return 0;
}

//=================================================================================
// doesCustNeedDelivery function
//=================================================================================

// Takes in a data type (CustomerRecord) which is an alias of everything within CustomerRecord (streetname, building number, etc)
// and returns a 1 or 0 depending on if the person needed a delivery

int doesCustNeedDelivery(CustomerRecord customer)
{
    if ((customer.estimated_number_of_gallons / customer.fuel_tank_capacity) < 0.20)
    {
        return 1;
    }
    else
    {
        return 0;
    }
 }

//=================================================================================
// updateCustEstFuelInTank function
//=================================================================================

// Takes in a pointer, double, and double and updates the estimated number of gallons within the customer variable

 void updateCustEstFuelInTank(CustomerRecord *customer, double number_of_gallons, double heating_degrees)
 {
     customer->estimated_number_of_gallons += (number_of_gallons - (heating_degrees*(customer->building_kFact)));
 }


