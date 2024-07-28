#include <stdio.h>
#include <math.h>

// This code calculates the range of R value based on the user's input of thickness and radius of a pipe.
// Uses functions

int main()
{
    // create the necessary variables for the code
    double radius, thickness, k_value1 = 0.017, k_value2 = 0.026, range_1, range_2;

    // function prototypes
    double calcRvalue(double, double, double);

    // get the radius and thickness in inches from the user
    printf("Enter a pipe radius in inches: ");
    scanf("%lf", &radius);

    printf("Enter the fiberglass insulation thickness in inches: ");
    scanf("%lf", &thickness);

    // convert radius and thickness (inches) to feet before calling the function
    radius /= 12;
    thickness /= 12;

    // use the functions to calculate R's range
    range_1 = calcRvalue(radius, thickness, k_value2);
    range_2 = calcRvalue(radius, thickness, k_value1);

    // print out the range
    printf("\nFor this configuration, the R value ranges from %.1lf to %.1lf.\n", range_1, range_2);

    return 0;
}

// takes in three inputs: radius, thickness, k_value (is a input because we created two variables of k for the range)
double calcRvalue(double radius, double thickness, double k_value)
{
    return (radius / k_value)*log((radius + thickness) / radius);
}

