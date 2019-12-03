#include <iostream>
#include <vector>
#include <fstream>
#include <regex>

using namespace std;

std::string openFile(const std::string & path){
    std::string fPath = "curl " + path + " > temp/currentPage.txt";
    system(fPath.c_str());
    ifstream ifs("currentPage.txt");
    std::string result;
    std::string line;
    while(getline(ifs, line))
        result+=line;
    return result;
}

void csv (const vector<vector<bool>> & adjs)
{
    std::ofstream ofs ("test.csv");
    cout << "Exportation de la matrice...";
    for (size_t row = 0; row < adjs.size(); ++row)
    {
        for (size_t column = 0; column < adjs[row].size(); ++column)
        {
            ofs << adjs[row][column] << ";";
        }
        ofs << "\n";
    }
    cout << " Ok" << endl;
}

void explore(vector<std::string> & pages, vector<vector<bool>> & adjs, const unsigned & current){
    if(pages.size() == current) return;
    std::string path = pages.at(current);
    std::string page = openFile(path);
    std::regex reg("<a href=\"[^\"]+\"");
    smatch matches;
    string::const_iterator searchStart( page.cbegin() );
    while ( regex_search( searchStart, page.cend(), matches, reg ) ) {
       // pages.push_back(matches[0]);
        searchStart = matches.suffix().first;
    }
    explore(pages, adjs, current+1);
}

int main()
{
    std::string path = "https://www.lmfdb.org/EllipticCurve/Q/";
    vector<std::string> pages = vector<std::string>();
    vector<vector<bool>> matrices (1000, vector<bool> (1000));
    vector<vector<bool>> adjs = vector<vector<bool>>();
    pages.push_back("nawak");
    pages.push_back(path);
    explore(pages, adjs, 1);


    csv (matrices);

    return 0;
}
