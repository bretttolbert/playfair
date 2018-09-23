#ifndef PLAYFAIRCRACK_H
#define PLAYFAIRCRACK_H
#include "playfair.h"
#include <string>

namespace playfair
{
    const float START_TEMPERATURE = 20;
    const float TEMPERATURE_STEP = 0.2;
    const size_t ITERATIONS = 10000;
    double crack(const std::string& ciphertext, std::string lang="en");
}

#endif