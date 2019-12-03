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

    std::regex reg("href=\"[^\"]+\"");
    smatch matches;
    if(std::regex_search(page, matches, reg)) {
        std::cout << "Match found\n";

        for (size_t i = 0; i < matches.size(); ++i) {
            std::cout << i << ": '" << matches[i] << "'\n";
        }
    } else {
        std::cout << "Match not found\n";
    }
    explore(pages, current+1);
}

int main()
{
    std::string path = "https://www.lmfdb.org/EllipticCurve/Q/";
    vector<std::string> pages = vector<std::string>();
    vector<vector<bool>> matrices = vector<vector<bool>>();
    pages.push_back("nawak");
    pages.push_back(path);
    explore(pages, 0);
    std::cout << "End" << std::endl;
    return 0;
}
