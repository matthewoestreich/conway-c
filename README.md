# conway-c

[Conway game of life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) in C with Raylib.

**Rules of Conway's Game of Life**

- Any live cell with fewer than two live neighbors dies, as if caused by underpopulation.
- Any live cell with two or three live neighbors lives on to the next generation.
- Any live cell with more than three live neighbors dies, as if by overpopulation.

# Build

**Only works on MacOS as of now** (feel free to download the correct Raylib version and modify the build script, though)

```bash
# From project root

# Build project
$ ./build.sh
# Run executable
$ ./build/main
```

# Usage

- Press `SPACE` to pause the simulation
- Draw using mouse (press and hold left mouse button, move mouse around)
- Press `SPACE` again to resume the simulation

# Design

- Bit Manipulation
  - Our grid uses 'cell bit manipulation' to handle calculating future generations.
  - This means we only need to allocate once, not every single frame.
  - Each cell is an `unsigned char` with only the first two bits being used.
    - If a bit is `0` it means that cell is dead; `1` means alive.
    - The first bit represents the current generation of that cell.
    - The second bit represents the next generation of that cell.

# Screenshots

[Gospers Gun](<https://en.wikipedia.org/wiki/Gun_(cellular_automaton)>)

<img width="1199" height="828" alt="gosper" src="https://github.com/user-attachments/assets/ea61ce22-456a-491c-a246-87c7cfe2d323" />
