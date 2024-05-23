#pragma once

#include <string>
#include <vector>

using namespace std;
using Lit = int;

class option {
public:
    option(string _msg) : msg(_msg) {};
    string msg;

    virtual bool parse(string) = 0;

    virtual string val() = 0;
};

class intoption : public option {
public:
    intoption(int *p, int _lb, int _ub, string _msg) : option(_msg),
                                                       p_to_var(p), lb(_lb), ub(_ub) {};
    int *p_to_var; // pointer to the variable holding the option value.
    int lb; // lower-bound
    int ub; // upper-bound
    bool parse(string st);

    string val() { return to_string(*p_to_var); }
};

class doubleoption : public option {
public:
    doubleoption(double *p, double _lb, double _ub, string _msg) : option(_msg),
                                                                   p_to_var(p), lb(_lb), ub(_ub) {}

    double *p_to_var; // pointer to the variable holding the option value.
    double lb; // lower-bound
    double ub; // upper-bound
    bool parse(string st);

    string val() { return to_string(*p_to_var); }
};

class vec_lit_option : public option {
public:
    vec_lit_option(std::vector<Lit> *p, const char* _msg) : option(_msg),
                                                       p_to_var(p) {};

    std::vector<Lit> *p_to_var; // pointer to the variable holding the option value.
    bool parse(string st);
    string val() {
        string s;
        for (Lit l : *p_to_var) {
            s += to_string(l) + " ";
        }
        return s;
    }
};
void Abort(string s, int i);

void parse_options(int argc, char **argv);
