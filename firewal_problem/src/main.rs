use std::fs::File;
use std::io::{BufRead, BufReader};

fn main() {
    let file = File::open("puzzle-input.txt").unwrap();
    
    let reader = BufReader::new(file);
    
    let mut excluded_ranges: Vec<(i64, i64)> = Vec::new();

    for line in reader.lines(){
        let text_line = line.unwrap();
        // create the tuple and store tuple in vector
        let parts: Vec<&str> = text_line.split("-").collect();

        let start = parts[0].trim().parse::<i64>().unwrap();
        let end = parts[1].trim().parse::<i64>().unwrap();

        let range = (start, end);
        excluded_ranges.push(range);
    }


    // sort the vector based on the first item in each elements tuple
    excluded_ranges.sort_by_key(|tuple| tuple.0);
    
    // now set the smallest number to 0 and as we iterate keep updating the smallest number
    
    let mut smlst_num = 0;

    for range in excluded_ranges.iter() {
        let (start, end) = range;
        // if range contains the smallest number then bump it up
        if smlst_num >= *start && smlst_num <= *end {
            smlst_num = end+1;
        }
    }

    println!("{}", smlst_num);
}
