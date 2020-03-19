/*!
 * \file main.cpp
 * \brief Fichier principal du logiciel
 * \version 1.0
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <regex>
#include <list>
#include <iterator>

#include "ctree.hpp"
#include "cmatrix.hpp"

using namespace std;

std::string urlBase; // String contenant l'url du site
unsigned MAX = 4;

/*!
     *  \brief Ouvre un fichier
     *
     *  Une un fichier et recuperation du contenue
     *
     *  \param path : Chemin du fichier
     *  \return Contenue du fichier
     */
std::string openFile(const std::string & path){

    std::string fPath = "curl -L \"" + path + "\" 2> err.txt > currentPage.txt";

    system(fPath.c_str());

    ifstream ifs("currentPage.txt");

    std::string result;
    std::string line;

    while(getline(ifs, line))
        result+=line;


    ifs.close();

    return result;
}

/*!
     *  \brief Verifie si un string commence par un string
     *
     *  Verifie si un string commence par une valeur
     *
     *  \param mainStr : String principal
     *  \param toMatch : Chaine qui doit se trouver un premiere position de la chaine principal
     *  \return true si existant en premiere position sinon false
     */
bool startsWith(const std::string & mainStr, const std::string & toMatch) {
    if(mainStr.find(toMatch) == 0)
        return true;
    else
        return false;
}

/*!
     *  \brief Recuperation de l'URL formatée
     *
     *  Recuperation de l'URL sans aucun parametre
     *
     *  \param url : Url a formater
     *  \return Url formaté
     */
std::string get_url_without_extra_data(const std::string & url){
    if(url.length() == 0)
        return url;
    std::string link = url;
    std::size_t pos = link.find("#");
    if(pos != std::string::npos){
        link = link.substr(0, pos);
    }
    pos = link.find("?");
    if(pos != std::string::npos){
        link = link.substr(0, pos);

    }
    return link;
}


/*!
     *  \brief Recupere le nom de dmaine
     *
     *  Recupere le nom de domaine a partir d'une Url
     *
     *  \param url : Url original
     *  \return Le nom de domaine
     */
std::string get_url_base(const std::string & url){
    std::string link = get_url_without_extra_data(url);
    return getFirstSplit(link, "/");
}

/*!
     *  \brief Retourne le nom de la page
     *
     *  Retourne le nom de la page sans nom de domaine a partir d'une Url
     *
     *  \param url : Url original
     *  \return L'url formaté
     */
std::string get_url_without_base(const std::string & url){
    return url.substr(get_url_base(url).length());
}

/*!
     *  \brief Retourne les parametres de l'url
     *
     *  Retourne les parametres etant dans une Url
     *
     *  \param url : Url original
     *  \return String contenant les parametres
     */
std::string get_extra_data(const std::string & url){
    std::size_t pos1 = url.find("#");
    std::size_t pos2 = url.find("?");

    if(pos1 == pos2){
        return pos1 == std::string::npos ? "" : url;
    }else{
        return url.substr(pos1 < pos2 ? pos1 : pos2, url.length());
    }
}

/*!
     *  \brief Formate une Url
     *
     *  Application de nombreux changement pour que la formes de toutes les Url sois les mêmes
     *
     *  \param link : Url original
     */
void normalize_link(std::string & link){
    if(link.length() == 0) return;
    std::size_t pos = link.find("#");
    if(pos != std::string::npos){
        std::size_t end_pos = link.find("?");

        link = link.substr(0, pos);
        if(end_pos <= link.length())
            link += link.substr(end_pos);
    }

    if(startsWith(link, "https://"))
        link = link.substr(8);
    else if(startsWith(link, "http://"))
        link = link.substr(7);
    else if(startsWith(link, "//"))
        link = link.substr(2);

    if(link.length() != 0){

        std::string url = get_url_without_extra_data(link);

        std::string extra = get_extra_data(link);

        if(url.length() != 0 && url.at(url.length()-1) == '/'){

            if(url.length() != 1)
                link = url.substr(0, url.length()-1) + extra;
        }
    }
}

/*!
     *  \brief Parcour d'une page
     *
     *  Parcours d'une page pour trouver et traiter des liens hypertext
     *
     *  \param pages : Liste de pages
     *  \param adjs : Matrice adjacente
     *  \param tree : Arbre
     *  \param current : iterateur courant
     */
