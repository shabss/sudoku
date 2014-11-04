
#ifndef __SUDOKU_CELLGRID__
#define __SUDOKU_CELLGRID__

class Cell;
class Sudoku;

class CellGrid
{
protected:
    int m_Dim;
    std::map<CellName, Cell*> *m_Grid;
    Sudoku* m_sdk;

public:
    CellGrid(int dim, Sudoku* sdk);
    virtual ~CellGrid();
    int Init();                 //vanilla grid
    int Init(CellGrid* grid);   //clones from another grid
    int DeleteInternalGrid();
    int GetDim();
    int Show();
    int ShowCSV();
    int SetCellVal(CellName cname, int val);
    const Cell* GetCell(CellName cname);
    int Solved();

};



#endif //__SUDOKU_CELLGRID__