#include <string>
#include <iostream>
#include <algorithm>
#include <regex>

const std::basic_regex<char> add_sub("^(([-+]|--)?[^-+]+[-+])");
const std::basic_regex<char> mul_div("^[^*/]+[*/]");
const std::basic_regex<char> check("^[+*/]|[-+*/]$|[^-0-9+*/]|[-+*/][+*/]|[-+*/]{3,}|/0");

int64_t calc(std::string str){
    std::smatch res;
    std::regex_search(str, res, add_sub);
    if(res.size()){
        int64_t left = calc(res.str().substr(0, res.str().length() - 1));
        int64_t right = calc(str.substr(res.str().length()-1, str.length()));
        return left + right;
    }
    std::regex_search(str, res, mul_div);
    if(res.size()) {
        int64_t left = calc(str.substr(0, res.str().length() - 1));
        int64_t right = calc(str.substr(res.str().length(), str.length() - 1));
       return res.str().back() == '*' ? left * right : left / right;
    }
    
    if(str.size() > 1 && !isdigit(str[1])) {
        char c = str[1] == str[0] ? '+': '-';
        str = str.substr(1,str.size());
        str[0] = c;
    }
    return std::stoi(str);
}

int main(int argc, char** argv) {
    if(argc != 2){
        std::cout<<"error";
        return 1;
    }
    std::string str(argv[1]);
    std::smatch res;
    str.erase(std::remove_if(str.begin(), str.end(), isspace), str.end());
    if(std::regex_search(str, check)) {
        std::cout<<"error";
        return 1;
    }
    std::cout<<calc(str);
    return 0;
}