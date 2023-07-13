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

    // returns 2 data frames with data copied into the two dataframes based on the given split
    // ratio. This does not mutate the dataframe itself, which is why I heavily rely on cloning
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

    // Divide the data frame into small shuffled chunks of the original data held in the orignal
    // data frame. This also uses cloning to not affect the existing data structure.
    fn get_shuffled_minibatches(&self, batch_size: usize) -> Vec<Dataframe> {
        let mut clone_and_shuffle = self.data.clone();
        clone_and_shuffle.shuffle(&mut rand::thread_rng());

        let subset = clone_and_shuffle[0..3].to_vec();

        subset
            .chunks(batch_size)
            .map(|chunk| Dataframe {
                headers: self.headers.clone(),
                data: chunk.to_vec(),
            })
            .collect()
    }

    /*
     * Extract the data into vector of independent variables and a vector of dependent variables
     * Given the index of the target variable we pretty much extract that one column out into it's
     * own vector, and return new vector of the subset of records that are the features
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
    // encapsulates the logic for getting the dependent variable/target given the intercept and
    // coefficients for a linear regression model
    fn predict(&self, independents: &Vec<f32>) -> f32 {
        // dot product :) on coefficients
        let coefficients_independents_sum: f32 = independents
            .iter()
            .zip(&self.coefficients)
            .map(|(a, b)| a * b)
            .sum();

        self.intercept + coefficients_independents_sum
    }
}

// partial derivative of loss function of RSS wrt each parameter and with no regard to linear or constant stuff
// partial derivative measures the rate of change of a function with multiple variables
// when all variables are constant except one is changed
fn calc_gradient(x: &Vec<Vec<f32>>, y: &Vec<f32>, lr: &LinearRegression) -> (Vec<f32>, f32) {
    let num_examples = x.len() as f32;
    let num_features = x[0].len();

    let mut gradient = vec![0.0; num_features];
    let mut gradient_intercept = 0.0;

    for i in 0..x.len() {
        let prediction = lr.predict(&x[i]);
        let actual = y[i];
        let error = prediction - actual;

        println!(
            "Predicted: {}, Actual: {}, Error: {}",
            prediction, actual, error
        );

        for j in 0..num_features {
            gradient[j] += error * x[i][j];
        }

        gradient_intercept += error;
    }

    for j in 0..num_features {
        gradient[j] *= 2.0 / num_examples;
    }

    gradient_intercept *= 2.0 / num_examples;

    (gradient, gradient_intercept)
}

// Main SGD algorithm happens here
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
        intercept: rand_gen.gen_range(-0.5..0.5),
        coefficients: Vec::with_capacity(num_coefficients as usize),
    };

    // random weights array initially
    lr.coefficients.extend((0..num_coefficients).map(|_| {
        let random: f32 = rand_gen.gen_range(-0.5..0.5);
        random
    }));

    println!("##################");
    println!(
        "Modle training loop starting with a maximum iteration set to {}",
        max_iterations
    );
    println!("Initial random weights based model {:?}", lr);
    println!("##################");

    for current_iteration in 0..max_iterations {
        println!("********** EPOCH {} STARTED ***********", current_iteration);
        df.get_shuffled_minibatches(batch_size)
            .iter()
            .for_each(|batch| {
                println!("iterating over with minibatch {:?}", batch);
                // split features and targets
                let (features, targets) = batch.feature_target_extraction(target_idx);
                
                assert_eq!(features.len() , targets.len());
                
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
            });

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

// Driver code

fn main() {
    let df = Dataframe::read_from_csv("../data.csv").unwrap();

    let (test_df, _) = df.training_split(1.0);

    let linear_regression_model = train_linear_regression(&test_df, 1, 5, 1, 0.001, 50);

    println!("{:?}", linear_regression_model);
}

#[cfg(test)]
mod tests {
    use crate::LinearRegression;

    #[test]
    fn test_linear_regression_predict() {
        let zero_line = LinearRegression {
            intercept: 0.0,
            coefficients: vec![0.0],
        };

        assert_eq!(0.0, zero_line.predict(&vec![1.2]));

        let origin_to_one_line = LinearRegression {
            intercept: 0.0,
            coefficients: vec![1.0],
        };

        assert_eq!(1.0, origin_to_one_line.predict(&vec![1.0]));

        let example_line = LinearRegression {
            intercept: 0.72001684,
            coefficients: vec![0.47640133],
        };

        assert_eq!(952.0935, example_line.predict(&vec![1997.0]));
    }
}