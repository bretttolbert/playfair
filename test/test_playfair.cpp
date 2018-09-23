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

TEST(playfair, strip_accents)
{
    ASSERT_EQ(strip_accents("CHÂTEAUchâteau"), "CHATEAUchateau");
    ASSERT_EQ(strip_accents("àáâäÀÁÂÄèéêëÈÉÊËìíîïÌÍÎÏùúûüÙÚÜÛ"), "aaaaAAAAeeeeEEEEiiiiIIIIuuuuUUUU");
    ASSERT_EQ(strip_accents("çÇæÆñÑ"), "cCaeAEnN");
}

TEST(playfair, plaintext_to_digrams)
{
    std::string input;
    std::vector<std::string> expected;
    expected = {"BA", "TX", "TA", "LI", "ON"};
    ASSERT_EQ(to_digraphs("battalion"), expected);
    expected = {"IU", "ST", "IN"};
    ASSERT_EQ(to_digraphs("justin"), expected);
    expected = {"BR", "ET", "TX"};
    ASSERT_EQ(to_digraphs("brett"), expected);
    expected = {"IU", "ST", "IN", "BR", "ET", "TX", "TO", "LB", "ER", "TX"};
    ASSERT_EQ(to_digraphs("Justin Brett Tolbert"), expected);
    expected = {"PL", "MO", "TH"};
    ASSERT_EQ(to_digraphs("plmoth"), expected);
    expected = {"HI", "DE", "TH", "EG", "OL", "DI", "NT", "HE", "TR", "EX", "ES", "TU", "MP"};
    ASSERT_EQ(to_digraphs("Hide the gold in the tree stump"), expected);
    expected = {"CH", "AT", "EA", "UX"};
    ASSERT_EQ(to_digraphs("château"), expected);
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
    ASSERT_EQ(encipher("plmoth", "PALMERSTON"), "AM OF RK ");
    ASSERT_EQ(encipher("pl mo th", "PALMERSTON"), "AM OF RK ");
    ASSERT_EQ(encipher("Justin Brett Tolbert", "PASWD"), "HV DQ HO FN GQ QZ NQ HF FQ QZ ");
    ASSERT_EQ(encipher("Hide the gold in the tree stump", "PLAYFIREXM"), "BM OD ZB XD NA BE KU DM UI XM MO UV IF ");
}

TEST(playfair, decipher)
{
    ASSERT_EQ(decipher("AMOFRK", "PALMERSTON"), "PLMOTH");
    ASSERT_EQ(decipher("HVDQHOFNGQQZNQHFFQQZ", "PASWD"), "IUSTINBRETTXTOLBERTX");
    ASSERT_EQ(decipher("BMODZBXDNABEKUDMUIXMMOUVIF", "PLAYFIREXM"), "HIDETHEGOLDINTHETREXESTUMP");
}

const std::string TEST_TEXT_FR = "Il y avait en Westphalie, dans le château de monsieur le baron de Thunder-ten-tronckh, un jeune garçon à qui la nature avait donné les mœurs les plus douces. Sa physionomie annonçait son âme. Il avait le jugement assez droit, avec l’esprit le plus simple ; c’est, je crois, pour cette raison qu’on le nommait Candide. Les anciens domestiques de la maison soupçonnaient qu’il était fils de la sœur de monsieur le baron et d’un bon et honnête gentilhomme du voisinage, que cette demoiselle ne voulut jamais épouser parce qu’il n’avait pu prouver que soixante et onze quartiers, et que le reste de son arbre généalogique avait été perdu par l’injure du temps.";
 
