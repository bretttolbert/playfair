#include <iostream>
#include <string>
#include "config.h"
#include "playfaircrack.h"

/*
std::string TEST_CIPHER_TEXT = 
"ZK DW KC SE XM ZK DW VF RV LQ VF WN ED MZ LW QE GY VF KD XF MP WC GO "
"BF MU GY QF UG ZK NZ IM GK FK GY ZS GQ LN DP AB BM CK OQ KL EZ KF DH "
"YK ZN LK FK EU YK FK KZ RY YD FT PC HD GQ MZ CP YD KL KF EZ CI ON DP "
"AC WK QS SY QL UN DU RU GY NS";
*/

std::string TEST_CIPHER_TEXT = 
"XZOGQRWVQWNROKCOAELBXZWGEQYLGDRZXYZRQAEKLRHDUMNUXYXSXYEMXEHDGNXZYNT"
"ZONYELBEUGYSCOREUSWTZRLRYBYCOLZYLEMWNSXFBUSDBORBZCYLQEDMHQRWVQWAEDPG"
"DPOYHORXZINNYWPXZGROKCOLCCOCYTZUEUIICERLEVHMVQWLNWPRYXHGNMLEKLRHDUYS"
"UCYRAWPUYECRYRYXHGNBLUYSCCOUYOHRYUMNUXYXSXYEMXEHDGN";

using namespace playfair;

void show_usage(const std::string& progname)
{
    std::cout << "Usage:\n";
    std::cout << progname << " --encipher <plaintext> <key>\n";
    std::cout << progname << " --decipher <ciphertext> <key>\n";
    std::cout << progname << " --crack <ciphertext> [--lang en|fr]\n";
    std::cout << "Advanced options:\n";
    std::cout << "--sub <omitted_letter><omitted_letter_substitute> (default is JI)\n";
    std::cout << "--sep <separator_letter> (default is X)\n";
}

int main(int argc, char** argv)
{
    srand((unsigned)time(NULL));

    std::string progname = argv[0];
    if (argc < 3)
    {
        show_usage(progname);
        return 0;
    }

    std::string lang = "en";

    std::string operation = argv[1];
    for (size_t i=3; i+1<argc; ++i)
    {
        std::string arg(argv[i]);
        std::string val(argv[i+1]);
        if (arg == "--lang" || arg == "--langue")
        {
            lang = val;
        }
        else if (arg == "--sub")
        {
            if (val.length() != 2 || val.find_first_not_of(ASCII_UPPERCASE) != std::string::npos)
            {
                std::cerr << "Invalid value for --sub argument.\n";
                std::cerr << "Expected two uppercase letters (omitted_letter_substitute and omitted_letter) e.g. --sub WV\n";
                return -1;
            }
            omitted_letter = val[0];
            omitted_letter_sub = val[1];
        }
        else if (arg == "--sep")
        {
            if (val.length() != 1 || ASCII_UPPERCASE.find(val) == std::string::npos)
            {
                std::cerr << "Invalid value for --sep argument.\n";
                std::cerr << "Expected an uppercase letter e.g. --sep Q\n";
                return -1;
            }
            separator_letter = val[0];
        }
    }

    if (operation == "--encipher" || operation == "--chiffrer")
    {
        std::string plaintext = argv[2];
        std::string key = argv[3];
        std::string ciphertext = encipher(plaintext, key);
        std::cout << "Ciphertext:\n";
        std::cout << ciphertext << std::endl;
    }
    else if (operation == "--decipher" || operation == "--dechiffrer")
    {
        std::string ciphertext = argv[2];
        std::string key = argv[3];
        std::string plaintext = decipher(ciphertext, key);
        std::cout << "Deciphered:\n";
        std::cout << plaintext << std::endl;
    }
    else if (operation == "--crack" || operation == "--craquer")
    {
        std::string ciphertext = argv[2];
        crack(ciphertext, lang);
    }
    else
    {
        show_usage(progname);
    }

    return 0;
}
