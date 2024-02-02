#include <stdio.h>

int main()
{
    // initalize variables
    int num_bulbs;
    float wattage_rating, current;

    // take user input
    printf("Enter the number of bulbs and the wattage rating: ");
    scanf("%d %f", &num_bulbs, &wattage_rating);
    int voltage = 120;

    // calculate the current
    current = (wattage_rating * num_bulbs) / voltage;

    // if statement for printing
    if (current > 1.5)
        printf("Current is %.2f, which is unsafe.\n", current);
    else
        printf("Current is %.2f, which is safe.\n", current);

    return 0;

}
