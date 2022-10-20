#include <functional>
#include <iostream>
#include <math.h>
#include <numeric>
#include <vector>

class Point {
  public:
  double x;
  double y;

  Point(double _x = 0, double _y = 0): x(_x), y(_y){}

  double horizontal_displacement_from(Point& point2) const {
    return point2.x - x;
  }
};

// Simple linear regression best line fit as a physics problem
Point calc_centroid(const std::vector<Point>& points) {
  int n = points.size();
  double summation_x = std::accumulate(points.begin(), points.end(), 0, [&](double sum, const Point& curr) {return sum+curr.x; });
  double summation_y = std::accumulate(points.begin(), points.end(), 0, [&](double sum, const Point& curr) {return sum+curr.y; });
  return { summation_x/n, summation_y/n };
}

class RegressionLine {
  Point fulcrum;
  double intercept = 0;
  double regression_coefficient = 0;
  double theta = 90; // angle b/w line and y axis

  double _cot(double x) {
    return cos(x)/sin(x);
  }

  void _calc_and_set_intercept() {
    intercept = fulcrum.y - (regression_coefficient*fulcrum.x);
  }

  /**
   * We mess around with the line by mutating theta between 180 && -180 
   * -ve_y = -180 deg and +ve_y = 180 deg
   * */
  void _adjust_theta(double theta_offset) {
    theta += theta_offset;
    // now adjust the line intercept and regression_coefficient wrt to new theta
    // when we adjust theta what will be our new slope and intercept?
    regression_coefficient = _cot(theta);
    _calc_and_set_intercept();
  }

  double _torque_accumulator(double sum, const Point& curr, bool is_left_of_fulcrum) {
    double predicted_y = predict(curr.x);
    // -ve means curr is on left of fulcrum, +ve means right of fulcrum
    double displacement_from_fulcrum = curr.horizontal_displacement_from(fulcrum);
    if ((displacement_from_fulcrum < 0 && !is_left_of_fulcrum) ||(displacement_from_fulcrum > 0 && is_left_of_fulcrum)) return sum;
    // when force is being pulled up it is +ve and when it is being pulled down it is -ve
    double force = curr.y - predicted_y;
    double dist_from_centroid_magnitude = fabs(displacement_from_fulcrum);
    double torque = force*dist_from_centroid_magnitude * sin(theta);
    return sum + torque;
  };

  public:
  
  void fit(const std::vector<Point>& points, double accuracy_step = 1) {
    fulcrum = calc_centroid(points);
    intercept = fulcrum.y;

    // binary search for till convergence or equilibrium point
    double max_theta = 180;
    double min_theta = -180;
    while (min_theta < max_theta) {
      double mid_theta  = (min_theta + max_theta)/2;
      _adjust_theta(mid_theta);
      
      double left_torque = std::accumulate(points.begin(), points.end(), 0, [&](double sum, const Point& curr) {
        return _torque_accumulator(sum, curr, true);
      });
      double right_torque = std::accumulate(points.begin(), points.end(), 0, [&](double sum, const Point& curr) {
        return _torque_accumulator(sum, curr, false);
      });
      double net_torque = right_torque - left_torque;
      
      // reached equilibrium!!!
      if (net_torque == 0) {
        return;
      }

      if (net_torque < 0) {
        min_theta = mid_theta+accuracy_step;
      } else {
        min_theta = mid_theta-accuracy_step;
      }
    }
  }
  
  double predict(double x) {
    return (regression_coefficient*x) + intercept;
  }

  double get_intercept() {
    return intercept;
  }

  double get_regression_coefficient() {
    return regression_coefficient;
  }
};

int main() {
  std::vector<double> x{2,3,5,7,12,13};
  std::vector<double> y{4,6,9,11,15,17};
  std::vector<Point> points;
  for(int i = 0; i < x.size(); i++) {
    Point p(x.at(i), y.at(i));
    points.push_back(p);
  }

  RegressionLine rl;
  rl.fit(points);

  std::cout << "Trained Linear Regression Model \n";
  std::cout << "Regression Coefficient: " << rl.get_regression_coefficient() << "\n";
  std::cout << "Intercept: " << rl.get_intercept() << "\n";

  return 0;
}
