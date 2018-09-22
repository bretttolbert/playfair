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

## Usage Examples

```
./bin/main.x --encipher "Hide the gold in the tree stump" "PLAYFIREXM"
BMODZBXDNABEKUDMUIXMMOUVIF

./bin/main.x --decipher "BMODZBXDNABEKUDMUIXMMOUVIF" "PLAYFIREXM"
HIDETHEGOLDINTHETREXESTUMP

./bin/main.x --crack XZOGQRWVQWNROKCOAELBXZWGEQYLGDRZXYZRQAEKLRHDUMNUXYXSXYEMXEHDGNXZYNTZONYELBEUGYSCOREUSWTZRLRYBYCOLZYLEMWNSXFBUSDBORBZCYLQEDMHQRWVQWAEDPGDPOYHORXZINNYWPXZGROKCOLCCOCYTZUEUIICERLEVHMVQWLNWPRYXHGNMLEKLRHDUYSUCYRAWPUYECRYRYXHGNBLUYSCCOUYOHRYUMNUXYXSXYEMXEHDGN

./bin/main.x --crack "ZK DW KC SE XM ZK DW VF RV LQ VF WN ED MZ LW QE GY VF KD XF MP WC GO BF MU GY QF UG ZK NZ IM GK FK GY ZS GQ LN DP AB BM CK OQ KL EZ KF DH YK ZN LK FK EU YK FK KZ RY YD FT PC HD GQ MZ CP YD KL KF EZ CI ON DP AC WK QS SY QL UN DU RU GY NS"

./bin/main.x --crack "IBWYMYEMIXLIQOOLZMKOREBIFRRECLGIZCCQWIMDLBMQIXLIQOOLZMKOREBIVNPILYOBIGCPVAFRIRLBPZ" --lang fr
```

In crack mode, program will run until it is stopped with Ctrl+C
