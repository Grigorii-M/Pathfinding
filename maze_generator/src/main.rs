use maze_generator::*;

fn main() {
    const WIDTH: u32 = 10;
    const HEIGHT: u32 = 10;
    let output = generate_maze(WIDTH, HEIGHT);

    for y in 0..HEIGHT as usize {
        for x in 0..WIDTH as usize {
            print!("{:?} ", output[x + y * WIDTH as usize]);
        }
        println!("\n");
    }

    let output = convert_maze(output, WIDTH, HEIGHT);

    let mut count = 0;
    for i in output {
        print!("{}", if i { '.' } else { 'X' });
        count += 1;
        if count == WIDTH * 3 {
            count = 0;
            println!();
        }
    }
}
