fn main() {
    cxx_build::bridge("src/lib.rs").compile("maze_gen");
}
