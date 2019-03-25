    #include <string>
    #include <iostream>
    #include <algorithm>
    #include <regex>

    const std::basic_regex<char> add_sub("^.?[^-+]+[-+]");
    const std::basic_regex<char> mul_div("^[^*/]+[*/]");
    const std::basic_regex<char> check("^[+*/]|[-+*/]$|[^-0-9+*/]|[-+*/][+*/]|[-+*/]{3,}|/0");

    int64_t calc(std::string::iterator first, std::string::iterator last){
        std::match_results<std::string::iterator> res;
        if(std::regex_search(first, last, res, add_sub)){
            int64_t left = calc(res[0].first, res[0].second - 1);
            int64_t right = calc(first + res.length() - 1, last);
            return left + right;
        }
        if(std::regex_search(first, last, res, mul_div)) {
            int64_t left = calc(res[0].first, res[0].second - 1);
            int64_t right = calc(first + res[0].length(), last);
        return (*(res[0].second - 1)) == '*' ? left * right : left / right;
        }
        
        int64_t atomic_val = 0;
        int sign = 1;
        if(!isdigit(*first)){
            sign = (*first) == '+' ? 1 : -1;
            ++first;
        }
        while(std::distance(first, last)) {
                atomic_val = atomic_val * 10 + (*first) - '0';
            ++first;
        }
        return atomic_val * sign;
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
        str = std::regex_replace(str, std::regex("--"), "+");
        str = std::regex_replace(str, std::regex("\\+-"), "-");
        std::cout<<calc(str.begin(), str.end());
        return 0;
    }