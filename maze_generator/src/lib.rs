use rand::{thread_rng, Rng};

#[derive(Debug, PartialEq)]
pub struct MazeCell {
    x: u32,
    y: u32,

    north: bool,
    south: bool,
    east: bool,
    west: bool,
}

pub fn generate_maze(width: u32, height: u32) -> Vec<MazeCell> {
    let mut cells: Vec<MazeCell> = vec![];
    cells.reserve((width * height) as usize);

    let mut unvisited: Vec<usize> = vec![];
    unvisited.reserve((width * height) as usize);

    for y in 0..height {
        for x in 0..width {
            let cell = MazeCell {
                x,
                y,
                north: false,
                south: false,
                east: false,
                west: false
            };
            cells.push(cell);
            unvisited.push(cells.len() - 1);
        }
    }

    let mut rng = thread_rng();
    let _ = unvisited.remove(rng.gen_range(0..unvisited.len()));

    while !unvisited.is_empty() {
        let mut cell = unvisited[rng.gen_range(0..unvisited.len())];
        let mut path = vec![cell];

        let get_neighbors = |cell: usize| -> Vec<usize> {
            let cell = cell as isize;
            let x = cell % width as isize;
            let width = width as isize;

            let mut neighbors = vec![];

            if cell - width >= 0 {
                neighbors.push((cell - width) as usize);
            }

            if cell + width < cells.len() as isize {
                neighbors.push((cell + width) as usize);
            }
            
            if x - 1 > 0 && cell - 1 >= 0{
                neighbors.push((cell - 1) as usize);
            }

            if x + 1 < width && cell + 1 < cells.len() as isize {
                neighbors.push((cell + 1) as usize);
            }
            
            neighbors

        };

        while unvisited.contains(&cell) {
            let neighbors = get_neighbors(cell);
            cell = neighbors[rng.gen_range(0..neighbors.len())];
            
            if path.contains(&cell) {
                path = path.into_iter().take_while(|el| *el != cell).collect();
            }
            path.push(cell);
        }

        let mut link_cells = |index1: usize, index2: usize| {
            let cell1 = cells.get(index1).unwrap();
            let cell2 = cells.get(index2).unwrap();

            let dx = cell2.x as i32 - cell1.x as i32;
            let dy = cell2.y as i32 - cell1.y as i32;

            if dx == 1 && dy == 0 {
                let cell1 = cells.get_mut(index1).unwrap();
                cell1.east = true;
                let cell2 = cells.get_mut(index2).unwrap();
                cell2.west = true;
            } else if dx == -1 && dy == 0 {
                let cell1 = cells.get_mut(index1).unwrap();
                cell1.west = true;
                let cell2 = cells.get_mut(index2).unwrap();
                cell2.east = true;
            } else if dx == 0 && dy == 1 {
                let cell1 = cells.get_mut(index1).unwrap();
                cell1.south = true;
                let cell2 = cells.get_mut(index2).unwrap();
                cell2.north = true;
            } else if dx == 0 && dy == -1 {
                let cell1 = cells.get_mut(index1).unwrap();
                cell1.north = true;
                let cell2 = cells.get_mut(index2).unwrap();
                cell2.south = true;
            }
        };

        for i in 0..path.len() - 1 {
            link_cells(path[i], path[i + 1]);
            if let Some(index) = unvisited.iter().position(|el| el == &path[i]) {
                unvisited.remove(index);
            }
        }
    }

    cells
}

pub fn convert_maze(maze: Vec<MazeCell>, width: u32, height: u32) -> Vec<bool> {
    let mut cells = vec![false; (width * 3 * height * 3) as usize];
    let width = width as usize;
    let height = height as usize;

    for y in 0..height {
        for x in 0..width {
            if let Some(cell) = maze.get(x + y * width) {

                let x_o = x as usize * 3;
                let y_o = y as usize * 3;
                let w_o = width * 3;

                // Set center to walkable
                cells[x_o + 1 + (y_o + 1) * w_o] = true;

                // Set perpendicular tiles depending on the connectons
                if cell.north {
                    cells[x_o + 1 + y_o * w_o] = true;
                }

                if cell.south {
                    cells[x_o + 1 + (y_o + 2) * w_o] = true;
                }

                if cell.east {
                    cells[x_o + 2 + (y_o + 1) * w_o] = true;
                }

                if cell.west {
                    cells[x_o + (y_o + 1) * w_o] = true;
                }
            }
        }
    }

    cells
}
