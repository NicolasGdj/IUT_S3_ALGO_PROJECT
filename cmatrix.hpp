#ifndef CMATRIX_HPP
#define CMATRIX_HPP

/*!
 * \file cmatrix.hpp
 * \brief Fichier contenant la classe cmatrix
 * \author Alexandre BOUILLE, Louis DUTTIER, Enzo PALOTY, Mathieu GIACALONE, Lucas FAUCHIER, Nicolas GUERROUDJ
 * \version 1.0
 */

#include <iostream>
#include <list>
#include <vector>

typedef std::pair<size_t, bool> pairColVal; /*!< Definition d'un type pairColVal*/

/*! \class CMatrix
   * \brief classe representant une matrice
   *
   *  Cette classe gere toutes les fonctions liées a une matrice
   */

class CMatrix{
private:
    std::vector<std::list<pairColVal>> matrix; /*!< Matrice*/
public:
    /*!
         *  \brief Constructeur
         *
         *  Constructeur de la classe CMatrix
         *
         *  \param size : Taille de la matrice
         *  \return Retourne une instance de la classe
         */
    CMatrix(const size_t & size) : matrix(size){

    }
    /*!
         *  \brief Recupere une valeur
         *
         *  Recupere une valeur dans la matrice
         *
         *  \param i : Premiere clé (ligne)
         *  \param j : Seconde clé (colonne)
         *  \return Retrourne false si la valeur n'existe pas
         */
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
    /*!
         *  \brief Definit une valeur
         *
         *  Association d'une valeur a deux clés
         *
         *  \param i : Premiere clé (ligne)
         *  \param j : Seconde clé (colonne)
         *  \param val : Valeur a definir avec les deux clé
         */
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

    /*!
         *  \brief Affichage d'une ligne
         *
         *  Afficahe une ligne dans un flux de sortie
         *
         *  \param i : Premiere clé (ligne)
         *  \param ostream : flux de sortie
         */
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
