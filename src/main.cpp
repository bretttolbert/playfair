#include <iostream>
#include <string>
#include "config.h"
#include "playfair.h"
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
    std::cout << progname << " --crack <ciphertext> [--lang <lang>]\n";
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

    std::string operation = argv[1];
    if (operation == "--encipher" || operation == "--chiffrer")
    {
        std::string plaintext = argv[2];
        std::string key = argv[3];
        std::cout << encipher(plaintext, key) << std::endl;
    }
    else if (operation == "--decipher" || operation == "--dechiffrer")
    {
        std::string ciphertext = argv[2];
        std::string key = argv[3];
        ciphertext = fmt_ciphertext(ciphertext);
        if (!is_valid_ciphertext(ciphertext))
        {
            std::cout << "Invalid ciphertext\n";
        }
        std::cout << decipher(ciphertext, key) << std::endl;
    }
    else if (operation == "--crack" || operation == "--craquer")
    {
        std::string ciphertext = argv[2];
        std::string lang = "en";
        if (argc >= 5)
        {
            std::string larg = std::string(argv[3]);
            if (larg == "--lang" || larg == "--langue")
            {
                lang = argv[4];
            }
        }
        ciphertext = fmt_ciphertext(ciphertext);
        if (!is_valid_ciphertext(ciphertext))
        {
            std::cout << "Invalid ciphertext\n";
        }
        crack(ciphertext, lang);
    }
    else
    {
        show_usage(progname);
    }

    return 0;
}
