sudoku
======

Sudoku solver in C++


To Do:
=========
1. Simplify design
   1. Each cell should be an 16 bit int with LSB 9 bits representing possible values
      1. Optionally the 7 MSB can contain the value if it speeds up stuff
         1.  Or maybe 4 MSB has the value giving 12 bits for possible values 
   1. CellGrid clone should be simply be memcpy
   1. Peers can also simply be an array of integers
1. Use bit hacks to optimize performance: http://graphics.stanford.edu/~seander/bithacks.html
1. The GellGrid::SetCellVal() should recusively call itself when there is no other choice for cell value
1. Many other changes
