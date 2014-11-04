
#ifndef __SUDOKU_CELL__
#define __SUDOKU_CELL__

class Possible;
class Sudoku;
class CellGrid;

#include "sudoku.h"

class Cell
{
protected:
    CellName                m_name;
    //const Sudoku*           m_sdk;
    const CellGrid*         m_grid;
    Possible*               m_pos;

public:
    Cell(CellName name, const CellGrid *grid);
    virtual ~Cell();
    int Init();
    CellName GetName();
    int ShowPossible();
    Cell& operator=(const Cell& c);
    int SetVal(int val);
    int GetVal();
    int ResetVal(int val);
    Possible* GetPossible();
};

#endif //__SUDOKU_CELL__




