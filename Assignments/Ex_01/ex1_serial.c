#include <stdio.h>
#include <stdlib.h>


double approx(double a, double b, double h){
  double result = 0.0;
  while(a < b){
    double x_i = a + h*0.5;
    result += 1.0/(1.0+x_i*x_i);
    a += h;
  }
  result =  h * result;
  return result;
}

int main(int argc, char* argv[]){

  double a = 0.0;
  double b = 1.0;
  double n = 1000000000;
  double h = 1.0/n;
  double pi = 4 * approx(a,b,h);
  printf("pi approximation: %lf\n", pi);

}


