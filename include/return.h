#include "whup_parser.h"

class Return {

    private:
    Environment *env;

    public:
    Return(Environment *env);
    Return(std::vector<Token> tokens,Environment *env);
};

