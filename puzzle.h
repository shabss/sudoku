

#ifndef __SUDOKU_PUZZLE__
#define __SUDOKU_PUZZLE__

#include <stdio.h>

class Puzzle
{
protected:
    char* m_Matrix;
    int m_Cols;
    int m_Rows;

public:
    Puzzle();
    virtual ~Puzzle();

    virtual int Init(char *fn);
    virtual int Show();
    int GetRowsCount(); 
    int GetColsCount();
    const char* GetMatrix();
    int GetVal(int r, int c);
};

#endif //__SUDOKU_PUZZLE__
