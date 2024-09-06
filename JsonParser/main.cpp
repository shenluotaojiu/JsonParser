#include<iostream>
#include<fstream>
#include<sstream>
#include"json.h"

using namespace sltj::json;
int main() {
    std::cout << "Json" << std::endl;   

    std::ifstream fin("./test.json");
    std::stringstream ss;
    ss << fin.rdbuf();

    Json v;
    v.parser(ss.str());
    

    std::cout << v.str() << std::endl;
    

    return 0;
}