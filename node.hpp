#ifndef NODE_HPP
#define NODE_HPP

/*!
 * \file node.hpp
 * \brief Fichier contenant la classe node
 * \author Alexandre BOUILLE, Louis DUTTIER, Enzo PALOTY, Mathieu GIACALONE, Lucas FAUCHIER, Nicolas GUERROUDJ
 * \version 1.0
 */


#include <iostream>
#include <memory>
#include <list>

/*!
     *  \brief Recupere la premiere partie d'une separation
     *
     *  Renvoie la premiere partie d'un string separé par un sperateur
     *
     *  \param str : Le string a coupé
     *  \param separator : Separateur
     */
std::string getFirstSplit(const std::string & str, const std::string & separator){
    std::size_t pos = str.find(separator);
    if(pos != std::string::npos){
        return str.substr(0, pos);
    }
    return str;
}

template<typename T>

/*! \class CNode
   * \brief classe representant une node
   *
   *  Cette classe créer et gere toutes les fonctions liées a une node
   */
class CNode
{
    T data; /*!< Conetient la valeur de la node (donnée(s)*/
    std::list<std::shared_ptr<CNode<T>>> children; /*!< Liste des enfants de la node*/
    typedef typename std::list<std::shared_ptr<CNode<T>>>::iterator children_it; /*!< Pointeur vers la node principal de l'arbre*/

public: // www.youtube.com/user/nicolaslebg/20002
    /*!
         *  \brief Constructeur
         *
         *  Constructeur de la classe CNode
         *
         *  \param data : Valeur de la node (donnée(s))
         *  \return Retourne une instance de la classe
         */
    CNode(const T & data) : data(data){

    }

    /*!
         *  \brief Retourne la valeur donnée(s) de la node
         *
         *  Retourne la valeur de la node (donnée(s)) definit
         *
         *  \return Valeur (donnée) de la node
         */
    T & getData(){
        return data;
    }

    /*!
         *  \brief Retourne les enfants
         *
         *  Retourne la liste de tous les enfants de cette node
         *
         *  \return Enfant de la node
         */
    std::list<std::shared_ptr<CNode<T>>> & getChildren(){
        return children;
    }

    /*
    CNode * find(T url){
        if(url.length() == 0) return nullptr;
        if(url == getData()) return this;
        T sub = getFirstSplit(url, "/");
        if(sub != getData()) return nullptr;
        T next = url.substr(sub.length());
        if(next.length() != 0 && next[0] == '/')
            next = next.substr(1);

        children_it first = children.begin();
        children_it last = children.end();

        for(; first != last; ++first){
            CNode * n = first->find(next);
            if(n != nullptr){
                return n;
            }
        }
        return nullptr;
    }
    */
};


#endif // NODE_HPP
