#include "playfair.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <regex>

namespace playfair
{
    bool is_valid_key(const std::string& key)
    {
        std::string used;
        for (char c : key)
        {
            if (used.find(c) != std::string::npos 
            || CIPHER_ALPHABET.find(c) == std::string::npos)
            {
                return false;
            }
            used += c;
        }
        return true;
    }

    using CharMap = std::vector<std::pair<std::string, std::vector<std::string>>>;
    CharMap CHAR_REPLACEMENTS {
        {"A", {"\u00C0", "\u00C1", "\u00C2", "\u00C3", "\u00C4", "\u00C5"}},
        {"AE", {"\u00C6"}},
        {"C", {"\u00C7"}},
        {"E", {"\u00C8", "\u00C9", "\u00CA", "\u00CB"}},
        {"I", {"\u00CC", "\u00CD", "\u00CE", "\u00CF"}},
        {"N", {"\u00D1"}},
        {"O", {"\u00D2", "\u00D3", "\u00D4", "\u00D5", "\u00D6", "\u00D8"}},
        {"U", {"\u00D9", "\u00DA", "\u00DB", "\u00DC"}},
        {"Y", {"\u00DD"}},
        {"SS", {"\u00DF"}},
        {"a", {"\u00E0", "\u00E1", "\u00E2", "\u00E3", "\u00E4", "\u00E5"}},
        {"ae", {"\u00E6"}},
        {"c", {"\u00E7"}},
        {"e", {"\u00E8", "\u00E9", "\u00EA", "\u00EB"}},
        {"i", {"\u00EC", "\u00ED", "\u00EE", "\u00EF"}},
        {"n", {"\u00F1"}},
        {"o", {"\u00F2", "\u00F3", "\u00F4", "\u00F5", "\u00F6", "\u00F8"}},
        {"u", {"\u00F9", "\u00FA", "\u00FB", "\u00FC"}},
        {"y", {"\u00FD", "\u00ff"}},
        {"th", {"\u00F0", "\u00FE"}}
    };

    std::string strip_accents(const std::string& text)
    {
        std::string s(text);
        for (CharMap::const_iterator it = CHAR_REPLACEMENTS.begin(); it != CHAR_REPLACEMENTS.end(); ++it)
        {
            for (std::string codepoint : it->second)
            {
                s = std::regex_replace(s, std::regex(codepoint), it->first);
            }
        }
        return s;
    }

    std::vector<std::string> to_digraphs(const std::string &text)
    {
        std::vector<std::string> digraphs;
        std::string s = text;
        s = strip_accents(s);
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
        s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
        std::replace(s.begin(), s.end(), 'J', 'I');

        s.erase(std::remove_if(s.begin(), 
                               s.end(), 
                               [](unsigned char c){return CIPHER_ALPHABET.find(c) == std::string::npos;}), 
                s.end());

        std::string dg;
        for (char c : s)
        {
            if (dg.length() == 1 && c == dg[0])
            {
                dg += 'X';
                digraphs.push_back(dg);
                dg = c;
            }
            else
            {
                dg += c;
            }
            if (dg.length() == 2)
            {
                digraphs.push_back(dg);
                dg.clear();
            }
        }
        if (dg.length() == 1)
        {
            dg += 'X';
            digraphs.push_back(dg);
        }
        return digraphs;
    }

    std::string gen_cipher_table_string(const std::string& key)
    {
        if (!is_valid_key(key))
        {
            throw InvalidKeyException();
        }
        std::string s(CIPHER_ALPHABET);
        for (char c : key)
        {
            s.erase(std::remove(s.begin(), s.end(), c), s.end());
        }
        return key + s;
    }

    CipherTable gen_cipher_table(const std::string& key)
    {
        std::string s = gen_cipher_table_string(key);
        CipherTable tbl;
        for (int i=0; i<5; ++i)
        {
            tbl.push_back(s.substr(i*5, 5));
        }
        return tbl;
    }

    RowCol get_letter_row_col(char c, const CipherTable& tbl)
    {
        RowCol result;
        for (size_t i=0; i<tbl.size(); ++i)
        {
            std::string row = tbl[i];
            for (size_t j=0; j<row.length(); ++j)
            {
                if (tbl[i][j] == c)
                {
                    result = {i, j};
                }
            }
        }
        return result;
    }

    size_t shift_letter_idx(size_t idx, bool decipher)
    {
        if (decipher && idx == 0)
            idx = 4;
        else if (!decipher && idx == 4)
            idx = 0;
        else
            idx += (decipher) ? -1 : 1;
        return idx;
    }

    std::string encipher_digraph(const std::string& digraph, const CipherTable& tbl, bool decipher)
    {
        RowCol l0 = get_letter_row_col(digraph[0], tbl);
        RowCol l1 = get_letter_row_col(digraph[1], tbl);
        std::string s;
        // case 1: both letters in same row
        if (l0.first == l1.first)
        {
            s += tbl[l0.first][shift_letter_idx(l0.second, decipher)];
            s += tbl[l1.first][shift_letter_idx(l1.second, decipher)];
        }
        // case 2: both letters in same col
        else if (l0.second == l1.second)
        {
            s += tbl[shift_letter_idx(l0.first, decipher)][l0.second];
            s += tbl[shift_letter_idx(l1.first, decipher)][l1.second];
        }
        // case 3: letters in different row and col
        else
        {
            s += tbl[l0.first][l1.second];
            s += tbl[l1.first][l0.second];
        }
        return s;
    }

    std::string encipher(const std::string& plaintext, const std::string &key, bool decipher)
    {
        std::vector<std::string> digraphs = to_digraphs(plaintext);
        return encipher_digraphs(digraphs, gen_cipher_table(key), decipher);
    }

    std::string decipher(const std::string& ciphertext, const std::string &key)
    {
        return encipher(ciphertext, key, true);
    }

    std::string decipher_digraphs(const std::vector<std::string>& digraphs, const CipherTable& key)
    {
        return encipher_digraphs(digraphs, key, true);
    }

    std::string encipher_digraphs(const std::vector<std::string>& digraphs, const CipherTable& key, bool decipher)
    {
        std::string ciphertext;
        for (std::string dg : digraphs)
        {
            ciphertext += encipher_digraph(dg, key, decipher);
        }
        return ciphertext;
    }

    bool is_valid_ciphertext(const std::string& ciphertext)
    {
        return ciphertext.find_first_not_of(CIPHER_ALPHABET) == std::string::npos;
    }

    std::string fmt_ciphertext(const std::string& ciphertext)
    {
        std::string s(ciphertext);
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
        s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
        return s;
    }
}