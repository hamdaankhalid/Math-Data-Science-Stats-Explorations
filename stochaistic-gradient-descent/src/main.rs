use csv::Reader;
use rand::seq::SliceRandom;
use rand::Rng;
use std::error::Error;
use std::fs::File;
use std::num::ParseFloatError;

#[derive(Debug)]
struct Dataframe {
    // Optional headers
    headers: Vec<String>,
    data: Vec<Vec<f32>>,
}

impl Dataframe {
    fn read_from_csv(file_path: &str) -> Result<Self, Box<dyn Error>> {
        let file = File::open(file_path)?;
        let mut reader = Reader::from_reader(file);

        let headers = reader
            .headers()?
            .iter()
            .map(|s| s.to_string())
            .collect::<Vec<String>>();
        let records: Result<Vec<csv::StringRecord>, csv::Error> = reader.records().collect();

        let records = records.map_err(Box::new)?;

        let data: Result<Vec<Vec<f32>>, Box<ParseFloatError>> = records
            .iter()
            .map(|record| {
                record
                    .iter()
                    .map(|value| value.parse::<f32>().map_err(Box::new))
                    .collect::<Result<Vec<f32>, Box<ParseFloatError>>>()
            })
            .collect();

        let data = data?;

        let df = Dataframe { headers, data };

        Ok(df)
    }

    fn training_split(&self, training_ratio: f32) -> (Dataframe, Dataframe) {
        let mut rng = rand::thread_rng();

        let mut validation_data: Vec<Vec<f32>> = Vec::new();
        let mut training_data: Vec<Vec<f32>> = Vec::new();

        self.data.iter().for_each(|row| {
            let number: f32 = rng.gen();
            if number < training_ratio {
                training_data.push(row.clone());
            } else {
                validation_data.push(row.clone());
            }
        });

        let validation_df = Dataframe {
            headers: self.headers.clone(),
            data: validation_data,
        };

        let training_df = Dataframe {
            headers: self.headers.clone(),
            data: training_data,
        };

        (training_df, validation_df)
    }

    fn get_shuffled_minibatches(&self, batch_size: usize) -> Vec<Dataframe> {
        let mut clone_and_shuffle = self.data.clone();
        clone_and_shuffle.shuffle(&mut rand::thread_rng());

        clone_and_shuffle
            .chunks(batch_size)
            .map(|chunk| Dataframe {
                headers: self.headers.clone(),
                data: chunk.to_vec(),
            })
            .collect()
    }

    /*
     * Extract the data into vector of independent variables and a vector of dependent variables
     * */
    fn feature_target_extraction(&self, target_index: usize) -> (Vec<Vec<f32>>, Vec<f32>) {
        let mut features: Vec<Vec<f32>> = Vec::new();
        let mut target: Vec<f32> = Vec::new();
        self.data.iter().for_each(|record| {
            let mut cleaned_record = record.clone();
            let target_extracted = cleaned_record.remove(target_index);
            features.push(cleaned_record);
            target.push(target_extracted);
        });

        (features, target)
    }
}

// Actual algorithm

/*
Stochastic Gradient Descent (SGD) is an optimization algorithm commonly used in machine learning for training models, especially in large-scale or online learning scenarios. It is an extension of the standard Gradient Descent algorithm that uses a random subset of the training data at each iteration.
Here is a high-level overview of the Stochastic Gradient Descent algorithm:
Initialize the model parameters: Start by initializing the model parameters (weights and biases) with some random values.
Shuffle and partition the training data: Randomly shuffle the training data and divide it into smaller subsets called mini-batches. The mini-batches are used to update the model parameters iteratively.
Iterate over mini-batches: For each mini-batch, perform the following steps:
a. Compute the gradient: Evaluate the gradient of the loss function with respect to the model parameters using the current mini-batch. The gradient indicates the direction of steepest ascent.
b. Update the parameters: Update the model parameters by taking a step in the opposite direction of the gradient. The learning rate determines the step size. The update rule typically used in SGD is:
parameter = parameter - learning_rate * gradient
The learning rate determines the size of the steps taken during parameter updates. It is usually a small positive value.
Repeat until convergence: Repeat Steps 3a and 3b for a fixed number of iterations or until a convergence criterion is met. Convergence is typically determined by monitoring the decrease in the loss function or the change in the model parameters.
The key difference between SGD and standard Gradient Descent is that SGD updates the model parameters using a random subset of the training data (mini-batch) rather than the entire dataset. This randomness introduces noise, which can help the algorithm escape from local optima and reach faster convergence, especially in high-dimensional or noisy datasets.
 * */

