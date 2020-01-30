#ifndef CTREE_HPP
#define CTREE_HPP

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

class CTree
{
private:
    std::shared_ptr<CNode<triplet>> m_root;
    CTree(const std::shared_ptr<CNode<triplet>> root) : m_root(root){

    }
public:
    CTree(const triplet & val) : m_root(std::shared_ptr<CNode<triplet>>(new CNode<triplet>(val))){

    }
    void add(const triplet & trip){ // www.youtube.com/user/nicolas
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


    unsigned get(const std::string & url){ // www.youtube.com/user/nicolas
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
