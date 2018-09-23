#pragma once
#include <string>
#include <vector>
#include <exception>

namespace playfair
{
    struct InvalidKeyException: public std::exception
    {
        const char* what() const throw()
        {
            return "Invalid cipher key";
        }
    };

    const std::string CIPHER_ALPHABET = "ABCDEFGHIKLMNOPQRSTUVWXYZ";

    bool is_valid_key(const std::string& key);

    std::string strip_accents(const std::string& text);

    std::vector<std::string> to_digraphs(const std::string& text);

    std::string gen_cipher_table_string(const std::string& key);

    using CipherTable = std::vector<std::string>;
    CipherTable gen_cipher_table(const std::string& key);

    using RowCol = std::pair<size_t, size_t>;
    RowCol get_letter_row_col(char c, const CipherTable& tbl);

    size_t shift_letter_idx(size_t idx, bool decipher);

    std::string encipher_digraph(const std::string& digraph, const CipherTable& tbl, bool decipher=false);

    std::string encipher(const std::string& plaintext, const std::string& key, bool decipher=false);
    std::string decipher(const std::string& ciphertext, const std::string& key);

    std::string decipher_digraphs(const std::vector<std::string>& digraphs, const CipherTable& key);
    std::string encipher_digraphs(const std::vector<std::string>& digraphs, const CipherTable& key, bool decipher=false);

    bool is_valid_ciphertext(const std::string& ciphertext);

    std::string fmt_ciphertext(const std::string& ciphertext);


}