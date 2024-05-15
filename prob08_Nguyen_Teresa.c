#include <stdio.h>
#include <math.h>
#include "graphics.h"

// This code takes as input the name of a file that contains points (x, y) per line and plots the least squares line with its points using covariance, variance, averages, and the regression coefficient.

#define NMAX 50
#define FILENAMEMAX 256

int cmain()
{
    // Declarations and initializations

    int i, N = 0;

    // all real variables are declared as floats
    float x_array[NMAX], y_array[NMAX], x_min, x_max, x_average, y_min, y_max, y_average, x_variance, y_variance, xy_covariance, regression_coef, plotOn_width, m, b;

    char filename[FILENAMEMAX];
    FILE *inputFile=NULL;


    // Function prototypes
    float getAvgOfOneArray(float [], int);
    float getMaxOfOneArray(float [], int);
    float getMinOfOneArray(float [], int);
    float getVarOfOneArray(float [], int);
    float getCovarOfTwoArrays(float [], float [], int);

    // Prompt for the filename, and read characters into the filename array.

    printf("Enter the name of the input file (or drag it here):\n");
    scanf("%s", filename);

    // Open the input file and make sure it opened properly.
    // Path Name: /Users/teresanguyen/Documents/Yale_Classes/ENAS130/Homework/build-prob08_starter_code-Qt_6_6_1_for_macOS-Debug/prob08_x_and_y_values.dat

    inputFile = fopen(filename, "r");

    if (inputFile == NULL)
    {
        printf("Could not open file %s.\n", filename);
        return 1;
    }

    // Read data from the input file and store in the x and y arrays, being
    // careful not to write any data beyond the array bounds.

    for (i=0; i < NMAX; i++)
    {
        if (fscanf(inputFile, "%f %f", &x_array[i], &y_array[i]) == EOF) break;
        N++;
    }

    // only true if EOF wasn't reached while reading the data
    if (i == NMAX)
    {
        // the scope of these variables are within the code block
        float extra_data_x, extra_data_y;

        if (fscanf(inputFile, "%f %f", &extra_data_x, &extra_data_y) != EOF)
        {
            printf("Too much data.\n");
            return 1;
        }
    }

    // Close the input file, since we're done reading from it.

    fclose(inputFile);

    // Write the number of points to the terminal.

    printf("\n%d points were read in.\n", N);

    // Find the min, the max, and the average of the x data and print to the
    // terminal.  Do the same for the y data.

    x_min = getMinOfOneArray(x_array, N);
    x_max = getMaxOfOneArray(x_array, N);
    x_average = getAvgOfOneArray(x_array, N);

    printf("\nx is in the interval [ %.3f, %.3f] with an avg of %.3f\n", x_min, x_max, x_average);

    y_min = getMinOfOneArray(y_array, N);
    y_max = getMaxOfOneArray(y_array, N);
    y_average = getAvgOfOneArray(y_array, N);

    printf("y is in the interval [ %.3f, %.3f] with an avg of %.3f\n", y_min, y_max, y_average);

    // Find the variance of the x data, the variance of the y data, and the
    // covariance.

    x_variance = getVarOfOneArray(x_array, N);
    y_variance = getVarOfOneArray(y_array, N);

    xy_covariance = getCovarOfTwoArrays(x_array, y_array, N);

    // Calculate the regression coefficient and print to the terminal.

    regression_coef = pow(xy_covariance, 2) / (x_variance * y_variance);
    printf("\nRegression coefficient r^2: %.3f\n", regression_coef);

    // Open a graphics window and scale it.

    // The width is scaled by making the ratio of 600 px (height) and the width to be equal to the ratio of the ranges of x and y
    plotOn_width = (600*(x_max - x_min)) / (y_max - y_min);

    plotOn(600, plotOn_width);

    // scaled so that way the data points with extreme values will not be at the edges
    // left and right margins are equal to 10% of the data's domain (x_max - x_min)
    xScale(x_min - 0.10*(x_max - x_min), x_max + 0.10*(x_max - x_min));
    // top and bottom margins are equal to 10% of the data's range (y_max - y_min)
    yScale(y_min - 0.10*(y_max - y_min), y_max + 0.10*(y_max - y_min));

    // Plot the data points using crosses.

    for (int i = 0; i < N; i++)
    {
        drawCross(x_array[i], y_array[i]);
    }

    // Draw the Least Squares Line

    setPen(x_min, y_min);
    drawLineSeg(x_max, y_max);

    // If the regression coefficient is > 0.5, then do some additional things.

    if (regression_coef > 0.5)
    {
        // the equation for m (slope) is cov(x,y) / var(x)
        m = xy_covariance / x_variance;

        // the equation for the y-intercept is the (average of y) - (slope)*(average of x)
        b = y_average - (m*x_average);

        printf("\nLeast squares line's slope: %.3f\n", m);
        printf("Least squares line's y-intercept: %.3f\n", b);
    }


    return 0;
}

//------------------------------------------------------------------------------
// Function definitions (two are already done for you)
//------------------------------------------------------------------------------

// Determine the average of the values in an array containing n pieces of data.
float getAvgOfOneArray(float dataArray[], int n)
{
    // Declaration and initialization
    float sum = 0;

    // Sum the elements in the array.
    for (int i=0; i<n; i++) 
        sum += dataArray[i];

    return (sum/n);
}

// Determine the maximum value in an array containing n pieces of data.
float getMaxOfOneArray(float dataArray[], int n)
{
    // Declaration and initialization
    float max = dataArray[0];

    // Loop over the rest of the elements to find the maximum.
    for (int i=1; i<n; i++)
        if (dataArray[i] > max) max = dataArray[i];

    return max;
}

// Determine the minimum value in an array containing n pieces of data.

float getMinOfOneArray(float dataArray[], int n)
{
    // Declaration and initalization
    float min = dataArray[0];

    // Loop over the rest of the elements to find the minimum
    for (int i = 1; i<n; i++)
    {
        if (dataArray[i] < min)
        {
            min = dataArray[i];
        }

    }

    return min;
}

// Determine the variance of the values in an array containing n pieces of data,
// given the average.

float getVarOfOneArray(float dataArray[], int n)
{

    float sum_of_var_squared = 0.0;

    // Loop over the elements to get the sum
    for (int i = 0; i<n; i++)
    {
        sum_of_var_squared += (pow(dataArray[i], 2));
    }

    // call the function we created earlier
    return (sum_of_var_squared / n) - (pow(getAvgOfOneArray(dataArray, n), 2));

}


// Determine the covariance of the values in two arrays, given the average of
// each array.  Each array contains n pieces of data.


float getCovarOfTwoArrays(float dataArray1[], float dataArray2[], int n)
{
    float sum_of_var_times_var = 0.0;

    for (int i = 0; i<n; i++)
    {
        sum_of_var_times_var += (dataArray1[i] * dataArray2[i]);
    }

    // call the functions that we created earlier
    return (sum_of_var_times_var / n) - ((getAvgOfOneArray(dataArray1, n)) * (getAvgOfOneArray(dataArray2, n)));
}

