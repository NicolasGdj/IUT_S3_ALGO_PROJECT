#include <iostream>
#include <vector>
#include <fstream>
#include <regex>

using namespace std;

std::string urlBase = "www.lmfdb.org";

std::string openFile(const std::string & path){
    std::string fPath = "curl -L " + path + " > currentPage.txt";
    system(fPath.c_str());
    ifstream ifs("currentPage.txt");
    std::string result;
    std::string line;
    while(getline(ifs, line))
        result+=line;
    return result;
}

bool startsWith(std::string mainStr, std::string toMatch) {
    if(mainStr.find(toMatch) == 0)
        return true;
    else
        return false;
}

void explore(vector<std::string> & pages, vector<vector<bool>> & adjs, const unsigned & current){
    if(pages.size() == current) return;
    std::string path = pages.at(current);
    std::cout << path << std::endl;


    std::string page = openFile(path);

    //Commencer a partir de body

    std::regex reg("<a href=\"[^\"]+\"");
    smatch matches;
    string::const_iterator searchStart( page.cbegin() );
    while ( regex_search( searchStart, page.cend(), matches, reg ) ) {
        std::string link = matches[0];
        link = link.substr(9, link.length()-10);
        std::cout << link << std::endl;
        if(startsWith(link, "https://"))
            link = link.substr(8);
        if(startsWith(link, "http://"))
            link = link.substr(7);
        bool absolut = link.at(0) == '/';

        size_t pos ;
        if(!absolut && !startsWith(link, urlBase)){
            pos = 0;
            std::cout << "Site exterieur" << std::endl;
        }else{
            pos = (std::find(pages.begin(), pages.end(), link) - pages.begin());
            if(pos == pages.size()){
                link = (absolut ? urlBase : "") + link;
                if(pages.size() != 10)
                    pages.push_back(link);
            }
        }
        adjs[current][pos] = true;
        searchStart = matches.suffix().first;
    }
    explore(pages, adjs, current+1);
}

int main()
{
    std::string path = urlBase + "/EllipticCurve/Q";
    vector<std::string> pages = vector<std::string>();
    vector<vector<bool>> matrices (1000, vector<bool> (1000));
    pages.push_back("nawak");
    pages.push_back(path);
    explore(pages, matrices, 1);

    for(const std::string & page : pages){
        std::cout << page << std::endl;
    }

    std::cout << "End" << std::endl;
    return 0;
}
