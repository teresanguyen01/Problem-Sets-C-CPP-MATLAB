// Keeps track of the real and imaginary parts of a complex number and prints out the user input of complex numbers

#include <iostream>

using namespace std;

// Class declaration
class Complex
{
    // create the member variables
    private:
        int realPart;
        int imagPart;

    // function prototypes for the class Complex
    public:
        Complex (int, int);
        void show();
};

// Class implementation
Complex::Complex(int real, int imag)
{
    // initalizes the member variables to the arguments
    realPart = real;
    imagPart = imag;
}

// Print out the the real and imaginary parts of a complex number
void Complex::show()
{
    // We don't want a case where it prints like "8+-1i" so we
    // make an if statement without the + if the imaginary number is negative
    if (imagPart < 0)
    {
        cout << "The number is " << realPart << imagPart << "i." << endl << endl;
    }
    else
    {
        cout << "The number is " << realPart << "+" << imagPart << "i." << endl << endl;
    }
    return;
}

int main()
{
    // initalize the input variables
    int real, imaginary;

    // Ask the user for the 1st complex number (real and imaginary)
    cout << "1st complex number..." << endl;
    cout << "Enter the real part: ";
    // user input is stored in real
    cin >> real;
    cout << "Enter the imaginary number: ";
    // user input is stored in imaginary
    cin >> imaginary;
    // Instantiate a Complex object using a 2 argument constructor
    Complex first(real, imaginary);

    // call the getter
    first.show();

    // repeat the process for the 2nd complex number
    cout << "2nd complex number..." << endl;
    cout << "Enter the real part: ";
    cin >> real;
    cout << "Enter the imaginary number: ";
    cin >> imaginary;
    Complex second(real, imaginary);
    second.show();

    return 0;
}
