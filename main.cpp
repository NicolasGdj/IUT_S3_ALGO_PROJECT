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

std::string openFile(const std::string & path){

    /*
     * Creation d'une chaine d'un string qui contient la commande qui sera executée.
     * On utilise le logiciel curl qui nous permet de recuperer le contenue d'une page.
     * L'option -L permet de preciser au logiciel qu'il doit suivre la redirection qu'il y a sur la page.
     * Une fois le telechargement fait, on redirige la sortie "erreur" vers le fichier err.txt pour évité un affichage dans la console
     * Alors que la sortie standard est redirigé vers le fichier currentPage.txt
     * */
    std::string fPath = "curl -L \"" + path + "\" 2> err.txt > currentPage.txt";

    /* La donction system() permet d'executer directement une commande.
     * On passe en parametre la chaine definis plus ou et on la convertie en type string du language C.
     * */
    system(fPath.c_str());

    /*Etablissement d'un flux donnée d'entrée avec le fichier currentPage.txt, celui-ci est rempli plus haut.
     * */
    ifstream ifs("currentPage.txt");

    std::string result; // Declaration d'un string qui contiendra le contenue du fichier CurrentPage.txt
    std::string line; // Declaration d'un string contenant une ligne du fichier CurrentPage.txt

    /*La boucle while parcourt chaque ligne du stream ifs. Grace au getline, on stoque la ligne dans la variable
     * line et ensuite on concatene le contenu de la varible avec notre variable result.
     * */
    while(getline(ifs, line))
        result+=line;

    /*On ferme le fichier de telle façons a le liberé car par la suite nous allons re-ecrire dessus.
     * */
    ifs.close();

    //removeFile

    /* On retourne le resultat de la page.
     * */
    return result;
}

bool startsWith(const std::string & mainStr, const std::string & toMatch) {
    if(mainStr.find(toMatch) == 0) // Si il trouve la chaine en position 1 alors on retourne truee
        return true;
    else
        return false;
}


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



std::string get_url_base(const std::string & url){
    std::string link = get_url_without_extra_data(url);
    return getFirstSplit(link, "/");
}

std::string get_url_without_base(const std::string & url){
    return url.substr(get_url_base(url).length());
}

std::string get_extra_data(const std::string & url){
    std::size_t pos1 = url.find("#");
    std::size_t pos2 = url.find("?");

    if(pos1 == pos2){
        return pos1 == std::string::npos ? "" : url;
    }else{
        return url.substr(pos1 < pos2 ? pos1 : pos2, url.length());
    }
}

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
   // std::cout << "Final link : " <<link << std::endl;
}


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
        adjs.setValue(current_matrice_pos, matrice_pos, true);
    }

    explore(pages, adjs, tree, ++current);

}



int main(int argc, char* argv[])
{

    std::string path = "www.lmfdb.org/EllipticCurve/Q/";
    MAX = 100;

    if(argc != 4){
        std::cout << "Usage : ./a.out <baseURL> <max> <outFile>" << std::endl;
        //return 1;
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
        std::string output = argv[3];
    }

    normalize_link(path);
    urlBase = get_url_base(path);
    CTree tree (triplet("/", false, 0));
    CMatrix adj(MAX);


    std::list<std::string> pages;
    //vector<std::string> pages = vector<std::string>();
    tree.add(triplet("extern", true, pages.size()));
    pages.push_back("extern");
    tree.add(triplet(path, true, pages.size()));
    pages.push_back(path);
    explore(pages, adj, tree, ++pages.begin());


    ofstream out("out.txt");

    auto first = pages.begin();
    auto last = pages.end();

    /* On vient placer sur la première ligne tous les liens trouvés
     * */
    for(; first != last; ++first)
        out << ";" << (*first);
    
    /* Retour à la ligne
     * */
    out << std::endl;
    
    first = pages.begin();
    for(unsigned i = 0; first != last; ++first, ++i){
        out << (*first)  << ";";
        adj.printLine(i, out);// Viens placer tous les 0 et 1 sur la ligne Exel au bon endroit
        out << std::endl;// Retour à la ligne
    }

    std::cout << std::endl << "Affichage de l'arbre correspondant" << std::endl;
    tree.show();

    return 0;
}
