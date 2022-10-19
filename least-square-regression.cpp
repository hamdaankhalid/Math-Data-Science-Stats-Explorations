#include<functional>
#include <math.h>
#include <numeric>
#include <vector>

// Simple linear regression best line fit as a physics problem

/**
 * 2 routes to visualize:
 *  1. Physics route with vector math on a solid body?
 *  2. iterate on range from farthest to closest element from line and pull
 *    eventually we will have the smallest pull and end state will be expected line
 *    trajectory.
 * */
struct Line {
  double intercept;
  double regression_coefficient;
};

struct Point {
  double x;
  double y;
};

Point calc_centroid(const std::vector<Point>& points) {
  int n = points.size();
  double summation_x = std::accumulate(points.begin(), points.end(), 0, [&](double sum, const Point& curr) {return sum+curr.x; });
  double summation_y = std::accumulate(points.begin(), points.end(), 0, [&](double sum, const Point& curr) {return sum+curr.y; });
  return { summation_x/n, summation_y/n };
}

/**
 * get centroid of points as anchor point
 * perform vector and fulcrum math with anchor point as fulcrum for a initially horizontal line
 * to get a slope in linear time?
 * */
Line lrs(std::vector<Point>& points) {
  auto centroid = calc_centroid(points);
  // create a horizontal line that is parallel to x axis and goes through the centroid y
  double coefficient = 0;
  double intercept = centroid.y;
  double fulcrum = centroid.x;
  bool is_left_of_fulcrum = true;
  std::function<double(double, const Point&)> torque_accumulator = [&fulcrum, &intercept, &is_left_of_fulcrum](double sum, const Point& curr) -> double {
    // -ve means curr is on left of fulcrum, +ve means right of fulcrum
    double dist_from_fulcrum = curr.x - fulcrum;
    if ((dist_from_fulcrum < 0 && !is_left_of_fulcrum) ||(dist_from_fulcrum > 0 && is_left_of_fulcrum)) return sum;
    // here we dont use absolute values because we want to define when force being pulled is +ve and being pulled down
    // is -ve
    double force = curr.y - intercept;
    double dist_from_vector = fabs(dist_from_fulcrum);
    // the funny equation on the right of sum is taking in account for the fact that points on the extreme
    // contribute more to a slope than points closer to the middle, this to me sounds very much like the place
    // to steal and use the concept of torque from physics.
    return sum + (force*dist_from_vector);
  };
  // opposing vectors on left side of fulcrum and vector on right side of fulcrum will represent opposing forces
  double left__of_centroid_vectors_magnitude = std::accumulate(points.begin(), points.end(), 0, torque_accumulator);
  is_left_of_fulcrum = false;
  double right_of_centroid_vectors_magnitude = std::accumulate(points.begin(), points.end(), 0, torque_accumulator);
  double net_tilt = right_of_centroid_vectors_magnitude - left__of_centroid_vectors_magnitude;
  // TODO: Find the translation from net_tile to slope.  
  return {0,0};
}

int main() {
  return 0;
}