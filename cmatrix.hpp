#ifndef CMATRIX_HPP
#define CMATRIX_HPP

#include <iostream>
#include <list>
#include <vector>

typedef std::pair<size_t, bool> pairColVal;
class CMatrix{
private:
    std::vector<std::list<pairColVal>> matrix;
public:
    CMatrix(const size_t & size) : matrix(size){

    }
    bool getValue(const size_t & i, const size_t & j){
        if(matrix.size() > i){
            std::list<pairColVal> & list = matrix[i];
            std::list<pairColVal>::iterator it;
            for(it = list.begin(); it != list.end(); ++it){
                if(it->first == j){
                    return it->second;
                }
            }
        }
        return false;
    }

    void setValue(const size_t & i, const size_t & j, const bool & val){

        if(matrix.size() > i){

            std::list<pairColVal> & list = matrix[i];
            std::list<pairColVal>::iterator it;
            for(it = list.begin(); it != list.end(); ++it){
                if(it->first == j){
                    it->second = val;
                    return;
                }else if(it->first > j){
                    list.insert(it, std::make_pair(j, val));
                    return;
                }
            }
            list.push_back(std::make_pair(j, val));
        }
        //Dépassement de la taille de la matrice
    }

    void printLine(const size_t & i, std::ostream & ostream){

        std::list<pairColVal> & list = matrix[i];
        std::list<pairColVal>::iterator it;

        size_t pos = 0;

        for(it = list.begin(); it != list.end(); ++it){
            for(size_t j = pos; j < it->first; ++j){
                if(j != 0)
                    ostream << ";";
                ostream << "0";
            }
            pos = it->first+1;
            if(it->first != 0)
                ostream << ";";
            ostream << it->second;
        }
        for(size_t j = pos; j < matrix.size(); ++j){
            if(j != 0)
                ostream << ";";
            ostream << "0";
        }
    }
};

#endif // CMATRIX_HPP
