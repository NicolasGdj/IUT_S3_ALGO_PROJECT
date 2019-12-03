#include <iostream>
#include <vector>
#include <fstream>
#include <regex>

using namespace std;

std::string openFile(const std::string & path){
    std::string fPath = "curl " + path + " > currentPage.txt";
    system(fPath.c_str());
    ifstream ifs("currentPage.txt");
    std::string result;
    std::string line;
    while(getline(ifs, line))
        result+=line;
    return result;
}

void explore(vector<std::string> & pages, const unsigned & current){
    if(pages.size() == current) return;
    std::string path = pages.at(current);
    std::cout << "Exploration de " << path << ":" << std::endl;
    std::string page = openFile(path);
    std::regex reg("<a href=\"[^\"]+\"");
    smatch matches;
    string::const_iterator searchStart( page.cbegin() );
    while ( regex_search( searchStart, page.cend(), matches, reg ) ) {
        cout << ( searchStart == page.cbegin() ? "" : " " ) << matches[0]; // Sortie du string
        searchStart = matches.suffix().first;
   }
    explore(pages, current+1);
}

int main()
{
    std::string path = "https://www.lmfdb.org/EllipticCurve/Q/";
    vector<std::string> pages = vector<std::string>();
    vector<vector<bool>> matrices = vector<vector<bool>>();
    pages.push_back(path);
    explore(pages, 0);
    std::cout << "End" << std::endl;
    return 0;
}
