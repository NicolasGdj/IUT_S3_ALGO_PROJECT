#ifndef CTREE_HPP
#define CTREE_HPP

/*!
 * \file ctree.hpp
 * \brief Fichier contenant la classe arbre
 * \author Alexandre BOUILLE, Louis DUTTIER, Enzo PALOTY, Mathieu GIACALONE, Lucas FAUCHIER, Nicolas GUERROUDJ
 * \version 1.0
 */

#include <iostream>
#include <memory>
#include <list>

#include "node.hpp"

struct triplet{
    std::string sub;
    bool added;
    unsigned matrice_pos;
    triplet(std::string sub, bool added, unsigned matrice_pos) : sub(sub), added(added), matrice_pos(matrice_pos){

    }
};

/*! \class CTree
   * \brief classe representant un arbre
   *
   *  Cette classe crée et contient toutes les fonctions utiles a un arbre
   */

class CTree
{
private:
    std::shared_ptr<CNode<triplet>> m_root; /*!< Pointeur vers la node principal de l'arbre*/

    /*!
         *  \brief Constructeur privée
         *
         *  Constructeur secondaire de la classe CTree
         *
         *  \param root : Pointeur vers la node principal de l'arbre
         */
    CTree(const std::shared_ptr<CNode<triplet>> root) : m_root(root){

    }

public:

    /*!
         *  \brief Constructeur
         *
         *  Constructeur de la classe CTree
         *
         *  \param val : Triplet servant a crée la premiere node
         *  \return Retourne une instance de la classe
         */
    CTree(const triplet & val) : m_root(std::shared_ptr<CNode<triplet>>(new CNode<triplet>(val))){

    }

    /*!
         *  \brief Ajout d'un triplet
         *
         *  Ajout d'un triplet a l'arbre
         *
         *  \param trip : Triplet a ajouter a l'arbre
         */
    void add(const triplet & trip){
        if(!m_root) return;
        const std::string & url = trip.sub;
        if(url.length() == 0) return;
        std::string sub = getFirstSplit(url, "/");
        std::string next = url.substr(sub.length());

        if(next.length() != 0 && next[0] == '/')
            next = next.substr(1);


        if(sub.length() == 0) return;

        std::list<std::shared_ptr<CNode<triplet>>> & children = m_root->getChildren();

        auto first = children.begin();
        auto last = children.end();

        std::shared_ptr<CNode<triplet>> found = nullptr;

        for(; first != last; ++first){
            if((*first)->getData().sub == sub){
                found = (*first);
                break;
            }
        }
        bool hasNext = next.size() != 0;

        if(!found){
            found = std::shared_ptr<CNode<triplet>>(new CNode<triplet>(triplet(sub, !hasNext, hasNext ? 0 : trip.matrice_pos)));
            children.push_back(found);
        }else if(!hasNext){
            triplet & oldTriplet = found->getData();
            oldTriplet.added = true;
            oldTriplet.matrice_pos = trip.matrice_pos;
        }


        CTree(found).add(triplet(next, trip.added, trip.matrice_pos));

    }

    /*!
         *  \brief Verifie si une node crée avec une URL existe
         *
         *  Verifie si une node crée avec une URL existe
         *
         *  \param url : Url d'une page internet
         *  \return Si la valeur est trouvé true sinon false
         */
    bool find(const std::string & url){ // www.youtube.com/user/nicolas
        if(!m_root) return false;
        if(url.length() == 0) return false;
        std::string sub = getFirstSplit(url, "/");
        std::string next = url.substr(sub.length());

        if(next.length() != 0 && next[0] == '/')
            next = next.substr(1);
        else if (next.length() == 0 && sub == m_root->getData().sub && m_root->getData().added)
            return true;

        if(sub.length() == 0) return false;

        std::list<std::shared_ptr<CNode<triplet>>> & children = m_root->getChildren();

        auto first = children.begin();
        auto last = children.end();

        for(; first != last; ++first)
            if((*first)->getData().sub == sub){
                if(next.length() == 0){
                    return (*first)->getData().added;
                }
                return CTree((*first)).find(next);
            }

        return false;
    }

    /*!
         *  \brief Recuperation de l'index
         *
         *  Recupere l'index d'une node a partir d'une URL
         *
         *  \param url : Url d'une page internet
         *  \return Index
         */
    unsigned get(const std::string & url){
        if(!m_root) return 0;
        if(url.length() == 0) return 0;
        std::string sub = getFirstSplit(url, "/");
        std::string next = url.substr(sub.length());

        if(next.length() != 0 && next[0] == '/')
            next = next.substr(1);
        else if (next.length() == 0 && sub == m_root->getData().sub && m_root->getData().added)
            return m_root->getData().matrice_pos;

        if(sub.length() == 0) return 0;

        std::list<std::shared_ptr<CNode<triplet>>> & children = m_root->getChildren();

        auto first = children.begin();
        auto last = children.end();

        for(; first != last; ++first)
            if((*first)->getData().sub == sub){
                if(next.length() == 0)
                    return (*first)->getData().matrice_pos;
                return CTree((*first)).get(next);
            }

        return 0;
    }

    /*!
         *  \brief Affichage de l'arbre
         *
         *  Affichage de l'abre avec la posibilité de mettre un ecart
         *
         *  \param shift : Ecart
         */
    void show(unsigned shift = 0) const{
        if(!m_root) return;
        for(unsigned i = 0; i < shift; ++i)
            std::cout << " ";
        std::cout << m_root->getData().sub << " " << m_root->getData().added << " " << m_root->getData().matrice_pos << std::endl;
        const std::list<std::shared_ptr<CNode<triplet>>> & children = m_root->getChildren();

        auto first = children.begin();
        auto last = children.end();
        for(; first != last; ++first){
            CTree(std::shared_ptr<CNode<triplet>>(*first)).show(shift+1);
        }
    }
};
#endif // CTREE_HPP
