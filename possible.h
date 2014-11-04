
#ifndef __SUDOKU_POSSIBLE__
#define __SUDOKU_POSSIBLE__
#include "sudoku.h"

#include <vector>

class Possible {

protected:
    int m_nPos;                 //total possibilities
    std::vector<int> m_Pos;     //vector containing possibilities
    int m_Allowed;

public:
    Possible(int pos);
    virtual ~Possible();
    int Place(int pos);
    int Remove(int pos);
    std::vector<int> Get();
    int GetVal();
    int Set(int pos);
    int Allowed();
    int MaxCount();
    int Allowed(int pos);


    int Show();

    int DoUT();
};

#endif //__SUDOKU_POSSIBLE__
