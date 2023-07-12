use csv::Reader;
use rand::seq::SliceRandom;
use rand::Rng;
use serde_derive::{Deserialize, Serialize};
use std::error::Error;
use std::fs::File;

struct Dataframe {
    // Optional headers
    headers: Vec<String>,
    data: Vec<Vec<f32>>,
}

impl Dataframe {
    fn read_from_csv(file_path: &str) -> Result<Self, Box<dyn Error>> {
        let file = File::open(file_path)?;
        let mut reader = Reader::from_reader(file);
        let headers = reader.headers()?.iter().map(|s| s.to_string()).collect();
        let data: Result<Vec<Vec<f32>>, csv::Error> = reader.deserialize().collect();
        // Handle any error that occurred during deserialization
        let data = data.map_err(Box::new)?;
        // Create the Dataframe instance
        let df = Dataframe { headers, data };

        Ok(df)
    }

    fn training_split(&self, training_ratio: f32) -> (Dataframe, Dataframe) {
        let mut rng = rand::thread_rng();

        let mut validation_data: Vec<Vec<f32>> = Vec::new();
        let mut training_data: Vec<Vec<f32>> = Vec::new();

        for row in self.data.clone() {
            let number: f32 = rng.gen();
            if number < training_ratio {
                training_data.push(row);
            } else {
                validation_data.push(row);
            }
        }

        let validation_df = Dataframe {
            headers: self.headers.clone(),
            data: validation_data,
        };
        let training_df = Dataframe {
            headers: self.headers.clone(),
            data: training_data,
        };

        (validation_df, training_df)
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
 *
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
It's worth noting that there are variations of SGD, such as mini-batch SGD, which uses a small fixed-size batch instead of a single training example, and variants that adapt the learning rate dynamically during training to improve convergence speed and stability (e.g., AdaGrad, RMSProp, Adam).
Overall, SGD is a popular and effective optimization algorithm used in various machine learning models to iteratively update the parameters based on a random subset of training data at each step, enabling efficient learning and convergence.
 * */

struct LinearRegression {
    intercept: f32,
    coefficients: Vec<f32>,
}

impl LinearRegression {
    fn predict(&self, independents: Vec<(f32, u32)>) -> Result<f32, String> {
        if independents.len() != self.coefficients.len() {
            return Err(
                "independent vectors length does not match the number of coefficients".to_string(),
            );
        }

        let coefficients_indindependents_sum: f32 = independents
            .iter()
            .zip(self.coefficients.clone())
            .map(|(a, b)| a.0 * b)
            .sum();

        Ok(self.intercept + coefficients_indindependents_sum)
    }
}

// Loss function
fn mean_squared_error(predicted: Vec<f32>, actual: Vec<f32>) -> f32 {
    let squared_sum: f32 = predicted
        .iter()
        .zip(actual)
        .map(|(a, b)| f32::powf(a - b, 2.0))
        .sum();

    (1.0 / predicted.len() as f32) * squared_sum
}

// derivative of loss function used to calculate gradient
fn derivative_mean_squared_error() {}

fn train_linear_regression(
    df: &Dataframe,
    num_coefficients: u32,
    batch_size: usize,
    target_idx: usize,
    max_iterations: u32,
    convergence_threshold: f32,
) -> Result<LinearRegression, String> {
    let mut rand_gen = rand::thread_rng();

    let mut lr = LinearRegression {
        intercept: rand_gen.gen(), // random starting point
        coefficients: Vec::with_capacity(num_coefficients as usize),
    };
    lr.coefficients.fill_with(|| rand_gen.gen()); // random weights array

    // do iterative training
    // while we are not converging or hitting upper threshold do the below
    let mut converged = false;
    let mut current_iteration = 0;

    while !converged && current_iteration < max_iterations {
        let batches = df.get_shuffled_minibatches(batch_size);
        for batch in batches {
            // calculate the current gradient based on the loss function
            let (x, y) = batch.feature_target_extraction(target_idx);

            // update the linear regression model coefficients
        }
        current_iteration += 1;
    }

    Ok(lr)
}

// Driver  Code
#[derive(Debug, Serialize, Deserialize)]
struct Row {
    player: i32,
    power: i32,
    score: f32,
}

fn main() {
    let df = Dataframe::read_from_csv("../data.csv").unwrap();

    for ele in df.data {
        println!("{:?}", ele);
    }
}