void explore(std::list<std::string> & pages, CMatrix & adjs, CTree & tree, std::list<std::string>::iterator & current){
    if(pages.end() == current) return;
    std::string path = *current;

    std::size_t current_matrice_pos = tree.get(path);
    std::cout << "Recherche de liens sur " << path << " [" << (current_matrice_pos+1) << "/" << pages.size() << "]" << std::endl;

    std::string page = openFile(path);

    std::string search = "<body";

    std::size_t pos_start = page.find(search);

    if(pos_start == std::string::npos){
        std::cout << "  -> <body> introuvable" << std::endl;
        explore(pages, adjs, tree, ++current);
        return;
    }

    search = "</body";
    std::size_t pos_end = page.find(search);

    if(pos_end == std::string::npos){
        std::cout << "  -> </body> introuvable" << std::endl;
        explore(pages, adjs, tree, ++current);
        return;
    }

    page = page.substr(pos_start, (pos_end-pos_start));

    string start_href = "href=\"";
    string end_href = "\"";
    size_t found = 0;
    size_t found_end = 0;

    string url_base_path = get_url_base(path);
    while(true){
        found = page.find(start_href, found+1);
        if(found == string::npos) break;
        found_end = page.find(end_href, found+start_href.length()+1);
        if(found_end == string::npos) continue;
        string link = page.substr(found+start_href.length(), found_end-(found+start_href.length()));
        normalize_link(link);

        if(get_url_base(link) == url_base_path){
            link = get_url_without_base(link);
        }

        if(link.length() == 0) continue;


        bool absolut = link.at(0) == '/';
        bool params = link.at(0) == '?';
        if(params)
            link = path + link;


        size_t matrice_pos = 0;

        if(absolut || startsWith(link, urlBase)){
            if(!params){
                if(absolut){
                    link = urlBase+link;
                }else{
                    link = get_url_without_extra_data(path) + link;
                }
            }
            normalize_link(link);

            if(!tree.find(link)){
                if(pages.size() != MAX){
                    matrice_pos = pages.size();
                    pages.push_back(link);
                    tree.add(triplet(link, true, matrice_pos));
                }else{
                    continue;
                }
            }else{
                matrice_pos = tree.get(link);
            }

        }
        adjs.setValue(matrice_pos, current_matrice_pos, true);
    }

    explore(pages, adjs, tree, ++current);

}

/*!
     *  \brief Fonction principal du programmme
     *
     *  Fonction executé en premier par le programme
     *
     *  \param argc : Nombre d'argument passé en parametre dans le programme
     *  \param argv : Parametre passé dans le programme
     */
int main(int argc, char* argv[])
{

    std::string path = "";
    MAX = 1;
    std::string output = "out.txt";
    for(int i = 0; i < argc; ++i){
        if(argv[i] == std::string("-i")){
            std::cout << std::endl
                      << "Software created by students of the IUT of Aix-en-provence :" << std::endl
                      << "Lucas Fauchier, Mathieu GIACALONE, Enzo PALOTY, Alexandre BOUILLE and Nicolas GUERROUDJ." << std::endl
                      << "Project tutored by M. Alain CASALI & M. Samuele ANNI." << std::endl
                      << std::endl;
            return 0;
        }
    }
    if(argc != 4){
        std::cout << "Usage : ./a.out [-i] <baseURL> <max> <outFile>" << std::endl;
        return 1;
    }else{
        path = argv[1];
        try {
            MAX = std::stoul(argv[2]) + 1; //Ajouter un pour combler les liens externes
        } catch (std::invalid_argument ia) {
            std::cout << "Max invalid value" << std::endl;
            return 1;
        } catch (std::out_of_range oor) {
            std::cout << "Max invalid value" << std::endl;
            return 1;
        }
       output = argv[3];
    }

    normalize_link(path);
    urlBase = get_url_base(path);
    CTree tree (triplet("/", false, 0));
    CMatrix adj(MAX);

    std::list<std::string> pages;
    tree.add(triplet("extern", true, pages.size()));
    pages.push_back("extern");
    tree.add(triplet(path, true, pages.size()));
    pages.push_back(path);
    explore(pages, adj, tree, ++pages.begin());


    ofstream out(output);

    auto first = pages.begin();
    auto last = pages.end();

    for(; first != last; ++first)
        out << ";" << (*first);
    

    out << std::endl;
    
    first = pages.begin();
    for(unsigned i = 0; first != last; ++first, ++i){
        out << (*first)  << ";";
        adj.printLine(i, out);
        out << std::endl;
    }

    std::cout << std::endl << "Display of the corresponding tree: " << std::endl;
    tree.show();

    return 0;
}
