


#include "sudoku.h"
#include "puzzle.h"
#include "cell.h"
#include "cellgrid.h"
#include "possible.h"

#include <algorithm>    // std::find
#include <assert.h>
#include <conio.h>

Sudoku* Sudoku::Create(Puzzle* p, SDK_INFO* info) {
    Sudoku* s = new Sudoku(p, info);    
    return s;
}

Sudoku::Sudoku(Puzzle *p, SDK_INFO* info)
{
    m_Puzzle = p;
    m_CellPeers = new CellPeers();
    assert(m_CellPeers->empty());
    m_PickCellsLookup = NULL;
    m_Info = info;
}

Sudoku::~Sudoku() {
    if (m_CellPeers) {
        CellPeers::iterator it = m_CellPeers->begin();
        for (; it != m_CellPeers->end(); ++it) {
            Peers* p = (*it).second;
            (*it).second = NULL;
            delete p;
        }
        delete m_CellPeers;
        m_CellPeers = NULL;
    }
    if (m_PickCellsLookup) {
        delete [] m_PickCellsLookup;
        m_PickCellsLookup = NULL;
    }
}

int Sudoku::Init() 
{
    int rv = SDKERR_SUCCESS;
    rv = MakePeers();
    if (rv != SDKERR_SUCCESS) {
        printf("Sudoku: Error making peers\n");
        return rv;
    }
    if (m_Info->opts & SDKOPT_VERBOSE) {
        ShowPeers();
    }
    return SDKERR_SUCCESS;
}


int Sudoku::MakePeers()
{
    int rv = SDKERR_SUCCESS;
    if (!m_CellPeers->empty()) {
        return SDKERR_SUCCESS;
    }

    int dim = GetDim();
    if (dim < 1) {
        printf("Sudoku: Invalid dimensions\n");
        return SDKERR_INVALID_PARAM;
    }

    for (int r = 1; r <= dim; r++) {
        for (int c = 1; c<= dim; c++) {
            CellName cname = MK_NAME(r, c);
            Peers *peers = new Peers();
            rv = MakeRowPeers(cname, peers);
            rv = MakeColPeers(cname, peers);
            rv = MakeGridPeers(cname, peers);
            (*m_CellPeers)[cname] = peers;        
        }
    }

    return SDKERR_SUCCESS;
}

int Sudoku::MakeRowPeers(CellName cname, Peers* peers)
{
    if (!peers) {
        return SDKERR_INVALID_PARAM;
    }
    
    //to do: Check for valid CellName
    int r = GET_ROW(cname);
    int c = GET_COL(cname);
    int dim = GetDim();

    //To do: Check if peer is not already present
    for (int pr = 1; pr <= dim; pr++) {
        CellName pname = MK_NAME(pr, c);
        if (pname != cname) {
            peers->push_back(pname);
        }
    }

    return SDKERR_SUCCESS;
}

int Sudoku::MakeColPeers(CellName cname, Peers* peers)
{
    if (!peers) {
        return SDKERR_INVALID_PARAM;
    }
    
    //to do: Check for valid CellName
    int r = GET_ROW(cname);
    int c = GET_COL(cname);
    int dim = GetDim();

    //To do: Check if peer is not already present
    for (int pc = 1; pc <= dim; pc++) {
        CellName pname = MK_NAME(r, pc);
        if (pname != cname) {
            peers->push_back(pname);
        }
    }

    return SDKERR_SUCCESS;
}

int Sudoku::MakeGridPeers(CellName cname, Peers* peers)
{
    int griddim = SDK_GRID_DIMENSION;

    int r = GET_ROW(cname);
    int c = GET_COL(cname);

    //get grid coordinates
    int gr = ((r - 1) / griddim) + 1;
    int gc = ((c - 1) / griddim) + 1;

    //get row and col start coordinates
    int rs = ((gr - 1) * griddim) + 1;
    int cs = ((gc - 1) * griddim) + 1;

    for (int pr = rs; pr < rs + griddim; pr++) {
        for (int pc = cs; pc < cs + griddim; pc++) {
            CellName pname = MK_NAME(pr, pc);
            if ((pname != cname) &&
                (std::find(peers->begin(), peers->end(), pname) == peers->end())) {
                peers->push_back(pname);
            }
        }
    }
    return SDKERR_SUCCESS;
}



