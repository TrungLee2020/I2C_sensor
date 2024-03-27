// Standard library
use std::io;
fn main() {
    let user_input = input_string();

    println!("{}",user_input);
    
}

fn input_string() -> String
{
    let mut input = String::new();

    io::stdin()
        .read_line(&mut input)
        .expect("not comment");

    input
}