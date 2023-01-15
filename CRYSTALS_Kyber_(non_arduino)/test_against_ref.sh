#Test der Algorithmen (mit SK-Variante 0) gegen Test-Vectoren der Referenz-Implementierung
cc cbd.cpp fips202.cpp indcpa.cpp kem.cpp matacc.cpp ntt.cpp poly.cpp polyvec.cpp randombytes.cpp reduce.cpp symmetric-fips202.cpp verify.cpp -DKYBER_K=2 -DSECRET_KEY_VARIANT=0 -DUse_Deterministic_RNG=1 test_kyber_ref_testvectors.cpp -o test_Kyber512_against_ref
./test_Kyber512_against_ref
cc cbd.cpp fips202.cpp indcpa.cpp kem.cpp matacc.cpp ntt.cpp poly.cpp polyvec.cpp randombytes.cpp reduce.cpp symmetric-fips202.cpp verify.cpp -DKYBER_K=3 -DSECRET_KEY_VARIANT=0 -DUse_Deterministic_RNG=1 test_kyber_ref_testvectors.cpp -o test_Kyber768_against_ref
./test_Kyber768_against_ref
cc cbd.cpp fips202.cpp indcpa.cpp kem.cpp matacc.cpp ntt.cpp poly.cpp polyvec.cpp randombytes.cpp reduce.cpp symmetric-fips202.cpp verify.cpp -DKYBER_K=4 -DSECRET_KEY_VARIANT=0 -DUse_Deterministic_RNG=1 test_kyber_ref_testvectors.cpp -o test_Kyber1024_against_ref
./test_Kyber1024_against_ref