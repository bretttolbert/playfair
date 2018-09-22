#include "gtest/gtest.h"
#include "playfair.h"

using namespace playfair;

std::vector<std::string> TEST_CIPHER_TABLE{
    "PALME",
    "RSTON",
    "BCDFG",
    "HIKQU",
    "VWXYZ"};

std::vector<std::string> TEST_CIPHER_TABLE2{
    "PASWD",
    "BCEFG",
    "HIKLM",
    "NOQRT",
    "UVXYZ"};

TEST(playfair, is_valid_key)
{
    ASSERT_TRUE(is_valid_key("BRET"));
    ASSERT_FALSE(is_valid_key("bret"));
    ASSERT_FALSE(is_valid_key("BRETT"));
    ASSERT_FALSE(is_valid_key("JOE"));
}

TEST(playfair, plaintext_to_digrams)
{
    std::string input;
    std::vector<std::string> expected;
    expected = {"BA", "TX", "TA", "LI", "ON"};
    ASSERT_EQ(plaintext_to_digraphs("battalion"), expected);
    expected = {"IU", "ST", "IN"};
    ASSERT_EQ(plaintext_to_digraphs("justin"), expected);
    expected = {"BR", "ET", "TX"};
    ASSERT_EQ(plaintext_to_digraphs("brett"), expected);
    expected = {"IU", "ST", "IN", "BR", "ET", "TX", "TO", "LB", "ER", "TX"};
    ASSERT_EQ(plaintext_to_digraphs("Justin Brett Tolbert"), expected);
    expected = {"PL", "MO", "TH"};
    ASSERT_EQ(plaintext_to_digraphs("plmoth"), expected);
    expected = {"HI", "DE", "TH", "EG", "OL", "DI", "NT", "HE", "TR", "EX", "ES", "TU", "MP"};
    ASSERT_EQ(plaintext_to_digraphs("Hide the gold in the tree stump"), expected);
}

TEST(playfair, gen_cipher_table_string)
{
    ASSERT_EQ(gen_cipher_table_string(""), CIPHER_ALPHABET);
    ASSERT_EQ(gen_cipher_table_string("PALMERSTON"), "PALMERSTONBCDFGHIKQUVWXYZ");
    ASSERT_EQ(gen_cipher_table_string("PASWD"), "PASWDBCEFGHIKLMNOQRTUVXYZ");
}

TEST(playfair, gen_cipher_table)
{
    ASSERT_EQ(gen_cipher_table("PALMERSTON"), TEST_CIPHER_TABLE);
    ASSERT_EQ(gen_cipher_table("PASWD"), TEST_CIPHER_TABLE2);
}

TEST(playfair, get_letter_row_col)
{
    RowCol expected{0,0};
    ASSERT_EQ(get_letter_row_col('P', TEST_CIPHER_TABLE), expected);
    expected = {1,2};
    ASSERT_EQ(get_letter_row_col('T', TEST_CIPHER_TABLE), expected);
}

TEST(playfair, encipher_digraph)
{
    ASSERT_EQ(encipher_digraph("PL", TEST_CIPHER_TABLE), "AM");
    ASSERT_EQ(encipher_digraph("MO", TEST_CIPHER_TABLE), "OF");
    ASSERT_EQ(encipher_digraph("TH", TEST_CIPHER_TABLE), "RK");
    ASSERT_EQ(encipher_digraph("TO", TEST_CIPHER_TABLE2), "NQ");
}

TEST(playfair, decipher_digraph)
{
    ASSERT_EQ(encipher_digraph("AM", TEST_CIPHER_TABLE, true), "PL");
    ASSERT_EQ(encipher_digraph("OF", TEST_CIPHER_TABLE, true), "MO");
    ASSERT_EQ(encipher_digraph("RK", TEST_CIPHER_TABLE, true), "TH");
    ASSERT_EQ(encipher_digraph("NQ", TEST_CIPHER_TABLE2, true), "TO");
}

TEST(playfair, encipher)
{
    ASSERT_EQ(encipher("plmoth", "PALMERSTON"), "AMOFRK");
    ASSERT_EQ(encipher("pl mo th", "PALMERSTON"), "AMOFRK");
    ASSERT_EQ(encipher("Justin Brett Tolbert", "PASWD"), "HVDQHOFNGQQZNQHFFQQZ");
    ASSERT_EQ(encipher("Hide the gold in the tree stump", "PLAYFIREXM"), "BMODZBXDNABEKUDMUIXMMOUVIF");
}

TEST(playfair, decipher)
{
    ASSERT_EQ(decipher("AMOFRK", "PALMERSTON"), "PLMOTH");
    ASSERT_EQ(decipher("HVDQHOFNGQQZNQHFFQQZ", "PASWD"), "IUSTINBRETTXTOLBERTX");
    ASSERT_EQ(decipher("BMODZBXDNABEKUDMUIXMMOUVIF", "PLAYFIREXM"), "HIDETHEGOLDINTHETREXESTUMP");
}
