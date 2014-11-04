
#include "sudoku.h"
#include "possible.h"


Possible::Possible(int pos) : m_Pos(pos, 1) 
{
    m_nPos = pos;     
    m_Allowed = pos;
}

Possible::~Possible() {

}

int Possible::Place(int pos) {
    if ((pos < 1) || (pos > m_nPos)) {
        return SDKERR_INVALID_PARAM;
    }

    if (Allowed(pos)) {
        std::fill(m_Pos.begin(), m_Pos.end(), 0);
        m_Allowed = 0;
        return Set(pos);
    } else {
        return SDKERR_CONSTRAINT;
    }
}

int Possible::Set(int pos) {
    if ((pos < 1) || (pos > m_nPos)) {
        return SDKERR_INVALID_PARAM;
    }

    if (!m_Pos[pos - 1]) {
        m_Allowed++;
        m_Pos[pos - 1] = 1;
    } else {
        return SDKERR_CONSTRAINT;
    }
   
    return SDKERR_SUCCESS;
}

int Possible::Remove(int pos) {
    if ((pos < 1) || (pos > m_nPos)) {
        return SDKERR_INVALID_PARAM;
    }

    if (m_Pos[pos - 1]) {
        m_Allowed--;
        m_Pos[pos - 1] = 0;
    }

    if (m_Allowed <= 0) {
        return SDKERR_CONSTRAINT;
    }

    return SDKERR_SUCCESS;
}

int Possible::Allowed() {
    return m_Allowed;
}

int Possible::Allowed(int pos) {
    if ((pos < 1) || (pos > m_nPos)) {
        return SDKERR_INVALID_PARAM;
    }
    return m_Pos[pos - 1];
}

int Possible::MaxCount() {
    return m_nPos;
}

int Possible::Show()
{
    for (int i = 0; i < m_Pos.size(); i++) {
        if (m_Pos[i]) {
            printf("%1d", i+1);
        } else {
            printf(".");
        }
    }
    return SDKERR_SUCCESS;
}

std::vector<int> Possible::Get()
{
    return m_Pos;
}

int Possible::GetVal() {

    if (m_Allowed > 1) {
        return SDKERR_INVALID_PARAM;
    } else if (m_Allowed < 1) {
        return SDKERR_CONSTRAINT;
    } else for (int i = 0; i < m_Pos.size(); i++) {
        if (m_Pos[i]) {
            return i+1;
        }
    }
}

int Possible::DoUT() 
{
    

    return SDKERR_SUCCESS;
}