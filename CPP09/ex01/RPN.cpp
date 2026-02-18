#include "RPN.hpp"

#include <stack>
#include <sstream>
#include <cctype>

static bool isSingleDigit(const std::string &t) {
    return (t.size() == 1 && std::isdigit(static_cast<unsigned char>(t[0])));
}

static bool isOp(const std::string &t) {
    return (t.size() == 1 && (t[0] == '+' || t[0] == '-' || t[0] == '*' || t[0] == '/'));
}

int RPN::evaluate(const std::string &expr, bool &ok) {
    ok = false;
    std::stack<long> st;

    std::istringstream iss(expr);
    std::string tok;
    while (iss >> tok) {
        if (isSingleDigit(tok)) {
            st.push(tok[0] - '0');
        } else if (isOp(tok)) {
            if (st.size() < 2) return 0;
            long b = st.top(); st.pop();
            long a = st.top(); st.pop();

            long r = 0;
            switch (tok[0]) {
                case '+': r = a + b; break;
                case '-': r = a - b; break;
                case '*': r = a * b; break;
                case '/':
                    if (b == 0) return 0;
                    r = a / b;
                    break;
            }
            st.push(r);
        } else {
            return 0;
        }
    }

    if (st.size() != 1) return 0;
    ok = true;
    return static_cast<int>(st.top());
}
