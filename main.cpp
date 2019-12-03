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

void explore(vector<std::string> & pages, vector<vector<bool>> & adjs, const unsigned & current){
    if(pages.size() == current) return;
    std::string path = pages.at(current);
    std::string page = openFile(path);


    explore(pages, adjs, current+1);
}

int main()
{
    std::string path = "https://www.lmfdb.org/EllipticCurve/Q/";
    vector<std::string> pages = vector<std::string>();
    vector<vector<bool>> adjs = vector<vector<bool>>();
    pages.push_back("nawak");
    pages.push_back(path);
    explore(pages, adjs, 1);

    string var = "first second third forth";

    const regex r("(.*)");
    smatch sm;

    if (regex_search(var, sm, r))
    {
        for (int i=1; i<sm.size(); i++)
        {
            cout << sm[i] << endl;
        }
    }


    std::cout << "End" << std::endl;
    return 0;
}
