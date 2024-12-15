#include "whup_parser.h"

class WInput {
    std::string input;
    public:
    WInput(std::vector<Token>& expr,Environment* env);
};