[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](../master/LICENSE)

# playfair

Playfair cipher cracker

Adapted from http://practicalcryptography.com/cryptanalysis/stochastic-searching/cryptanalysis-playfair/

## How to build

```
cmake -H. -Bbuild -DCMAKE_INSTALL_PREFIX:PATH=/tmp/foo
cd build
cmake --build .
./bin/unit_tests
./bin/main.x
```

## Usage
```
./bin/main.x --encipher <plaintext> <key>
./bin/main.x --decipher <ciphertext> <key>
./bin/main.x --crack <ciphertext> [--lang en|fr]
```
Advanced options
```
--sub <omitted_letter><omitted_letter_substitute> (default is JI)
--sep <separator_letter> (default is X)
```

## Usage Examples

```
./bin/main.x --encipher "Hide the gold in the tree stump" "playfair example"
Omitted letter: 'J' Substitute: 'I'
Key Table:
P L A Y F 
I R E X M 
B C D G H 
K N O Q S 
T U V W Z 
Digraphs:
HI DE TH EG OL DI NT HE TR EX ES TU MP 
Ciphertext:
BM OD ZB XD NA BE KU DM UI XM MO UV IF 

./bin/main.x --decipher "BM OD ZB XD NA BE KU DM UI XM MO UV IF" "playfair example"
Omitted letter: 'J' Substitute: 'I'
Key Table:
P L A Y F 
I R E X M 
B C D G H 
K N O Q S 
T U V W Z 
Digraphs:
BM OD ZB XD NA BE KU DM UI XM MO UV IF 
Deciphered:
HIDETHEGOLDINTHETREXESTUMP

./bin/main.x --crack XZOGQRWVQWNROKCOAELBXZWGEQYLGDRZXYZRQAEKLRHDUMNUXYXSXYEMXEHDGNXZYNTZONYELBEUGYSCOREUSWTZRLRYBYCOLZYLEMWNSXFBUSDBORBZCYLQEDMHQRWVQWAEDPGDPOYHORXZINNYWPXZGROKCOLCCOCYTZUEUIICERLEVHMVQWLNWPRYXHGNMLEKLRHDUYSUCYRAWPUYECRYRYXHGNBLUYSCCOUYOHRYUMNUXYXSXYEMXEHDGN
```

In crack mode, program will run until it is stopped with Ctrl+C. Here is a particularly difficult one that takes a while to crack:
```
./bin/main.x --crack "ZK DW KC SE XM ZK DW VF RV LQ VF WN ED MZ LW QE GY VF KD XF MP WC GO BF MU GY QF UG ZK NZ IM GK FK GY ZS GQ LN DP AB BM CK OQ KL EZ KF DH YK ZN LK FK EU YK FK KZ RY YD FT PC HD GQ MZ CP YD KL KF EZ CI ON DP AC WK QS SY QL UN DU RU GY NS"
```

Français

```
$ ./bin/main.x --chiffrer "Cache l'or dans la souche de l'arbre" "exemple playfair" --sub WV
Omitted letter: 'W' Substitute: 'V'
Key Table:
E X M P L 
A Y F I R 
B C D G H 
J K N O Q 
S T U V Z 
Digraphs:
CA CH EL OR DA NS LA SO UC HE DE LA RB RE 
Ciphertext:
BY DB XE QI BF JU ER VJ TD BL BM ER AH AL

./bin/main.x --dechiffrer "BY DB XE QI BF JU ER VJ TD BL BM ER AH AL" "exemple playfair" --sub WV
Omitted letter: 'W' Substitute: 'V'
Key Table:
E X M P L 
A Y F I R 
B C D G H 
J K N O Q 
S T U V Z 
Digraphs:
BY DB XE QI BF JU ER VJ TD BL BM ER AH AL 
Deciphered:
CACHELORDANSLASOUCHEDELARBRE

./bin/main.x --craquer "EVZTOVBVRPXRQAQGVTRBKVPMEGBKVACLVMIPRWMCIXEOBCOCRWGIBQWMGIBORPOBRWKSDYMRIXPRKLEIRWTSVDTVSOVYEBVOVCVHRWPREGMNWIPACPXEZMFVZICPZCQGZQRVWRUDCLPWWRSRVCAQRWVSBRTVOVBVEGDVPGPIQOCZPCUKFRBVVOBIEGMQERATGXVXPZMCNQEGIBQARBIEVRMQVWEIBLLBCOCMRWMYRWEGWRPUSVBVKCMFDMGECPOSIRRPMKVNCPVBMYCPGITVSVCMRWNAXMRAPWSORBQOMYEVBLVCOHEVMKGECZWIGINVPMRBWIEGCTFRPRVHWMRTPRBQRWPRLBPGQOEVFTPUPIMVOLCMRMLKBPXIIBLYLBGINVCMGEEGPROLXVYVCVSVCMGXVWPCENOCIBMYEVSOOVBVMXNEVWLIBNXINAEUOSLBBLRWXCMYOCVBBEPCBYXIEGEBQAIGCPRWOCCEGPRPCLTLDEMYCLOVBVBLGXBEMVSLEORMDVEBMVLBNQPZ" --langue fr
```
