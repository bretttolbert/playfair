#ifndef SCORETEXT_H
#define SCORETEXT_H
#include <string>

/**
 * Source: 
 * http://practicalcryptography.com/cryptanalysis/stochastic-searching/cryptanalysis-playfair/
 * http://practicalcryptography.com/cryptanalysis/text-characterisation/quadgrams/
 * http://practicalcryptography.com/cryptanalysis/letter-frequencies-various-languages/
 */
extern float qgram_stats_array_en[];
extern float qgram_stats_array_fr[];

namespace scoretext
{
    struct InvalidLangException: public std::exception
    {
        const char* what() const throw()
        {
            return "Invalid language";
        }
    };

    struct InvalidTextException: public std::exception
    {
        const char* what() const throw()
        {
            return "Invalid text";
        }
    };

    /**
     * Returns a random string of the given length consisting only of uppercase letters
     */
    std::string rnd_string(size_t len);

    /**
     * Converts to uppercase and removes everything except for uppercase letters
     */
    std::string fmt_upper_only(const std::string& text);

    /**
     * Gets the array index for the specified quagram
     */
    size_t get_qgram_stats_array_idx(const std::string& qgram);

    /**
     * Returns a fitness measure of text based on quadgram statistics for the given language
     * Strings must be same length for a meaningful comparison.
     * Assumes that text consists only of uppercase letters (no punctuation or spaces)
     */
    double score_text_qgram(const std::string& text, const std::string& lang);
}

#endif