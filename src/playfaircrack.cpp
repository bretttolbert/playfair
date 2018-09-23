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

    double crack_iter(const std::vector<std::string>& digraphs, std::string& bestkey, const std::string& lang)
    {
        std::string maxkey = bestkey;
        std::string plaintext = decipher_digraphs(digraphs, gen_cipher_table(maxkey));
        double maxscore = scoretext::score_text_qgram(plaintext, lang);
        double bestscore = maxscore;
        for (float T = START_TEMPERATURE; T >= 0; T-=TEMPERATURE_STEP)
        {
            for (size_t count = 0; count < ITERATIONS; count++)
            { 
                std::string testkey = rnd_mod(maxkey);    
                std::string plaintext = decipher_digraphs(digraphs, gen_cipher_table(testkey));
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
        std::vector<std::string> digraphs = to_digraphs(ciphertext);
        std::cout << "Attemping to crack:\n" << ciphertext << std::endl;
        std::cout << "Presumed language: " << lang << std::endl;
        std::cout << "This may take a few minutes.\n";
        std::string key = CIPHER_ALPHABET;

        std::string plaintext = decipher_digraphs(digraphs, gen_cipher_table(key));
        double maxscore = scoretext::score_text_qgram(plaintext, lang);
        std::cout << "Initial score: " << maxscore << std::endl;
        std::cout << "    Key: " << key << std::endl;
        std::cout << "    Plaintext: " << plaintext << std::endl;

        for (size_t i=0; ; i++)
        {
            double score = crack_iter(digraphs, key, lang);
            if (score > maxscore)
            {
                maxscore = score;
                std::string plaintext = decipher_digraphs(digraphs, gen_cipher_table(key));
                std::cout << "Best score so far: " << score << ", on iteration " << i << std::endl;
                std::cout << "    Key: " << key << std::endl;
                std::cout << "    Plaintext: " << plaintext << std::endl;
            }
        }
    }
}