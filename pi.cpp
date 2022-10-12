#include<iostream>
#include<cmath>

// Approximating pi

/**
 * 
 * Probability that a random point chosen on a square with a circle inside of it
 * who's radius is of size side_of_square/2. Can be expressed as (Area of circle)/(Area of square)
 * We also know how to get probability from experiments. Experimentally inferenced probablity will
 * be approximately equal to the real probability.
 * 
 * The following code run N experiments where we use the above 2 ideas to approximate probability
 * and then use the probability to approximate pi.
 * Mathematically this can be expressed as the following:
 * pi approximate =
 *  P(Area of a random point being in the cirlcleon square with circle in between) * squares area / (radius of circle)^2
 * */

int main() {
  long experiments_to_run = 100000000;
  int square_side = 1000;
  int radius_of_circle = square_side/2;
  int radius_of_circle_square = radius_of_circle*radius_of_circle;
  int square_area = square_side*square_side;
  int circle_hits = 0;
  double pi_approx;
  for (int i = 1; i <= experiments_to_run; i++) {
    int shot_x = std::rand() % square_side;
    int shot_y = std::rand() % square_side;
    circle_hits += std::sqrt(shot_y*shot_y + shot_x*shot_x) > radius_of_circle;
    double probabililty_of_rand_in_circle = circle_hits/(double)i;
    pi_approx = (square_area * probabililty_of_rand_in_circle) / radius_of_circle_square;
    std::cout << pi_approx << "\n";
  }
}
