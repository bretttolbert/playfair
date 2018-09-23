#include "scoretext.h"
#include "qgram_data_en.h"
#include "qgram_data_fr.h"
#include <algorithm>
#include <random>

namespace scoretext
{
    const std::string ASCII_UPPERCASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    std::string rnd_string(size_t len)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 25);

        std::string s;
        for (size_t i=0; i<len; ++i)
        {
            s += ASCII_UPPERCASE[dis(gen)];
        }
        return s;
    }

    std::string fmt_upper_only(const std::string& text)
    {
        std::string s(text);
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
        std::string out;
        for (char c : s)
        {
            if (ASCII_UPPERCASE.find(c) != std::string::npos)
            {
                out += c;
            }
        }
        return out;
    }

    size_t get_qgram_stats_array_idx(const std::string& qgram)
    {
        char temp[4];
        temp[0]=qgram[0]-'A';
        temp[1]=qgram[1]-'A';
        temp[2]=qgram[2]-'A';
        temp[3]=qgram[3]-'A';
        return 17576*temp[0] + 676*temp[1] + 26*temp[2] + temp[3];      
    }

    double score_text_qgram(const std::string& text, const std::string& lang)
    {
        double score = 0;
        for (size_t i=0; i<text.length()-3; i++)
        {
            std::string qgram = text.substr(i, 4);
            size_t idx = get_qgram_stats_array_idx(qgram);
            if (lang == "en")
                score += qgram_stats_array_en[idx];
            else if (lang == "fr")
                score += qgram_stats_array_fr[idx];
            else
                throw InvalidLangException();
        }
        return score;
    }
}