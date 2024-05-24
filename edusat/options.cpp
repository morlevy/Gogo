#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include "options.h"
using namespace std;


void Abort(string s, int i) {
	cout << endl << "Abort: ";
	switch (i) {
	case 1: cout << "(input error)" << endl; break;
	case 2: cout << "command line arguments error" << endl; break;
	case 3: break;
	default: cout << "(exit code " << i << ")" << endl; break;
	}
	cout << s << endl;
	exit(i);
}

template<typename T> bool Tparse(string st, T val, T lb, T ub, T* var) {
	if (val < lb || val > ub)
		Abort(st + string(" value not in range"), 2);
	*var = val;	
	return true;
}

bool intoption::parse(string st) {
	int val;
	try { val = stoi(st); }
	catch (...) {Abort("value " + st + " not numeric", 1); }
	return Tparse<int>(st, val, lb, ub, p_to_var);
};

bool doubleoption::parse(string st) {
	double val;
	try { val = stod(st); }
	catch (...) { Abort("value " + st + " not double", 1); }
	return Tparse<double>(st, val, lb, ub, p_to_var);
};

extern int v2l(int v);

bool vec_lit_option::parse(string st) {
    stringstream ss(st);
    string s;
    vector<Lit>& v = *p_to_var;
    while (ss >> s) {
        try {
            int val = stoi(s);
            v.push_back(v2l(val));
        }
        catch (...) {
            Abort("value " + s + " not numeric", 1);
        }
    }
    return true;
}

extern unordered_map<string, option*> options;

extern int l2rl(int l);

void help() {
	stringstream st;
	st << "\nUsage: edusat <options> <file name>\n \n"
		"Options:\n";
	for (const auto& h : options) {
		option* opt = options[h.first];
		st << left << setw(16) << "-" + h.first;
		st << opt->msg << ". Default: " << opt->val() << "." << endl;
	}
	Abort(st.str(), 3);
}

void parse_options(int argc, char** argv) {
	if (argc == 1 || string(argv[1]) == "-h")
		help();
	for (int i = 1; i < argc - 1; ++i) {
		string st = argv[i] + 1;
		if (argv[i][0] != '-' || options.count(st) == 0) {
			cout << st << endl;
			Abort("Unknown flag ", 2);
		}
		if (i == argc - 2) Abort(string("missing value after ") + st, 2);
		i++;
		options[st]->parse(argv[i]);
        cout << "DEBUG: " << options[st]->val() << endl;
	}
	cout << argv[argc - 1] << endl;
}
