#include "calc.h"

double calc_add(double a, double b) { return a + b; }

double calc_sub(double a, double b) { return a - b; }

double calc_mul(double a, double b) { return a * b; }

double calc_div(double a, double b, int *error) {
  if (b == 0.0) {
    if (error)
      *error = 1;
    return 0.0;
  }
  if (error)
    *error = 0;
  return a / b;
}
