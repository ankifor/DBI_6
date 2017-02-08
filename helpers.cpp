#include <string>

using namespace std;

string ReplaceString(string subject, const string& search, const string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

void ReplaceStringInPlace(string& subject, const string& search, const string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}

size_t log2(size_t x) {
	return sizeof(x)*8 - __builtin_clzl(x);
}

bool subsetOf(size_t left, size_t right) {
	size_t tmp = ~left | right;
	const size_t all = ~size_t(0);
	return tmp == all;
}

bool bitSetTest(size_t src, size_t bit) {
	size_t res = (src >> bit) & 1;
	return res == 1;
}