CellGrid* Sudoku::MakeCellGrid() 
{
    CellGrid *grid = new CellGrid(GetDim(), this);
    grid->Init();
    return grid;
}

CellGrid* Sudoku::CloneCellGrid(CellGrid *source)
{
    CellGrid *grid = new CellGrid(GetDim(), this);
    grid->Init(source);
    return grid;
}


int Sudoku::ShowCellGrid(char* msg, CellGrid *grid)
{
    printf(msg);
    return grid->Show();
}

int Sudoku::PlacePuzzle(CellGrid *grid)
{
    int rv = SDKERR_SUCCESS;
    int dim = GetDim();

    for (int r = 1; r <= dim; r++) {
        for (int c = 1; c <= dim; c++) {
            int val = m_Puzzle->GetVal(r, c);
            if (val) { //if there is a value
                CellName cname = MK_NAME(r, c);
                rv = grid->SetCellVal(cname, val);
                if (rv != SDKERR_SUCCESS) {
                    return rv;
                }
            }
        } //for cols
    }//for rows
    return rv;
}

CellGrid* Sudoku::Solve() {

    int rv = SDKERR_SUCCESS;
    CellGrid *solution = NULL;

    solution = MakeCellGrid();
    rv = PlacePuzzle(solution);
    if (rv != SDKERR_SUCCESS) {
        printf("%s:%d: Unsolvable puzzle\n");
        delete solution;
        solution = NULL;
        return solution;
    }
    if (m_Info->opts & SDKOPT_VERBOSE) {
        ShowCellGrid("Puzzle Possibilities:\n", solution);
    }
    solution = Search(solution);
    return solution;
}

CellGrid* Sudoku::Search(CellGrid* grid) {

    int rv = SDKERR_SUCCESS;
    CellGrid* result = NULL;
    CellGrid* tmpgrd = NULL;

    rv = Solved(grid);
    if (rv == SDKERR_SUCCESS) {
        return grid;
    }

    Cell* cell = (Cell*) PickCell(grid);
    if (!cell) {
        return NULL;
    }
    CellName cname = cell->GetName();
    //sprintf(msg, "Searching %1x%1x:\n", GET_ROW(cname), GET_COL(cname));
    //ShowCellGrid(msg, grid);
    Possible* possible = cell->GetPossible();
    assert(possible->Allowed() > 1);

    std::vector<int> pos = possible->Get();
    unsigned int i = 0;
    for (; i < pos.size(); i++) {
        if (pos[i]) {
            if (tmpgrd) delete tmpgrd;
            tmpgrd = CloneCellGrid(grid);
            rv = tmpgrd->SetCellVal(cell->GetName(), i + 1);
            if (m_Info->opts & SDKOPT_VERBOSE) {
                char msg[32];
                sprintf(msg, "Place %1x in %1x%1x:\n", i + 1, 
                    GET_ROW(cname), GET_COL(cname));
                    ShowCellGrid(msg, tmpgrd);
            }
            if (rv != SDKERR_SUCCESS) {
                //if it didnt work then
                //try another value
                if (m_Info->opts & SDKOPT_VERBOSE) {
                    printf("Backtrack -- ");
                }
            } else {
                result = Search(tmpgrd);
                if (result && (Solved(result) == SDKERR_SUCCESS)) {
                    break;
                }
            }
        } //if pos[i]
    }//for
    if (result != tmpgrd) {
        delete tmpgrd;
        tmpgrd = NULL;
    }
    if (i == pos.size()) {
        if (result) {
            delete result;
            result = NULL;
        }
    }
    return result;
}

int Sudoku::Solved(CellGrid *grid)
{
    return grid->Solved();
}

const Cell* Sudoku::PickCell(CellGrid *grid)
{
    if (!grid) {
        return NULL;
    }

    //ShowCellGrid("Picking Cell:\n", grid);
    int dim = grid->GetDim();
    const Cell* cell = NULL;
    if (!m_PickCellsLookup) {
        m_PickCellsLookup = new Cell*[dim];
    }
    memset(m_PickCellsLookup, 0, sizeof(Cell*) * dim);

    //pick the cell that has > 1 possibilities
    //The algo below pics the last cell
    for (int r = 1; r <= dim; r++) {
        for (int c = 1; c<= dim; c++) {
            CellName cname = MK_NAME(r, c);
            cell = grid->GetCell(cname);
            assert(cell);
            int allowed = ((Cell*)cell)->GetPossible()->Allowed();
            assert(allowed != 0);
            m_PickCellsLookup[allowed - 1] = (Cell*) cell;
        }
    }

    int i = 1; //skip allowed = 1 values
    cell = NULL;
    while((!cell) && (i < dim)) {
        cell = m_PickCellsLookup[i++];
    }
    return cell;
}

