# Stochastic Gradient Descent

Special shoutut to microsoft for letting me work on this during the hackdays!

Fitting a line or any "model" to satsify constraints such as reducing the error between existing data points and those produced by the model is a form of a constraint satisfaction problem. Today we are going to use the concept of partial derivatives to solve the problem of Linear Regression Modelling. Note: We will not let any "buzz words" fly past our heads, we will demystify the concepts till everything just sort of seems like "dumb math".

Before we dive into the algorithm and how it works we  need to clearly understand the concept of derivatives and partial derivatives. Derivatives can be simpilified to be called the "rate of change". When we calculate the rate of change of something we need to understand what is the rate of change in respect to. The easiest way I understood this concept was to think of the concept of speed, the rate of change of speed with respect to time is the acceleration, and so the derivative of speed with respect to time is acceleration. This helped me remember that derivative is a fancy word for rate of change of function with respect to a variable. It is more important to understand the concept rather than the formula for derivatives in this article (and honestly in life too lol).
Now that we know derivatives the next concept we need to understand is partial derivatives. Partial derivatives are used when we have a function with multiple variables, and we want to know what is the rate of change of the function if all other variables are held constant except one.
Let's say we have a function f(x, y) = 55x + y^2 , this function tells us the temperature of your cars engine. the variable y represents outside temperature, and x represents distance. If we wanted to know the relation/sensitivity of the cars engine temperature with respect to the outside temperature we would calculate derivative of f(x,y) with respect to y, which yields 2 * y. This function 2 * y tells us that for each unit change in temperature outside the cars engine temperature will increase by twice as much if distance is held constant.

With derivatives and partial derivatives some what out of our way we can get back to solving linear regression now!
Linear regression can be represented as the following:
f(x0, x1, xi) = intercept + weight0 * x0 + weight1 * x1 + weightI * xi...
Our problem statement for solving linear regression can be put forth as:
Given a set of data points where each dataset is of the form (x0, x1, x2, xi) and corresponding label of (y) find the weights and intercept such that we can construct a line that minimizes the error between what f(x0..xi) produces and the existing points y values. In actual regression modelling you want to do more than just find the perfect fitting line (you need to care about overfitting and what not), but in our case we just want this best fitting line.
We have multiple options for how we calculate the error, one way to calculate the error between the predicted value by the model and the acutal value is using a metric called Mean Squared Error or MSE.MSE = (1/n) * Σ(y - ŷ)²

Our goal is to minimize the MSE created by our linear regression model. We initialize our Linear Regression model with a random intercept and random weights / coefficients. Now we can use the partial derivative of our loss function with respect to each parameter to get information about the rate of change of the loss function as each parameter is changed in isolation. We will collect the partial derivatives with respect to each parameter into a matrix/vector, this matrix of partial derivatives is called the gradient. By calculating the gradient, we can determine the direction and magnitude of the steepest ascent or descent in the loss function. The gradient points in the direction of the greatest increase of the loss function. Therefore, to minimize the loss function and improve the model's performance, we need to update the parameters in the opposite direction of the gradient. The magnitude of the gradient at a specific point indicates how steep the loss function is at that point. A larger magnitude suggests a steeper slope, indicating that a significant adjustment to the parameters is required. Conversely, a smaller magnitude suggests a flatter slope, indicating that the parameters are closer to the optimal values.

The Stochastic Gradient Descent Alogrithm does the above describe gradient descent iteratively on a random selected subset of data points or a single data point to calculate the gradient and update the parameter. The iterations are performed until we hit a certain max number of iterations or the change in parameters becomes neglibible. Each iteration accounts for something called a learning rate when modifying the parameters based on the calculated gradient.


The main algorithm can be seen in the excerpt from the codebase implementing the fitting of a linear regression model in rust.

```
fn train_linear_regression(
    df: &Dataframe,
    num_coefficients: u32,
    batch_size: usize,
    target_idx: usize,
    learning_rate: f32,
    max_iterations: u32,
) -> Result<LinearRegression, String> {
    let initial_random_range = 1.0;

    let mut rand_gen = rand::thread_rng();

    let mut lr = LinearRegression {
        intercept: rand_gen.gen_range(-initial_random_range..initial_random_range),
        coefficients: Vec::with_capacity(num_coefficients as usize),
    };

    // random weights array initially
    lr.coefficients.extend((0..num_coefficients).map(|_| {
        let random: f32 = rand_gen.gen_range(-initial_random_range..initial_random_range);
        random
    }));

    println!("##################");
    println!(
        "Model training loop starting with a maximum iteration set to {}",
        max_iterations
    );
    println!("Initial random weights based model {:?}", lr);
    println!("##################");

    for current_iteration in 0..max_iterations {
        println!("********** EPOCH {} STARTED ***********", current_iteration);

        let batch = df.get_shuffled_minibatch(batch_size);

        println!("iterating over with minibatch {:?}", batch);
        // split features and targets
        let (features, targets) = batch.feature_target_extraction(target_idx);

        assert_eq!(features.len(), targets.len());

        println!(
            "Dataframe Divided Between Features and Target labels -> FEATURES: {:?}, TARGET {:?}",
            features, targets
        );

        let (gradient, intercept_dervative) = calc_gradient(&features, &targets, &lr);

        println!("Calculated Gradient -> {:?}", gradient);

        lr.coefficients
            .iter_mut()
            .zip(gradient.iter())
            .for_each(|(coeff, grad)| {
                *coeff -= grad * learning_rate;
            });

        lr.intercept -= intercept_dervative * learning_rate;

        println!("Updated Model -> {:?}", lr);

        println!(
            "********** EPOCH {} SUMMARY ************",
            current_iteration
        );
        println!("Model adjusted: {:?}", lr);
        println!("********* EPOCH {} COMPLETED **********", current_iteration);

        if lr.intercept.is_nan() || lr.coefficients.iter().any(|x| x.is_nan()) {
            return Err("Numbers went to NAN, early exiting".to_string());
        }
    }

    println!("------ Model Training Completed --------");

    Ok(lr)
}

```
