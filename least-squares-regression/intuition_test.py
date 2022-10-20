import matplotlib.pyplot as plt
import numpy as np
from sklearn import datasets, linear_model
from sklearn.metrics import mean_squared_error, r2_score

def centroid_calc(arr_x, arr_y):
  return (np.sum(arr_x)/len(arr_x), np.sum(arr_y)/len(arr_y))

def linear_regr_visual_test():
  # Load dataset
  x = np.array([2,3,5,7,12,13]).reshape(-1, 1)
  y = np.array([4,6,9,11,15,17]).reshape(-1, 1)

  # Create linear regression object
  regr = linear_model.LinearRegression()

  # Train the model using the training sets
  regr.fit(x, y)

  # Make predictions using the testing set
  pred = regr.predict(x)

  # The coefficients
  print("Coefficients: \n", regr.coef_)

  # Intercept
  print("Intercept: \n", regr.intercept_)

  # The mean squared error
  print("Mean squared error: %.2f" % mean_squared_error(y, pred))
  # The coefficient of determination: 1 is perfect prediction
  print("Coefficient of determination: %.2f" % r2_score(y, pred))

  centroid = centroid_calc(x, y)
  expected = centroid[1]
  print(f"Centroid cooridinates: {centroid}")
  returned = regr.predict(np.array(centroid[0]).reshape(1, -1))
  print(f"TEST HYPOTHESIS: centroid always lies on the line: Expect {expected} : Returned {returned}")

  # Plot outputs
  plt.scatter(x, y, color="black")
  plt.plot(x, pred, color="blue", linewidth=3)

  plt.xticks(())
  plt.yticks(())

  plt.show()

linear_regr_visual_test()