

#include <string.h>
#include <stdlib.h>
#include "sudoku.h"
#include "puzzle.h"

Puzzle::Puzzle() 
{
    m_Matrix = NULL;
    m_Cols = 0;
    m_Rows = 0;
}

Puzzle::~Puzzle() 
{
    if (m_Matrix) {
        delete [] m_Matrix;
        m_Matrix = NULL;
    }
    m_Cols = m_Rows = 0;
}

int Puzzle::GetRowsCount() {
    return m_Rows;
}

int Puzzle::GetColsCount() {
    return m_Cols;
}

const char* Puzzle::GetMatrix() {
    return (const char*) m_Matrix;
}

int Puzzle::Init(FILE *fp) {
    //printf("%s:%d: here\n", __FUNCTION__, __LINE__);
    int rv = SDKERR_SUCCESS;

    if (!fp) {
        return SDKERR_INVALID_PARAM;
    }

    rv = fseek(fp, 0, SEEK_SET);
    if (rv != 0) {
        return SDKERR_UNKNOWN;
    }

    char line[257];

    //assume a 9x9 matrix for now. Will change this later
    m_Cols = m_Rows = 9; 
    m_Matrix = new char [m_Cols * m_Rows];
    memset(m_Matrix, 0 , m_Cols * m_Rows);

    char *m = m_Matrix; //short name
    while (fgets(line, sizeof(line) - 1, fp) != NULL) {
        //printf("%s:%d: here\n", __FUNCTION__, __LINE__); 
        char *l = line;
        //To Do: Ignore blank lines
        if ((m - m_Matrix) >= (m_Cols * m_Rows)) {
            //We are done
            //return SDKERR_UNKNOWN;
            break;
        }
        char *comma = NULL;
        int col = 0;
        while (comma = strchr(l, ',')) {
            *comma = '\0';
            m[col++] = atoi(l);
            l = ++comma;
        }
        //do one last column
        if (col < m_Cols) {
            m[col++] = atoi(l);
        }

        //printf("%s:%d: here\n", __FUNCTION__, __LINE__);
        if (col != m_Cols) {
            //unexpected number of cols
            return SDKERR_UNKNOWN;
        } 
        m = &m[col];
    }

    //to do: Check the values for sanity
    //printf("%s:%d: here\n", __FUNCTION__, __LINE__);
    return rv;
}


int Puzzle::Show() {
    
    for (int i = 0; i < m_Rows; i++) {
        for (int j = 0; j < m_Cols; j++) {
            printf(" %d%c", 
                m_Matrix[(i*m_Cols) + j], 
                j < m_Cols - 1 ? ',' : '\n');
        }
    }
    return SDKERR_SUCCESS;
}

int Puzzle::GetVal(int r, int c)
{
    if (!m_Matrix) {
        return SDKERR_PROGRAM_ERROR;
    }

    if ((r < 1) || (c < 1) || (r > m_Rows) || (c > m_Cols)) {
        return SDKERR_PROGRAM_ERROR;
    }

    return m_Matrix[((r-1)*m_Cols) + (c -1)];

}