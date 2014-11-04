
#include "sudoku.h"
#include "cell.h"
#include "possible.h"
#include "puzzle.h"
#include "cellgrid.h"

Cell::Cell(CellName name, const CellGrid *grid)
{
    m_grid = grid;
    m_name = name;
    m_pos = new Possible(((CellGrid*)grid)->GetDim());
}

Cell::~Cell() 
{
    if (m_pos) {
        delete m_pos;
        m_pos = NULL;
    }
}

int Cell::Init() 
{

    return SDKERR_SUCCESS;
}

int Cell::ShowPossible()
{
    return m_pos->Show();
}


Cell& Cell::operator=(const Cell& c) {
    *m_pos = *c.m_pos;
    return *this;
}

int Cell::SetVal(int val)
{
    int rv = SDKERR_SUCCESS;
    if (m_pos->Allowed(val)) {
        m_pos->Place(val);
    } else {
        rv = SDKERR_CONSTRAINT;
    }
    return rv;
}

int Cell::GetVal()
{
    return m_pos->GetVal();
}

int Cell::ResetVal(int val)
{
    return m_pos->Remove(val);
}

Possible* Cell::GetPossible()
{
    return m_pos;
}

CellName Cell::GetName()
{
    return m_name;
}