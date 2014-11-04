

#ifndef __SUDOKU__
#define __SUDOKU__

#include <vector>
#include <map>

#define SDKERR_SUCCESS          0
#define SDKERR_UNKNOWN          -1
#define SDKERR_INVALID_PARAM    -2
#define SDKERR_PROGRAM_ERROR    -3
#define SDKERR_CONSTRAINT       -4

#define SDKOPT_QUITE        0x01
#define SDKOPT_VERBOSE      0x02
#define SDKOPT_TEST         0x04

//Support for only 9x9 matrix
#define SDK_DIMENSION           9
#define SDK_GRID_DIMENSION      3
#define SDK_PEERS               20


class Puzzle;
class Cell;
class CellGrid;

#define MK_NAME(row, col) ((row << 8) | col)
#define GET_ROW(cname) ((cname >> 8) & 0xFF)
#define GET_COL(cname) (cname  & 0xFF)

typedef unsigned short CellName;
typedef std::vector<CellName> Peers;
typedef std::map<CellName, Peers*> CellPeers;
//typedef std::map<CellName, Cell*> CellGrid;

typedef struct SDK_INFO_ {
    unsigned int opts;
} SDK_INFO;

class Sudoku 
{

private:

protected:
    Puzzle*         m_Puzzle;
    CellPeers*      m_CellPeers;
    Cell**          m_PickCellsLookup;
public:
    SDK_INFO*       m_Info;

public:
    static Sudoku* Create(Puzzle* p, SDK_INFO* info);
    Sudoku(Puzzle* p, SDK_INFO* info);
    virtual ~Sudoku();
    
    int Init();
    const Puzzle* GetPuzzle();
    int GetDim();
    int MakePeers();
    int MakeRowPeers(CellName name, Peers* peers);
    int MakeColPeers(CellName name, Peers* peers);
    int MakeGridPeers(CellName name, Peers* peers);
    int ShowPeers();


    CellGrid* MakeCellGrid();
    CellGrid* CloneCellGrid(CellGrid *grid);
    int ShowCellGrid(char* msg, CellGrid *grid);

    int PlacePuzzle(CellGrid *grid);
    CellGrid* Solve();
    int Solved(CellGrid *grid);
    const Cell* PickCell(CellGrid *grid);
    int Show();
    CellGrid* Search(CellGrid *grid);
    int WriteSolution(CellGrid *grid, char* file);

    const Peers* GetPeers(CellName name);
};

#endif //__SUDOKU__
