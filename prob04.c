#include <stdio.h>

/* Finds the amount of data pairs read from the input file, calculates the covariance, and determines
whether x and y are positively, negatively, or uncorrelated */
// Uses file I/O for input and output. Output is also printed to the terminal.

int main()
{
    // file point variables
    FILE *inputFile = NULL, *outputFile = NULL;

    // declare variables needed for the program
    int counter = 0;
    double x, y, xy_sum = 0, x_sum = 0, y_sum = 0, covariance;

    // open the input and output files
    inputFile = fopen("prob04_x_and_y_values.dat", "r");
    outputFile = fopen("prob04_covariance_calculation", "w");

    // ensure that the inputFile is opened correctly
    if (inputFile == NULL)
    {
        printf("Couldn't open file myinput.dat.\n");
        return 1;
    }

    // ensure that the outputFile is opened correctly
    if (outputFile == NULL)
    {
        printf("Couldn't open file myoutput.dat.\n");
        return 1;
    }

    // loop until the end of the file
    while (fscanf(inputFile, "%lf %lf", &x, &y) != EOF)
    {
        // counts how many data pairs are in the input file
        counter += 1;

        // updates all the sums for calculation
        x_sum += x;
        y_sum += y;
        xy_sum += (x*y);
    }

    // Close the input file, since we're done reading from it.
    fclose(inputFile);

    // calculate covariance
    covariance = ((xy_sum) / counter) - (x_sum / counter)*(y_sum / counter);

    // regardless of if covariance is positively, negatively, or uncorrelated, we still need these lines (prints to terminal and file I/O)
    printf("%d data pairs were read in.\nCovariance: %.2lf", counter, covariance);
    fprintf(outputFile, "%d data pairs were read in.\nCovariance: %.2lf\n", counter, covariance);

    // checks to see if the covariance is positvely, negatively, or uncorrelated to print to the terminal and write to output file
    if (covariance > 0) {
        printf("\nx and y are positively correlated.\n");
        fprintf(outputFile,"x and y are positively correlated.\n");
    }
    else if (covariance < 0) {
        printf("\nx and y are negatively correlated.\n");
        fprintf(outputFile, "x and y are negatively correlated.\n");
    }
    else {
        printf("\nx and y are uncorrelated.\n");
        fprintf(outputFile, "x and y are uncorrelated.\n");
    }

    // Close the output file, since we're done writing to it.
    fclose(outputFile);

    return 0;
}