TEST(playfair, to_digraphs_fr)
{
    std::vector<std::string> expected {"IL", "YA", "VA", "IT", "EN", "WE", "ST", "PH", "AL", "IE", "DA", "NS", "LE", "CH", "AT", "EA", "UD", "EM", "ON", "SI", "EU", "RL", "EB", "AR", "ON", "DE", "TH", "UN", "DE", "RT", "EN", "TR", "ON", "CK", "HU", "NI", "EU", "NE", "GA", "RC", "ON", "AQ", "UI", "LA", "NA", "TU", "RE", "AV", "AI", "TD", "ON", "NE", "LE", "SM", "UR", "SL", "ES", "PL", "US", "DO", "UC", "ES", "SA", "PH", "YS", "IO", "NO", "MI", "EA", "NX", "NO", "NC", "AI", "TS", "ON", "AM", "EI", "LA", "VA", "IT", "LE", "IU", "GE", "ME", "NT", "AS", "SE", "ZD", "RO", "IT", "AV", "EC", "LE", "SP", "RI", "TL", "EP", "LU", "SX", "SI", "MP", "LE", "CE", "ST", "IE", "CR", "OI", "SP", "OU", "RC", "ET", "TE", "RA", "IS", "ON", "QU", "ON", "LE", "NO", "MX", "MA", "IT", "CA", "ND", "ID", "EL", "ES", "AN", "CI", "EN", "SD", "OM", "ES", "TI", "QU", "ES", "DE", "LA", "MA", "IS", "ON", "SO", "UP", "CO", "NX", "NA", "IE", "NT", "QU", "IL", "ET", "AI", "TF", "IL", "SD", "EL", "AS", "UR", "DE", "MO", "NS", "IE", "UR", "LE", "BA", "RO", "NE", "TD", "UN", "BO", "NE", "TH", "ON", "NE", "TE", "GE", "NT", "IL", "HO", "MX", "ME", "DU", "VO", "IS", "IN", "AG", "EQ", "UE", "CE", "TX", "TE", "DE", "MO", "IS", "EL", "LE", "NE", "VO", "UL", "UT", "IA", "MA", "IS", "EP", "OU", "SE", "RP", "AR", "CE", "QU", "IL", "NA", "VA", "IT", "PU", "PR", "OU", "VE", "RQ", "UE", "SO", "IX", "AN", "TE", "ET", "ON", "ZE", "QU", "AR", "TI", "ER", "SE", "TQ", "UE", "LE", "RE", "ST", "ED", "ES", "ON", "AR", "BR", "EG", "EN", "EA", "LO", "GI", "QU", "EA", "VA", "IT", "ET", "EP", "ER", "DU", "PA", "RL", "IN", "IU", "RE", "DU", "TE", "MP", "SX"};
    ASSERT_EQ(to_digraphs(TEST_TEXT_FR), expected);
}

TEST(playfair, encipher_decipher_fr)
{
    std::string expected_ciphertext = "EV ZT OV BV RP XR QA QG VT RB KV PM EG BK VA CL VM IP RW MC IX EO BC OC RW GI BQ WM GI BO RP OB RW KS DY MR IX PR KL EI RW TS VD TV SO VY EB VO VC VH RW PR EG MN WI PA CP XE ZM FV ZI CP ZC QG ZQ RV WR UD CL PW WR SR VC AQ RW VS BR TV OV BV EG DV PG PI QO CZ PC UK FR BV VO BI EG MQ ER AT GX VX PZ MC NQ EG IB QA RB IE VR MQ VW EI BL LB CO CM RW MY RW EG WR PU SV BV KC MF DM GE CP OS IR RP MK VN CP VB MY CP GI TV SV CM RW NA XM RA PW SO RB QO MY EV BL VC OH EV MK GE CZ WI GI NV PM RB WI EG CT FR PR VH WM RT PR BQ RW PR LB PG QO EV FT PU PI MV OL CM RM LK BP XI IB LY LB GI NV CM GE EG PR OL XV YV CV SV CM GX VW PC EN OC IB MY EV SO OV BV MX NE VW LI BN XI NA EU OS LB BL RW XC MY OC VB BE PC BY XI EG EB QA IG CP RW OC CE GP RP CL TL DE MY CL OV BV BL GX BE MV SL EO RM DV EB MV LB NQ PZ ";
    ASSERT_EQ(encipher(TEST_TEXT_FR, "VOLTAIRE"), expected_ciphertext);
    std::string expected_plaintext = "ILYAVAITENWESTPHALIEDANSLECHATEAUDEMONSIEURLEBARONDETHUNDERTENTRONCKHUNIEUNEGARCONAQUILANATUREAVAITDONNELESMURSLESPLUSDOUCESSAPHYSIONOMIEANXNONCAITSONAMEILAVAITLEIUGEMENTASSEZDROITAVECLESPRITLEPLUSXSIMPLECESTIECROISPOURCETTERAISONQUONLENOMXMAITCANDIDELESANCIENSDOMESTIQUESDELAMAISONSOUPCONXNAIENTQUILETAITFILSDELASURDEMONSIEURLEBARONETDUNBONETHONNETEGENTILHOMXMEDUVOISINAGEQUECETXTEDEMOISELLENEVOULUTIAMAISEPOUSERPARCEQUILNAVAITPUPROUVERQUESOIXANTEETONZEQUARTIERSETQUELERESTEDESONARBREGENEALOGIQUEAVAITETEPERDUPARLINIUREDUTEMPSX";
    ASSERT_EQ(decipher(expected_ciphertext, "VOLTAIRE"), expected_plaintext);
}
