#include <stdio.h>
#include <math.h>
#include "graphics.h"

// This code asks the user for an angle to rotate the drawing and the code will rotate the drawing based on the angle given.
// Uses Functions, Pointers, and Plotting

int cmain()
{

    // Declarations and initializations

    FILE *inputFile = NULL;
    float angle_in_degrees, r, theta, x, y;
    float lower_left_ytheta, lower_left_xr, upper_right_xr, upper_right_ytheta, x_left, x_right, y_down, y_upper, r_first_pen, theta_first_pen, x_first, y_first;


    // Function prototypes

    void rotateByAngle(float*, float);
    void polarToCartesian(float, float, float*, float*);

    // Open the input file and make sure it opened properly.

    inputFile = fopen("/Users/teresanguyen/Documents/Yale_Classes/ENAS130/Homework/build-prob07_starter_code-Qt_6_6_1_for_macOS-Debug/prob07_r_and_theta_values.dat", "r");

    if (inputFile == NULL)
    {
        printf("Could not open inputFile...\n");
        return 1;
    }

    // Prompt for and get the rotation angle.

    printf("Positive angle -> counterclockwise rotation\n");
    printf("Negative angle -> clockwise rotation\n");

    printf("\nEnter the angle, in degrees, by which to rotate the data: ");
    scanf("%f", &angle_in_degrees);

    // Read in the lower left point, rotate it, and convert it to Cartesian.

    fscanf(inputFile, "%f %f", &lower_left_xr, &lower_left_ytheta);
    rotateByAngle(&lower_left_ytheta, angle_in_degrees);
    polarToCartesian(lower_left_xr, lower_left_ytheta, &x_left, &y_down);


    // Read in the upper right point, rotate it, and convert it to Cartesian.

    fscanf(inputFile, "%f %f", &upper_right_xr, &upper_right_ytheta);
    rotateByAngle(&upper_right_ytheta, angle_in_degrees);
    polarToCartesian(upper_right_xr, upper_right_ytheta, &x_right, &y_upper);


    // Open a graphics window and scale it (i.e., set the coordinate system
    // inside the window).

    plotOn(400, 1075);

    xScale(x_left, x_right);
    yScale(y_down, y_upper);


    // Read in the rest of the data and draw the shapes.

    fscanf(inputFile, "%f %f", &r_first_pen, &theta_first_pen);
    rotateByAngle(&theta_first_pen, angle_in_degrees);
    polarToCartesian(r_first_pen, theta_first_pen, &x_first, &y_first);
    setPen(x_first, y_first);


    while (fscanf(inputFile, "%f %f", &r, &theta) != EOF)
    {

        // ensures that the distance is located less than or equal to 1000 units away
        if (r <= 1000)
        {
            rotateByAngle(&theta, angle_in_degrees);
            polarToCartesian(r, theta, &x, &y);
            drawLineSeg(x, y);
        }
        else
        {
            // if the vertex is located more than 1000 units away, it scans a new vertex and sets that as the new point/pen
            fscanf(inputFile, "%f %f", &r, &theta);
            rotateByAngle(&theta, angle_in_degrees);
            polarToCartesian(r, theta, &x, &y);
            setPen(x, y);
        }

    }

    // Close the input file, since we're done reading from it.

    fclose(inputFile);


    return 0;
}

// Rotates the angle (theta in radians) given a rotation angle (alpha in degrees) and returns the updated value of theta (in radians)

void rotateByAngle(float *theta, float alpha_rotation_angle)
{
    const double pi = 4.0 * atan(1.0);
    float alphaRadians = alpha_rotation_angle * pi / 180.0;
    *theta += alphaRadians;

    return;

}

// Converts coordinates of a point polar (r, theta) to (x,y) cartesian
// uses pointers to change two values (x and y)

void polarToCartesian(float r, float theta, float *x, float *y)
{
    *x = r*cos(theta);
    *y = r*sin(theta);
    return;
}


