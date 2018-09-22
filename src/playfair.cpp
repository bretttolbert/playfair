#include "playfair.h"
#include <iostream>
#include <algorithm>

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

    std::vector<std::string> plaintext_to_digraphs(const std::string &plaintext)
    {
        std::vector<std::string> digraphs;
        std::string s = plaintext;
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
        s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
        std::replace(s.begin(), s.end(), 'J', 'I');
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
        CipherTable tbl = gen_cipher_table(key);
        std::vector<std::string> digraphs = plaintext_to_digraphs(plaintext);
        std::string ciphertext;
        for (std::string dg : digraphs)
        {
            ciphertext += encipher_digraph(dg, tbl, decipher);
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

    std::string decipher(const std::string& ciphertext, const std::string &key)
    {
        std::string s = fmt_ciphertext(ciphertext);
        if (!is_valid_ciphertext(s))
        {
            throw InvalidCiphertextException();
        }
        return encipher(s, key, true);
    }
}