int Sudoku::Show()
{
    return SDKERR_SUCCESS;
}

const Puzzle* Sudoku::GetPuzzle() {
    return (const Puzzle*) m_Puzzle;
}

int Sudoku::GetDim() {
    int dim = SDKERR_INVALID_PARAM;
    if (m_Puzzle) {
        dim = m_Puzzle->GetColsCount();
    }

    return dim;
}

const Peers* Sudoku::GetPeers(CellName name)
{
    const Peers* peers = NULL;
    CellPeers::iterator it = m_CellPeers->find(name);
    if (it != m_CellPeers->end()) {
        peers = (const Peers*) it->second;
    }
    return peers;
}

int Sudoku::ShowPeers() 
{
    if (!m_CellPeers) {
        return SDKERR_INVALID_PARAM;
    }

    CellPeers::iterator it = m_CellPeers->begin();
    for (; it != m_CellPeers->end(); ++it) {
        CellName cname = (*it).first;
        Peers* p = (*it).second;
        printf("%1x%1x(%02d): ", GET_ROW(cname), GET_COL(cname), p->size());
        for (unsigned int i = 0; i < p->size(); i++) {
            CellName pname = (*p)[i];
            printf("%1x%1x, ", GET_ROW(pname), GET_COL(pname));
        }
        printf("\n");
    }
    return SDKERR_SUCCESS;
}




////////////////////////
////////////////////////
////////////////////////
////////////////////////
////////////////////////

void Help() 
{
    printf("Solves sudoku problem in file.csv provided.\n");
    printf("Usage:\n");
    printf("\tsudoku [-q] [-t] [-v] -f file.csv\n\n");
    printf("\t\t-q \t Quite; Do not print the puzzle\n");
    printf("\t\t-v \t Be verbose\n");
    printf("\t\t-t \t Run tests\n");
    printf("\t\t-f \t Puzzle file\n");
}

typedef struct _SDKARGS {
    char* file;
    unsigned int opts;
} SDKARGS;

int ParseArgs(int argv, char** args, SDKARGS* sdkargs)
{
    memset(sdkargs, 0, sizeof(SDKARGS));
    for (int i = 1; i < argv; i++) {
        if (!strcmp(args[i], "-v")) {
            sdkargs->opts |=  SDKOPT_VERBOSE;
        } else if (!strcmp(args[i], "-t")) {
            sdkargs->opts |=  SDKOPT_TEST;
        } else if (!strcmp(args[i], "-q")) {
            sdkargs->opts |=  SDKOPT_QUITE;
        } else if (!strcmp(args[i], "-f")) {
            if (argv >= (i + 1)) {
                sdkargs->file = args[++i];
            }    
        }
    }

    if (!sdkargs->file) {
        return SDKERR_INVALID_PARAM;
    } else {
        return SDKERR_SUCCESS;
    }
}

void main(int argn, char** args)
{
    SDKARGS arg;
    int rv = ParseArgs(argn, args, &arg);
    if (rv != SDKERR_SUCCESS) {
        Help();
        return;
    }

    Puzzle p;
    rv = p.Init(arg.file);
    if (rv != SDKERR_SUCCESS) {
        return;
    }

    if (!(arg.opts & SDKOPT_QUITE)) {
        printf("Puzzle:\n");
        rv = p.Show();
        printf("\n");
    }

    SDK_INFO sdkinfo;
    sdkinfo.opts = arg.opts;
    Sudoku *sdk = Sudoku::Create(&p, &sdkinfo);
    rv = sdk->Init();
    if (rv != SDKERR_SUCCESS) {
        printf("%s:%d: Error at sdk init\n");
    }

    CellGrid *grid = sdk->Solve();
    if (grid) {
        if (!(arg.opts & SDKOPT_QUITE)) {
            printf("Solution:\n");
        }
        grid->ShowCSV();
    } else {
        printf("Not solved!\n");
    }

    if (grid) {
        delete grid;
    }
    delete sdk;
}

