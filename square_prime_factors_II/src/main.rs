use std::env;
use std::collections::HashMap;

// Given a number return the prime factors for it.
// Find the prime factors who's squares divide the target number
// Given a number convert it to scientific notation

fn find_non_obvious_prime_fac(number: i32) -> Option<i32> {
    let upper_range = (number as f64).sqrt() as i32;

    for test in 2..=upper_range {
        if number % test == 0 {
            return Some(test);
        }
    }

    return None;
}

fn prime_factors(number: i32, mut collected_primes: Vec<i32>) -> Vec<i32> {
    let obvious_primes: Vec<i32> = vec![2, 3, 5, 7, 11, 13, 17];
    let potential_divisor = obvious_primes.into_iter().find(|&num| number % num == 0);

    match potential_divisor {
        Some(divisor) => {
            collected_primes.push(divisor);
            let quotient = number / divisor;
            return prime_factors(quotient, collected_primes);
        }
        None => {
            // the number is either a non obvious prime or truly a non prime
            match find_non_obvious_prime_fac(number) {
                Some(prime_fact) => {
                    collected_primes.push(prime_fact);
                    let quotient = number / prime_fact;
                    return prime_factors(quotient, collected_primes);
                }
                None => {
                    collected_primes.push(number);
                    return collected_primes;
                }
            };
        }
    };
}

// Let ck(n) be the number of integers between and inclusive with exactly square prime factors.
fn ck(k: i32, n: i32)-> i32 {
    (1..=n).filter(|num| {
     let primes = prime_factors(*num, vec![]);
    
    let mut frequency_map: HashMap<i32, u32> = HashMap::new();
    for &num in primes.iter() {
        let count = frequency_map.entry(num).or_insert(0);
        *count += 1;
    }
    
    let square_primes: Vec<i32> = frequency_map.iter().filter(|(_, v)| {
        let how_many_times_to_add = *v / 2;
        how_many_times_to_add >= 2
    }).map(|(k, _)| *k).collect();
    
//    println!("Square Primes {:?}", square_primes);

    square_primes.len() == (k as usize) 
    }).collect::<Vec<i32>>().len() as i32
}

fn ck_n_ratio(k: i32, n: i32) {
    for i in 1..=n {
        let ck_calc = ck(k, i) as f64;
        let ratio: f64 = ck_calc / i as f64;
        println!("C k->{} n->{} ck: {}; ratio: {}", k, i, ck_calc, ratio);
    }
}

fn main() {
    // collect args iterator into a vector of strings
    let args: Vec<String> = env::args().collect();

    if args.len() < 2 {
        println!("Usage: {} <number>", args[0]);
    }

    // early exit if parsing errors
    let number = match args[1].parse::<i32>() {
        Ok(num) => num,
        Err(_) => {
            println!("Error: invalid number");
            return;
        }
    };
    
    ck_n_ratio(2, number);
}
