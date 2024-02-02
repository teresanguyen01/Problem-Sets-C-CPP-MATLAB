// This code calculates a gas mileage and returns a price based on how much gas was used.

#include <stdio.h>

int main()
{
    // create variables
    // NOTE: readings are int's because we expect cubic meters to be integers
    int previous_reading, current_reading, gas;
    // NOTE: current bill is 0 in order to use +=
    float current_bill = 0;
\
    // scan for user input
    printf("Enter the previous and current readings:\n");
    scanf("%d %d", &previous_reading, &current_reading);

    // calculate gas
    if (current_reading > previous_reading)
    {
        gas = current_reading - previous_reading;
    }
    else if (current_reading < previous_reading)
    {
        gas = (10000 - previous_reading) + current_reading;
    }
    else
    {
        gas = 0;
    }

    // print gas usage before value of "gas" changes
    printf("Gas usage: %d cubic meters\n", gas);


    // calculate the current bill
    if (gas < 70)
    {
        current_bill += 7;
    }
    else if (gas > 70)
    {
        current_bill += 7;
        gas -= 70;

        // we use the following if-else so that way we can ensure we add current_bill by the correct amount (ex: if gas is
        // less than 100, multiply 0.05 by the gas value, else multiply gas by 100 and then subtract...(same logic for the rest)
        if (gas >= 0 && gas < 100)
        {
            current_bill += (0.05*gas);
        }
        else
        {
            current_bill += (0.05*100);
            gas -= 100;
            if ((gas >= 0) && (gas < 230))
            {
                current_bill += (0.025*gas);
            }
            else
            {
                current_bill += (0.025*230);
                gas -= 230;
                current_bill += (0.015*gas);
            }
        }
    }

    // print the current bill - ensure to round to two decimal points
    printf("Current bill: $%.2f\n", current_bill);

    return 0;
}
