cd avr/
sh compile.sh
sh compile.sh 
cd ..
cd cpucycles/
sh compile.sh
sh compile.sh 
cd ..
cd print/
sh compile.sh
sh compile.sh 
cd ..
cd arduino_core/
sh build_core.sh
cd ..
cd Crypto/
sh compile.sh
cd ..
#Compiling: CRYSTALS-Kyber Source-Files
cd Crystals-Kyber/
sh compile.sh $1 $2 1
sh compile.sh $1 $2 0
cd ..
#Compiling: Keygen_Test
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./Crystals-Kyber -I./arduino_core -I./Crypto -DKYBER_K=$1 -DSECRET_KEY_VARIANT=$2 -DUse_Deterministic_RNG=1 ./Crystals-Kyber/test_kyber_keygen.cpp -o ./Crystals-Kyber/test_kyber_keygen_K$1_V$2.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./Crystals-Kyber/test_kyber_keygen_K$1_V$2.elf ./Crystals-Kyber/test_kyber_keygen_K$1_V$2.o ./Crypto/BigNumberUtil.o  ./Crypto/Crypto.o  ./Crypto/Hash.o ./Crypto/KeccakCore.o  ./Crypto/SHA3.o ./Crypto/SHAKE.o ./Crypto/XOF.o  ./arduino_core/core.a ./Crystals-Kyber/kyber_K$1_V$2_R1.a
avr-objcopy -O ihex -R .eeprom ./Crystals-Kyber/test_kyber_keygen_K$1_V$2.elf test_kyber_keygen_K$1_V$2.hex
#Compiling: Enc_Test
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./Crystals-Kyber -I./arduino_core -I./Crypto -DKYBER_K=$1 -DSECRET_KEY_VARIANT=$2 -DUse_Deterministic_RNG=1 ./Crystals-Kyber/test_kyber_dec.cpp -o ./Crystals-Kyber/test_kyber_enc_K$1_V$2.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./Crystals-Kyber/test_kyber_enc_K$1_V$2.elf ./Crystals-Kyber/test_kyber_enc_K$1_V$2.o ./Crypto/BigNumberUtil.o  ./Crypto/Crypto.o  ./Crypto/Hash.o ./Crypto/KeccakCore.o  ./Crypto/SHA3.o ./Crypto/SHAKE.o ./Crypto/XOF.o  ./arduino_core/core.a ./Crystals-Kyber/kyber_K$1_V$2_R1.a
avr-objcopy -O ihex -R .eeprom ./Crystals-Kyber/test_kyber_enc_K$1_V$2.elf test_kyber_enc_K$1_V$2.hex
#Compiling: Dec_Test
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./Crystals-Kyber -I./arduino_core -I./Crypto -DKYBER_K=$1 -DSECRET_KEY_VARIANT=$2 -DUse_Deterministic_RNG=1 ./Crystals-Kyber/test_kyber_dec.cpp -o ./Crystals-Kyber/test_kyber_dec_K$1_V$2.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./Crystals-Kyber/test_kyber_dec_K$1_V$2.elf ./Crystals-Kyber/test_kyber_dec_K$1_V$2.o ./Crypto/BigNumberUtil.o  ./Crypto/Crypto.o  ./Crypto/Hash.o ./Crypto/KeccakCore.o  ./Crypto/SHA3.o ./Crypto/SHAKE.o ./Crypto/XOF.o  ./arduino_core/core.a ./Crystals-Kyber/kyber_K$1_V$2_R1.a
avr-objcopy -O ihex -R .eeprom ./Crystals-Kyber/test_kyber_dec_K$1_V$2.elf test_kyber_dec_K$1_V$2.hex
#Compiling: Cycle-Count-Measurement (KeyGen)
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./Crystals-Kyber -I./arduino_core -I./Crypto -I./avr -I./print -I./cpucycles -DKYBER_K=$1 -DSECRET_KEY_VARIANT=$2 -DUse_Deterministic_RNG=0 ./Crystals-Kyber/test_kyber_keygen_speed.cpp -o ./Crystals-Kyber/test_kyber_keygen_speed_K$1_V$2.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./Crystals-Kyber/test_kyber_keygen_speed_K$1_V$2.elf ./Crystals-Kyber/test_kyber_keygen_speed_K$1_V$2.o  ./Crypto/BigNumberUtil.o  ./Crypto/Crypto.o  ./Crypto/Hash.o ./Crypto/KeccakCore.o  ./Crypto/SHA3.o ./Crypto/SHAKE.o ./Crypto/XOF.o ./print/print.o ./avr/avr.o ./cpucycles/cpucycles.o ./Crypto/RNG.o ./Crypto/ChaCha.o  ./arduino_core/core.a ./Crystals-Kyber/kyber_K$1_V$2_R0.a
avr-objcopy -O ihex -R .eeprom ./Crystals-Kyber/test_kyber_keygen_speed_K$1_V$2.elf test_kyber_keygen_speed_K$1_V$2.hex
#Compiling: Cycle-Count-Measurement (Enc)
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./Crystals-Kyber -I./arduino_core -I./Crypto -I./avr -I./print -I./cpucycles -DKYBER_K=$1 -DSECRET_KEY_VARIANT=$2 -DUse_Deterministic_RNG=0 ./Crystals-Kyber/test_kyber_enc_speed.cpp -o ./Crystals-Kyber/test_kyber_enc_speed_K$1_V$2.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./Crystals-Kyber/test_kyber_enc_speed_K$1_V$2.elf ./Crystals-Kyber/test_kyber_enc_speed_K$1_V$2.o  ./Crypto/BigNumberUtil.o  ./Crypto/Crypto.o  ./Crypto/Hash.o ./Crypto/KeccakCore.o  ./Crypto/SHA3.o ./Crypto/SHAKE.o ./Crypto/XOF.o ./print/print.o ./avr/avr.o ./cpucycles/cpucycles.o ./Crypto/RNG.o ./Crypto/ChaCha.o  ./arduino_core/core.a ./Crystals-Kyber/kyber_K$1_V$2_R0.a
avr-objcopy -O ihex -R .eeprom ./Crystals-Kyber/test_kyber_enc_speed_K$1_V$2.elf test_kyber_enc_speed_K$1_V$2.hex
#Compiling: Cycle-Count-Measurement (Dec)
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./Crystals-Kyber -I./arduino_core -I./Crypto -I./avr -I./print -I./cpucycles -DKYBER_K=$1 -DSECRET_KEY_VARIANT=$2 -DUse_Deterministic_RNG=0 ./Crystals-Kyber/test_kyber_dec_speed.cpp -o ./Crystals-Kyber/test_kyber_dec_speed_K$1_V$2.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./Crystals-Kyber/test_kyber_dec_speed_K$1_V$2.elf ./Crystals-Kyber/test_kyber_dec_speed_K$1_V$2.o  ./Crypto/BigNumberUtil.o  ./Crypto/Crypto.o  ./Crypto/Hash.o ./Crypto/KeccakCore.o  ./Crypto/SHA3.o ./Crypto/SHAKE.o ./Crypto/XOF.o ./print/print.o ./avr/avr.o ./cpucycles/cpucycles.o ./Crypto/RNG.o ./Crypto/ChaCha.o  ./arduino_core/core.a ./Crystals-Kyber/kyber_K$1_V$2_R0.a
avr-objcopy -O ihex -R .eeprom ./Crystals-Kyber/test_kyber_dec_speed_K$1_V$2.elf test_kyber_dec_speed_K$1_V$2.hex
#Compiling: Memory-Measurement (KeyGen with keys)
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./Crystals-Kyber -I./arduino_core -I./Crypto -I./print -DKYBER_K=$1 -DSECRET_KEY_VARIANT=$2 -DUse_Deterministic_RNG=0 ./Crystals-Kyber/test_kyber_keygen_stack.cpp -o ./Crystals-Kyber/test_kyber_keygen_stack_K$1_V$2.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./Crystals-Kyber/test_kyber_keygen_stack_K$1_V$2.elf ./Crystals-Kyber/test_kyber_keygen_stack_K$1_V$2.o ./Crypto/BigNumberUtil.o  ./Crypto/Crypto.o  ./Crypto/Hash.o ./Crypto/KeccakCore.o  ./Crypto/SHA3.o ./Crypto/SHAKE.o ./Crypto/XOF.o ./print/print.o ./Crypto/RNG.o ./Crypto/ChaCha.o  ./arduino_core/core.a ./Crystals-Kyber/kyber_K$1_V$2_R0.a
avr-objcopy -O ihex -R .eeprom ./Crystals-Kyber/test_kyber_keygen_stack_K$1_V$2.elf test_kyber_keygen_stack_K$1_V$2.hex
#Compiling: Memory-Measurement (Enc with keys)
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./Crystals-Kyber -I./arduino_core -I./Crypto -I./print -DKYBER_K=$1 -DSECRET_KEY_VARIANT=$2 -DUse_Deterministic_RNG=0 ./Crystals-Kyber/test_kyber_enc_stack.cpp -o ./Crystals-Kyber/test_kyber_enc_stack_K$1_V$2.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./Crystals-Kyber/test_kyber_enc_stack_K$1_V$2.elf ./Crystals-Kyber/test_kyber_enc_stack_K$1_V$2.o ./Crypto/BigNumberUtil.o  ./Crypto/Crypto.o  ./Crypto/Hash.o ./Crypto/KeccakCore.o  ./Crypto/SHA3.o ./Crypto/SHAKE.o ./Crypto/XOF.o ./print/print.o ./Crypto/RNG.o ./Crypto/ChaCha.o  ./arduino_core/core.a ./Crystals-Kyber/kyber_K$1_V$2_R0.a
avr-objcopy -O ihex -R .eeprom ./Crystals-Kyber/test_kyber_enc_stack_K$1_V$2.elf test_kyber_enc_stack_K$1_V$2.hex
#Compiling: Memory-Measurement (Dec with keys)
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./Crystals-Kyber -I./arduino_core -I./Crypto -I./print -DKYBER_K=$1 -DSECRET_KEY_VARIANT=$2 -DUse_Deterministic_RNG=0 ./Crystals-Kyber/test_kyber_dec_stack.cpp -o ./Crystals-Kyber/test_kyber_dec_stack_K$1_V$2.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./Crystals-Kyber/test_kyber_dec_stack_K$1_V$2.elf ./Crystals-Kyber/test_kyber_dec_stack_K$1_V$2.o ./Crypto/BigNumberUtil.o  ./Crypto/Crypto.o  ./Crypto/Hash.o ./Crypto/KeccakCore.o  ./Crypto/SHA3.o ./Crypto/SHAKE.o ./Crypto/XOF.o ./print/print.o ./Crypto/RNG.o ./Crypto/ChaCha.o  ./arduino_core/core.a ./Crystals-Kyber/kyber_K$1_V$2_R0.a
avr-objcopy -O ihex -R .eeprom ./Crystals-Kyber/test_kyber_dec_stack_K$1_V$2.elf test_kyber_dec_stack_K$1_V$2.hex
#Compiling: Memory-Measurement (KeyGen without keys)
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./Crystals-Kyber -I./arduino_core -I./Crypto -I./print -DKYBER_K=$1 -DSECRET_KEY_VARIANT=$2 -DUse_Deterministic_RNG=0 ./Crystals-Kyber/test_kyber_keygen_stack_without_keys.cpp -o ./Crystals-Kyber/test_kyber_keygen_stack_K$1_V$2_without_keys.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./Crystals-Kyber/test_kyber_keygen_stack_K$1_V$2_without_keys.elf ./Crystals-Kyber/test_kyber_keygen_stack_K$1_V$2_without_keys.o ./Crypto/BigNumberUtil.o  ./Crypto/Crypto.o  ./Crypto/Hash.o ./Crypto/KeccakCore.o  ./Crypto/SHA3.o ./Crypto/SHAKE.o ./Crypto/XOF.o ./print/print.o ./Crypto/RNG.o ./Crypto/ChaCha.o  ./arduino_core/core.a ./Crystals-Kyber/kyber_K$1_V$2_R0.a
avr-objcopy -O ihex -R .eeprom ./Crystals-Kyber/test_kyber_keygen_stack_K$1_V$2_without_keys.elf test_kyber_keygen_stack_K$1_V$2_without_keys.hex
#Compiling: Memory-Measurement (Enc without keys)
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./Crystals-Kyber -I./arduino_core -I./Crypto -I./print -DKYBER_K=$1 -DSECRET_KEY_VARIANT=$2 -DUse_Deterministic_RNG=0 ./Crystals-Kyber/test_kyber_enc_stack_without_keys.cpp -o ./Crystals-Kyber/test_kyber_enc_stack_K$1_V$2_without_keys.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./Crystals-Kyber/test_kyber_enc_stack_K$1_V$2_without_keys.elf ./Crystals-Kyber/test_kyber_enc_stack_K$1_V$2_without_keys.o ./Crypto/BigNumberUtil.o  ./Crypto/Crypto.o  ./Crypto/Hash.o ./Crypto/KeccakCore.o  ./Crypto/SHA3.o ./Crypto/SHAKE.o ./Crypto/XOF.o ./print/print.o ./Crypto/RNG.o ./Crypto/ChaCha.o  ./arduino_core/core.a ./Crystals-Kyber/kyber_K$1_V$2_R0.a
avr-objcopy -O ihex -R .eeprom ./Crystals-Kyber/test_kyber_enc_stack_K$1_V$2_without_keys.elf test_kyber_enc_stack_K$1_V$2_without_keys.hex
#Compiling: Memory-Measurement (Dec without keys)
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./Crystals-Kyber -I./arduino_core -I./Crypto -I./print -DKYBER_K=$1 -DSECRET_KEY_VARIANT=$2 -DUse_Deterministic_RNG=0 ./Crystals-Kyber/test_kyber_dec_stack_without_keys.cpp -o ./Crystals-Kyber/test_kyber_dec_stack_K$1_V$2_without_keys.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./Crystals-Kyber/test_kyber_dec_stack_K$1_V$2_without_keys.elf ./Crystals-Kyber/test_kyber_dec_stack_K$1_V$2_without_keys.o ./Crypto/BigNumberUtil.o  ./Crypto/Crypto.o  ./Crypto/Hash.o ./Crypto/KeccakCore.o  ./Crypto/SHA3.o ./Crypto/SHAKE.o ./Crypto/XOF.o ./print/print.o ./Crypto/RNG.o ./Crypto/ChaCha.o  ./arduino_core/core.a ./Crystals-Kyber/kyber_K$1_V$2_R0.a
avr-objcopy -O ihex -R .eeprom ./Crystals-Kyber/test_kyber_dec_stack_K$1_V$2_without_keys.elf test_kyber_dec_stack_K$1_V$2_without_keys.hex
