#include <iostream>
#include <stdexcept>


/**
 * 
 * Pseudo random generator based on equation:
 * Xi+1 = (a*Xi + c) % m;
 * m > 0
 * a (0, m)
 * c (0, m)
 * X0 [0, m) seed
 * 
 * */
class LcpRand {
  int x;
  int a;
  int m;
  int c;

  public:
  
  LcpRand(int seed, int mulitplier, int increment, int modulus) {
    if (modulus < 0) {
      throw std::invalid_argument("modulus has to be greater than 0");
    }
    if (mulitplier > modulus || mulitplier <= 0) {
      throw std::invalid_argument("multiplier range: (0, m)");
    }
    if (increment > modulus || increment <= 0) {
      throw std::invalid_argument("increment range: (0, m)");
    }
    if (seed > modulus || seed < 0) {
      throw std::invalid_argument("seed range: [0, m)");
    }
    x = seed;
    a = mulitplier;
    m = modulus;
    c = increment;
  }

  int rand() {
    x = ( (a*x) + c ) % m;
    return x;
  }
};

int main() {
  LcpRand lcpr(5, 3, 3, 7);
  
  for (int i = 0; i < 10; i++) {
    std::cout << lcpr.rand() << "\n";
  }
}