#[derive(Debug)]
struct LinearRegression {
    intercept: f32,
    coefficients: Vec<f32>,
}

impl LinearRegression {
    fn predict(&self, independents: &Vec<f32>) -> f32 {
        // dot product :)
        let coefficients_indindependents_sum: f32 = independents
            .iter()
            .zip(self.coefficients.clone())
            .map(|(a, b)| a * b)
            .sum();

        self.intercept + coefficients_indindependents_sum
    }
}

fn calc_gradient(x: &Vec<f32>, y: f32, lr: &LinearRegression) -> (Vec<f32>, f32) {
    let predicted_value = lr.predict(x);

    println!(
        "Predicted Value: {} with LinearRegression model {:?}",
        predicted_value, lr
    );

    let parameter_gradients = x
        .iter()
        .map(|x_i| {
            // derivative of mean squared error with respect to each parameter
            2.0 * (predicted_value - y) * x_i
        })
        .collect();

    let intercept_gradient = 2.0 * (predicted_value - y);

    (parameter_gradients, intercept_gradient)
}

fn train_linear_regression(
    df: &Dataframe,
    num_coefficients: u32,
    batch_size: usize,
    target_idx: usize,
    learning_rate: f32,
    max_iterations: u32,
) -> Result<LinearRegression, String> {
    let mut rand_gen = rand::thread_rng();

    let mut lr = LinearRegression {
        intercept: rand_gen.gen(), // random starting point
        coefficients: Vec::with_capacity(num_coefficients as usize),
    };

    // random weights array
    lr.coefficients.extend((0..num_coefficients).map(|_| {
        let random: f32 = rand_gen.gen();
        random
    }));

    println!("Initial random based model {:?}", lr);

    let mut current_iteration = 0;

    while current_iteration < max_iterations {
        println!("EPOCH {} STARTED", current_iteration);
        df.get_shuffled_minibatches(batch_size)
            .iter()
            .for_each(|batch| {
                println!("iterating over with minibatch {:?}", batch);
                // split features and targets
                let (features, targets) = batch.feature_target_extraction(target_idx);

                println!(
                    "Dataframe Divided -> FEATURES: {:?}, TARGET {:?}",
                    features, targets
                );
                assert!(features.len() == targets.len());

                // calculate the current gradient based on the loss function
                features.iter().zip(targets).for_each(|(feature, target)| {
                    // compute gradient
                    let (gradient, intercept_dervative) = calc_gradient(feature, target, &lr);

                    assert!(gradient.len() == lr.coefficients.len());

                    println!("Gradient: {:?}", gradient);

                    // update parameters based on gradient
                    lr.coefficients = lr
                        .coefficients
                        .iter()
                        .zip(gradient)
                        .map(|(coefficient, derivative)| coefficient - learning_rate * derivative)
                        .collect();

                    // update the intercept
                    lr.intercept = lr.intercept - learning_rate * intercept_dervative;

                    println!("Updated Model -> {:?}", lr);
                });
            });
        println!("Model adjusted: {:?}", lr);
        println!("EPOCH {} COMPLETED", current_iteration);
        current_iteration += 1
    }

    Ok(lr)
}

// Driver code

fn main() {
    let df = Dataframe::read_from_csv("../data.csv").unwrap();

    let (test_df, _) = df.training_split(1.0);

    let linear_regression_model = train_linear_regression(&test_df, 2, 3, 2, 0.001, 10);

    println!("{:?}", linear_regression_model);
}
