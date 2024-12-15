#include "whup_parser.h"

#ifndef CAST_H
#define CAST_H

class Cast{  
    std::string type;
    std::string output;
    std::string input;
    public:
    Cast(std::vector<Token>& expr,Environment* env);

};


#endif 