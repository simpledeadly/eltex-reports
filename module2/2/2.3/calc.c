#include "calc.h"

double add(double a, double b) { return a + b; }
double subtract(double a, double b) { return a - b; }
double multiply(double a, double b) { return a * b; }
double divide(double a, double b) { return a / b; }

double (*operations[4])(double, double) = {add, subtract, multiply, divide};
