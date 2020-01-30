#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <memory>
#include <list>


std::string getFirstSplit(const std::string & str, const std::string & separator){
    std::size_t pos = str.find(separator);
    if(pos != std::string::npos){
        return str.substr(0, pos);
    }
    return str;
}

template<typename T>
class CNode
{
    T data;
    std::list<std::shared_ptr<CNode<T>>> children;
    typedef typename std::list<std::shared_ptr<CNode<T>>>::iterator children_it;

public: // www.youtube.com/user/nicolaslebg/20002
    CNode(const T & data) : data(data){

    }

    T & getData(){
        return data;
    }

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
