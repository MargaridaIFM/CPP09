#ifndef RPN_HPP
#define RPN_HPP

#include <string>

class RPN {
public:
    static int evaluate(const std::string &expr, bool &ok);
};

#endif
