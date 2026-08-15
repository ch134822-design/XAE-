#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
===========================================================
                    XAE LANGUAGE ENGINE
===========================================================
0 = XAE
1 = XALON
2 = XABI
3 = ALONSO

Base 4: 3 positions => 64 combinations (4³ = 64)
  0..25 : A-Z
  26    : SPACE (' ')
  27    : PONTO FINAL ('.')
  28    : VÍRGULA (',')
  29    : EXCLAMAÇÃO ('!')
  30    : INTERROGAÇÃO ('?')
  31    : DOIS PONTOS (':')
  32    : PONTO E VÍRGULA (';')
  33    : HÍFEN ('-')
  34    : ABRE PARÊNTESE ('(')
  35    : FECHA PARÊNTESE (')')
  36..45: DÍGITOS 0-9
===========================================================
*/

static const char* TOKENS[4] = {
    "XAE",
    "XALON",
    "XABI",
    "ALONSO"
};

int token_to_value(const char* token) {
    if (strcmp(token, "XAE") == 0) return 0;
    if (strcmp(token, "XALON") == 0) return 1;
    if (strcmp(token, "XABI") == 0) return 2;
    if (strcmp(token, "ALONSO") == 0) return 3;
    return -1;
}

const char* value_to_token(int val) {
    if (val >= 0 && val < 4) return TOKENS[val];
    return "?";
}

int char_to_index(char c) {
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

char index_to_char(int idx) {
    if (idx >= 0 && idx <= 25) return (char)('A' + idx);
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
    if (idx >= 36 && idx <= 45) return (char)('0' + (idx - 36));
    return '?';
}

char* normalize_text(const char* src) {
    size_t len = strlen(src);
    char* res = (char*)malloc(len + 1);
    if (!res) return NULL;
    size_t out_idx = 0;

    for (size_t i = 0; i < len; i++) {
        unsigned char uc = (unsigned char)src[i];

        // Handle UTF-8 accents
        if (uc == 0xC3 && i + 1 < len) {
            unsigned char next = (unsigned char)src[++i];
            // Á, À, Â, Ã, Ä, á, à, â, ã, ä
            if (next >= 0x80 && next <= 0x85) { res[out_idx++] = 'A'; continue; }
            if (next >= 0xA0 && next <= 0xA5) { res[out_idx++] = 'A'; continue; }
            // É, È, Ê, Ë, é, è, ê, ë
            if (next >= 0x88 && next <= 0x8B) { res[out_idx++] = 'E'; continue; }
            if (next >= 0xA8 && next <= 0xAB) { res[out_idx++] = 'E'; continue; }
            // Í, Ì, Î, Ï, í, ì, î, ï
            if (next >= 0x8C && next <= 0x8F) { res[out_idx++] = 'I'; continue; }
            if (next >= 0xAC && next <= 0xAF) { res[out_idx++] = 'I'; continue; }
            // Ó, Ò, Ô, Õ, Ö, ó, ò, ô, õ, ö
            if (next >= 0x92 && next <= 0x96) { res[out_idx++] = 'O'; continue; }
            if (next >= 0xB2 && next <= 0xB6) { res[out_idx++] = 'O'; continue; }
            // Ú, Ù, Û, Ü, ú, ù, û, ü
            if (next >= 0x99 && next <= 0x9C) { res[out_idx++] = 'U'; continue; }
            if (next >= 0xB9 && next <= 0xBC) { res[out_idx++] = 'U'; continue; }
            // Ç, ç
            if (next == 0x87 || next == 0xA7) { res[out_idx++] = 'C'; continue; }
        }

        char c = (char)toupper(uc);
        res[out_idx++] = c;
    }
    res[out_idx] = '\0';
    return res;
}

void encode_text(const char* text) {
    char* norm = normalize_text(text);
    if (!norm) return;

    int first = 1;
    for (size_t i = 0; norm[i] != '\0'; i++) {
        int idx = char_to_index(norm[i]);
        if (idx == -1) continue;

        int x = idx / 16;
        int y = (idx / 4) % 4;
        int z = idx % 4;

        if (!first) {
            printf(", ");
        }
        printf("%s, %s, %s", value_to_token(x), value_to_token(y), value_to_token(z));
        first = 0;
    }
    printf("\n");
    free(norm);
}

void decode_text(const char* text) {
    size_t len = strlen(text);
    char* norm = (char*)malloc(len + 1);
    if (!norm) return;

    // Convert everything to uppercase and extract tokens
    for (size_t i = 0; i < len; i++) {
        norm[i] = (char)toupper((unsigned char)text[i]);
    }
    norm[len] = '\0';

    int val_buf[3];
    int count = 0;

    char tok[64];
    size_t t_idx = 0;

    for (size_t i = 0; i <= len; i++) {
        char c = (i < len) ? norm[i] : ' ';

        if (c >= 'A' && c <= 'Z') {
            if (t_idx < sizeof(tok) - 1) {
                tok[t_idx++] = c;
            }
        } else {
            if (t_idx > 0) {
                tok[t_idx] = '\0';
                int v = token_to_value(tok);
                if (v != -1) {
                    val_buf[count++] = v;
                    if (count == 3) {
                        int num = val_buf[0] * 16 + val_buf[1] * 4 + val_buf[2];
                        putchar(index_to_char(num));
                        count = 0;
                    }
                }
                t_idx = 0;
            }
        }
    }

    if (count == 3) {
        int num = val_buf[0] * 16 + val_buf[1] * 4 + val_buf[2];
        putchar(index_to_char(num));
    }

    printf("\n");
    free(norm);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Uso:\nxae_codec encode \"TEXTO\"\nxae_codec decode \"CODIGO XAE\"\n");
        return 1;
    }

    const char* mode = argv[1];
    const char* text = argv[2];

    if (strcmp(mode, "encode") == 0) {
        encode_text(text);
        return 0;
    }

    if (strcmp(mode, "decode") == 0) {
        decode_text(text);
        return 0;
    }

    fprintf(stderr, "Modo inválido.\n");
    return 1;
}
