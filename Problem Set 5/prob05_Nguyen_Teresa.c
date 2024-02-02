#include <stdio.h>
#include <math.h>

/* This code is a minute by minute report on the height of the fuel tank as it empties and how long it takes to empty based on the nozzle diameter */
// Uses File I/O

int main()
{
    // creating variables
    double nozzle_diameter, oil_height = 12., rate_volume_decreases, change_in_oil_volume, A_nozzle, speed_oil_flows;
    const double pi=4.*atan(1.);
    double tank_volume = pi*pow(12.5,2)*12;

    // we know that these values will not result in a decimal so we can declare them as int
    int time = 0, i, seconds = 0;

    printf("Enter a nozzle diameter in cm:\n");
    scanf("%lf", &nozzle_diameter);

    // convert the nozzle diameter (cm) to meters
    nozzle_diameter = nozzle_diameter * 0.01;

    // calculate the area of the nozzle
    A_nozzle = pi*pow((nozzle_diameter / 2.), 2.);

    // file-pointers
    FILE *outputFile = NULL;

    outputFile = fopen("tank_emptying_report.dat", "w");

    // check to ensure that outputFile is opened
    if (outputFile == NULL)
    {
        printf("Couldn't open file tank_emptying_report.dat.\n");
        return 1;
    }

    // print the nozzle diameter to the output file
    fprintf(outputFile, "Nozzle diameter = %.2lf cm\n", nozzle_diameter * 100);
    fprintf(outputFile, "\nTime (min)    Oil height (m)\n");
    fprintf(outputFile, "%7d %17.6lf\n", time, oil_height);

    // we do not know how many times this code is going to run so we use a while loop
    while (oil_height > 0)
    {
        for (i = 6; i > 0; i--)
        {
            // for loop for every minute
            seconds += 10;
            speed_oil_flows = sqrt(2*9.81*oil_height);
            rate_volume_decreases = speed_oil_flows * A_nozzle;
            change_in_oil_volume = rate_volume_decreases * 10.;

            // Check if the tank volume is going to be negative
            if (tank_volume - change_in_oil_volume < 0)
            {
                tank_volume = 0;
                oil_height = 0;
                break;
            }
            else
            {
                tank_volume -= change_in_oil_volume;
                oil_height = tank_volume / (pi * pow(12.5, 2.));
            }
        }

        // To ensure that NaN is not printed
        if (oil_height < 0.000001)
        {
            break;
        }

        // counts the amount of minutes
        time++;

        // print to outputFile
        fprintf(outputFile, "%7d %17.6lf\n", time, oil_height);

    }

    // final print statements
    fprintf(outputFile, "\nTime to empty the tank: %d minutes %d seconds", time, seconds % 60);
    printf("Time to empty the tank: %d minutes %d seconds", time, seconds % 60);
    printf("\nMinute-by-minute report: tank_emptying_report.dat\n");

    fclose(outputFile);

    return 0;
}
