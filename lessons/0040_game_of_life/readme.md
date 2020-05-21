# Game of Life
The classical [Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) is a sort of mathematical "game" invented by John Conway in the seventies, that it will be used to show how to record information inside a matrix about the image to be displayed.

In a nutshell, there is a NxN matrix with 0 and 1s in each cell, where 0 mean *dead* and 1 mean *alive* (sometimes they are referred as *unpopulated* and *populated*). This matrix evolves with discrete steps, updating every cell at each step using these simple rules:
- Any live cell with fewer than two live neighbors dies, as if by underpopulation.
- Any live cell with two or three live neighbors lives on to the next generation.
- Any live cell with more than three live neighbors dies, as if by overpopulation.
- Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.

As a result, there is a feeling of moving things, because of the continuing change in cell values, as it can see in the below video

[![YouTube video on Game of Life](http://img.youtube.com/vi/OWfAzvJidVQ/0.jpg)](http://www.youtube.com/watch?v=OWfAzvJidVQ "YouTube video on Game of Life")

The code is quite simple, there are four functions that done all the job:

1. ```void start(int m[][DIM])```: populate all the matrix with 0 and 1s, using the the `rand()` function to choose between 0 and 1, with a ratio of 6 to 1, to avoid to overpopulate the matrix
2. ```void draw(int m[][DIM])```: put the entire matrix to the screen with the ```draw_filled_rect``` function. The coordinates of each cell depends on its *i, j* position, multiplicated for **CELL_DIMENSION**, in this case 8, to have cell that are clearly visible on the screen. Only cells with 1 inside are put on the screen, red coloured.
3. ```void evolve(int m[][DIM])```: this is the main function, which is in charge of evolving the matrix state using the rules explained above. 
In particular there are four *if*, each one matching one rule
```c
int n = neighbours(m, i, j);
//Any live cell with fewer than two live neighbors dies, 
//as if by underpopulation.
if (m[i][j] == 1 && n < 2)
	temp[i][j] = 0;
//Any live cell with two or three live neighbors lives 
//on to the next generation.
if (m[i][j] == 1 && (n == 2 || n == 3))
	temp[i][j] = 1;
//Any live cell with more than three live neighbors dies, 
//as if by overpopulation.
if (m[i][j] == 1 && n > 3)
	temp[i][j] = 0;
//Any dead cell with exactly three live neighbors 
//becomes a live cell, as if by reproduction.
if (m[i][j] == 0 && n == 3)
	temp[i][j] = 1;
```
4. ```int neighbours(int m[][DIM], int r, int c)```: a little utility function to count how many neighbours cells are near to cell **m[r][c]**

Using all these functions is now easy to implement the main loop, drawing and evolving the state at each step.