#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

const vector<string> TOKENS = {
    "XAE",
    "XALON",
    "XABI",
    "ALONSO"
};

int tokenToValue(const string& token) {
    if (token == "XAE") return 0;
    if (token == "XALON") return 1;
    if (token == "XABI") return 2;
    if (token == "ALONSO") return 3;
    return -1;
}

string valueToToken(int value) {
    if (value >= 0 && value < 4) return TOKENS[value];
    return "?";
}

int charToIndex(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c == ' ') return 26;
    if (c == '.') return 27;
    if (c == ',') return 28;
    if (c == '!') return 29;
    if (c == '?') return 30;
    if (c == ':') return 31;
    if (c == ';') return 32;
    if (c == '-') return 33;
    if (c == '(') return 34;
    if (c == ')') return 35;
    if (c >= '0' && c <= '9') return 36 + (c - '0');
    return -1;
}

char indexToChar(int idx) {
    if (idx >= 0 && idx <= 25) return static_cast<char>('A' + idx);
    if (idx == 26) return ' ';
    if (idx == 27) return '.';
    if (idx == 28) return ',';
    if (idx == 29) return '!';
    if (idx == 30) return '?';
    if (idx == 31) return ':';
    if (idx == 32) return ';';
    if (idx == 33) return '-';
    if (idx == 34) return '(';
    if (idx == 35) return ')';
    if (idx >= 36 && idx <= 45) return static_cast<char>('0' + (idx - 36));
    return '?';
}

string normalizeText(const string& text) {
    string res;
    size_t len = text.size();
    for (size_t i = 0; i < len; i++) {
        unsigned char uc = static_cast<unsigned char>(text[i]);
        if (uc == 0xC3 && i + 1 < len) {
            unsigned char next = static_cast<unsigned char>(text[++i]);
            if ((next >= 0x80 && next <= 0x85) || (next >= 0xA0 && next <= 0xA5)) { res += 'A'; continue; }
            if ((next >= 0x88 && next <= 0x8B) || (next >= 0xA8 && next <= 0xAB)) { res += 'E'; continue; }
            if ((next >= 0x8C && next <= 0x8F) || (next >= 0xAC && next <= 0xAF)) { res += 'I'; continue; }
            if ((next >= 0x92 && next <= 0x96) || (next >= 0xB2 && next <= 0xB6)) { res += 'O'; continue; }
            if ((next >= 0x99 && next <= 0x9C) || (next >= 0xB9 && next <= 0xBC)) { res += 'U'; continue; }
            if (next == 0x87 || next == 0xA7) { res += 'C'; continue; }
        }
        res += static_cast<char>(toupper(uc));
    }
    return res;
}

string encodeText(const string& text) {
    string norm = normalizeText(text);
    string result;
    bool first = true;

    for (char c : norm) {
        int idx = charToIndex(c);
        if (idx == -1) continue;

        int x = idx / 16;
        int y = (idx / 4) % 4;
        int z = idx % 4;

        if (!first) result += ", ";
        result += valueToToken(x) + ", " + valueToToken(y) + ", " + valueToToken(z);
        first = false;
    }
    return result;
}

string decodeText(const string& text) {
    string result;
    vector<int> buffer;
    string tok;

    for (size_t i = 0; i <= text.size(); i++) {
        char c = (i < text.size()) ? static_cast<char>(toupper(static_cast<unsigned char>(text[i]))) : ' ';

        if (c >= 'A' && c <= 'Z') {
            tok += c;
        } else {
            if (!tok.empty()) {
                int v = tokenToValue(tok);
                if (v != -1) {
                    buffer.push_back(v);
                    if (buffer.size() == 3) {
                        int num = buffer[0] * 16 + buffer[1] * 4 + buffer[2];
                        result += indexToChar(num);
                        buffer.clear();
                    }
                }
                tok.clear();
            }
        }
    }

    if (buffer.size() == 3) {
        int num = buffer[0] * 16 + buffer[1] * 4 + buffer[2];
        result += indexToChar(num);
    }

    return result;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Uso:\nxae_codec encode \"TEXTO\"\nxae_codec decode \"CODIGO XAE\"\n";
        return 1;
    }

    string mode = argv[1];
    string text = argv[2];

    if (mode == "encode") {
        cout << encodeText(text) << endl;
        return 0;
    }

    if (mode == "decode") {
        cout << decodeText(text) << endl;
        return 0;
    }

    cerr << "Modo inválido." << endl;
    return 1;
}
