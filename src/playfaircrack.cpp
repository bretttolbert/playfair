#include "playfaircrack.h"
#include "playfair.h"
#include "scoretext.h"
#include <algorithm>
#include <functional>
#include <cmath>
#include <iostream>

namespace playfair
{
    std::string swap_two_letters(const std::string& key)
    {
        std::string s(key);
        int i = rand()%25;
        int j = rand()%25;
        char temp = key[i];
        s[i] = s[j];
        s[j] = temp;
        return s;
    }

    std::string swap_two_rows(const std::string& key)
    {
        std::string s(key);
        int i = rand()%5;
        int j = rand()%5;
        char temp;
        int k;
        for(k=0; k<5; k++)
        {
            temp = s[i*5 + k];
            s[i*5 + k] = s[j*5 + k];
            s[j*5 + k] = temp;
        }
        return s;
    }

    std::string swap_two_cols(const std::string& key)
    {
        std::string s(key);
        int i = rand()%5;
        int j = rand()%5;
        char temp;
        int k;
        for (k=0; k<5; k++)
        {
            temp = s[k*5 + i];
            s[k*5 + i] = s[k*5 + j];
            s[k*5 + j] = temp;
        }
        return s;
    }

    std::string reverse_whole_key(const std::string& key)
    {
        std::string s(key);
        std::reverse(s.begin(), s.end());
        return s;
    }

    std::string swap_rows_up_down(const std::string& key)
    {
        std::string s(key);
        for (int i=0; i<5; i++)
        {
            for (int j=0; j<5; j++)
            {
                s[i*5 + j] = key[(4-i)*5+j];
            }
        }
        return s;
    }

    std::string swap_cols_left_right(const std::string& key)
    {
        std::string s(key);
        for (int i=0; i<5; i++)
        {
            for (int j=0; j<5; j++)
            {
                s[j*5 + i] = key[(4-j) * 5+i];
            }
        }
        return s;
    }
    
    std::string rnd_mod(const std::string& key)
    {
        int i = rand()%50;
        switch(i)
        {
            case 0:
                return swap_two_rows(key);
            case 1:
                return swap_two_cols(key);     
            case 2:
                return reverse_whole_key(key);
            case 3:
                return swap_rows_up_down(key);
            case 4:
                return swap_two_cols(key);
            default:
                return swap_two_letters(key);
        }
    }

    double crack_iter(const std::string& ciphertext, std::string& bestkey, const std::string& lang)
    {
        std::string maxkey = bestkey;
        double maxscore = scoretext::score_text_qgram(decipher(ciphertext, maxkey), lang);
        double bestscore = maxscore;
        for (float T = START_TEMPERATURE; T >= 0; T-=TEMPERATURE_STEP)
        {
            for (size_t count = 0; count < ITERATIONS; count++)
            { 
                std::string testkey = rnd_mod(maxkey);    
                std::string plaintext = decipher(ciphertext, testkey);
                double score = scoretext::score_text_qgram(plaintext, lang);
                double dF = score - maxscore;
                if (dF >= 0)
                {
                    maxscore = score;
                    maxkey = testkey;
                }
                else if(T > 0)
                {
                    // simulated annealing
                    double prob = exp(dF/T);
                    if (prob > 1.0*rand()/RAND_MAX)
                    {
                        maxscore = score;
                        maxkey = testkey;              
                    }
                }
                if (maxscore > bestscore)
                {
                    bestscore = maxscore;
                    bestkey = maxkey;
                }
            }
        }
        return bestscore;
    }

    double crack(const std::string& ciphertext, std::string lang)
    {
        std::string text = fmt_ciphertext(ciphertext);
        std::cout << "Attemping to crack:\n" << text << std::endl;
        std::cout << "This may take a few minutes.\n";
        double maxscore = -99e99;
        std::string key = CIPHER_ALPHABET;
        for (size_t i=0; ; i++)
        {
            double score = crack_iter(text, key, lang);
            if (score > maxscore)
            {
                maxscore = score;
                std::string plaintext = decipher(text, key);
                std::cout << "Best score so far: " << score << ", on iteration " << i << std::endl;
                std::cout << "    Key: " << key << std::endl;
                std::cout << "    Plaintext: " << plaintext << std::endl;
            }
        }
    }
}