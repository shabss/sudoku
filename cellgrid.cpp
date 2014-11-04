

#include "sudoku.h"
#include "cell.h"
#include "cellgrid.h"
#include "possible.h"

CellGrid::CellGrid(int dim, Sudoku* sdk)
{
    m_Dim = dim;
    m_Grid = NULL;
    m_sdk = sdk;
}

CellGrid::~CellGrid()
{
    DeleteInternalGrid();
}

int CellGrid::DeleteInternalGrid()
{
    if (m_Grid) {
        for (int r = 1; r <= m_Dim; r++) {
            for (int c = 1; c<= m_Dim; c++) {
                CellName cname = MK_NAME(r, c);
                std::map<CellName, Cell*>::iterator it;
                it = m_Grid->find(cname);
                if (it != m_Grid->end()) {
                    Cell* cell = it->second;
                    it->second = NULL;
                    delete cell;
                }
            }
        } 
        delete m_Grid;
        m_Grid = NULL;
    }

    return SDKERR_SUCCESS;
}

int CellGrid::Init()
{
    int rv = SDKERR_SUCCESS;
    if (m_Grid) {
        rv = DeleteInternalGrid();
    }

    m_Grid = new std::map<CellName, Cell*>;

    for (int r = 1; r <= m_Dim; r++) {
        for (int c = 1; c <= m_Dim; c++) {
            CellName cname = MK_NAME(r, c);
            Cell *cell = new Cell(cname, this);
            (*m_Grid)[cname] = cell;
        }
    }    
    return rv;
}

int CellGrid::Init(CellGrid* source)
{
    int rv = SDKERR_SUCCESS;
    if (!m_Grid) {
        rv = Init();
        //to do: check for rv
    }

    //To do: Copy source value
    for (int r = 1; r <= m_Dim; r++) {
        for (int c = 1; c <= m_Dim; c++) {
            CellName cname = MK_NAME(r, c);
            std::map<CellName, Cell*>::iterator itsrc, itme;
            itsrc = source->m_Grid->find(cname);
            itme  = m_Grid->find(cname);
            if ((itsrc != source->m_Grid->end()) && 
                (itme != m_Grid->end())) {
                Cell *src = itsrc->second;
                Cell *mine= itme->second;

                //note copies only the possiblities
                *mine = *src;
            } else {
                return SDKERR_PROGRAM_ERROR;
            }
        }
    }    
    return rv;
}


int CellGrid::GetDim()
{
    return m_Dim;
}

int CellGrid::Show()
{
    printf("---|");
    for (int c = 1; c <= m_Dim; c++) {
        printf("----%1d----|", c);
    }
    printf("\n");

    for (int r = 1; r <= m_Dim; r++) {
        printf(" %1d |", r);
        for (int c = 1; c<= m_Dim; c++) {
            CellName cname = MK_NAME(r, c);
            std::map<CellName, Cell*>::iterator it;
            it = m_Grid->find(cname);
            if (it != m_Grid->end()) {
                Cell* cell = it->second;
                cell->ShowPossible();
                printf("|");
            }
        }
        printf("\n");
    }

    return SDKERR_SUCCESS;
}

int CellGrid::SetCellVal(CellName cname, int val)
{
    int rv = SDKERR_SUCCESS;
    Cell *cell = (Cell*) GetCell(cname);
    if (!cell) {
        return SDKERR_PROGRAM_ERROR;
    }

    rv = cell->SetVal(val);
    if (rv != SDKERR_SUCCESS) {
        return rv;
    }

    const Peers* peers = m_sdk->GetPeers(cname);
    for (int i = 0; i < peers->size(); i++) {
        CellName pname = (*peers)[i];
        std::map<CellName, Cell*>::iterator it;
        it = m_Grid->find(pname);
        if (it == m_Grid->end()) {
            return SDKERR_PROGRAM_ERROR;
        }
        cell = it->second;
        rv = cell->ResetVal(val);
        if (rv != SDKERR_SUCCESS) {
            return rv;
        }
    }
    return rv;
}

const Cell* CellGrid::GetCell(CellName cname)
{
    Cell *cell = NULL;
    std::map<CellName, Cell*>::iterator it;
    it = m_Grid->find(cname);
    if (it == m_Grid->end()) {
        return NULL;
    }
    cell = it->second;
    return (const Cell*) cell;
}



int CellGrid::Solved()
{
    int rv = SDKERR_SUCCESS;

    for (int r = 1; r <= m_Dim; r++) {
        for (int c = 1; c<= m_Dim; c++) {
            CellName cname = MK_NAME(r, c);
            Cell* cell = (Cell*) GetCell(cname);

            //find if this cell has at most 1 value
            int cval = cell->GetVal();
            if (cval < 0) {
                return cval;
            }

            //find if any peers have same values
            const Peers* peers = m_sdk->GetPeers(cname);
            for (int i = 0; i < peers->size(); i++) {
                CellName pname = (*peers)[i];
                Cell* pcell = (Cell*) GetCell(pname);
                int pval = pcell->GetVal();
                if (pval < 0) {
                    //zero values
                    return pval;
                } else if (cval == pval) {
                    return SDKERR_CONSTRAINT;
                }
            }
        }
    }
    return SDKERR_SUCCESS;
}