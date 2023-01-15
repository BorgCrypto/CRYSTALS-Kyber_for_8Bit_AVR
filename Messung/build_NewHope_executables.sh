#Build needed Object-Files
cd arduino_core/
sh build_core.sh
cd ..
cd Crypto/
sh compile.sh
cd ..
cd avr/
sh compile.sh
cd ..
cd cpucycles/
sh compile.sh
cd ..
cd print/
sh compile.sh
cd ..
cd NewHope/
sh compile.sh
cd ..
#Build NewHope-Test
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./Crystals-Kyber -I./print -I./NewHope -I./Crypto -I./avr ./NewHope/test_NewHope.cpp -o ./NewHope/test_NewHope.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./NewHope/test_NewHope.elf ./NewHope/test_NewHope.o ./Crypto/BigNumberUtil.o ./Crypto/Crypto.o ./NewHope/NewHope.o ./Crypto/KeccakCore.o  ./Crypto/ChaCha.o  ./print/print.o   ./Crypto/SHA3.o  ./avr/avr.o   ./Crypto/Hash.o ./Crypto/SHAKE.o ./Crypto/XOF.o ./Crystals-Kyber/randombytes.o  ./arduino_core/core.a
avr-objcopy -O ihex -R .eeprom ./NewHope/test_NewHope.elf test_NewHope.hex
#Build Keygen-Speed-Test
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./print -I./NewHope -I./Crypto -I./avr -I./cpucycles  ./NewHope/test_NewHope_keygen_speed.cpp -o ./NewHope/test_NewHope_keygen_speed.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./NewHope/test_NewHope_keygen_speed.elf ./NewHope/test_NewHope_keygen_speed.o  ./NewHope/NewHope.o  ./Crypto/BigNumberUtil.o  ./Crypto/ChaCha.o ./Crypto/Crypto.o ./print/print.o  ./Crypto/SHA3.o ./Crypto/Hash.o ./avr/avr.o ./Crypto/KeccakCore.o  ./Crypto/SHAKE.o ./Crypto/XOF.o ./Crypto/RNG.o ./cpucycles/cpucycles.o  ./arduino_core/core.a
avr-objcopy -O ihex -R .eeprom ./NewHope/test_NewHope_keygen_speed.elf test_NewHope_keygen_speed.hex
#Build Enc-Speed-Test
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./print -I./NewHope -I./Crypto -I./avr -I./cpucycles  ./NewHope/test_NewHope_enc_speed.cpp -o ./NewHope/test_NewHope_enc_speed.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./NewHope/test_NewHope_enc_speed.elf ./NewHope/test_NewHope_enc_speed.o  ./NewHope/NewHope.o  ./Crypto/BigNumberUtil.o  ./Crypto/ChaCha.o ./Crypto/Crypto.o ./print/print.o  ./Crypto/SHA3.o ./Crypto/Hash.o ./avr/avr.o ./Crypto/KeccakCore.o  ./Crypto/SHAKE.o ./Crypto/XOF.o ./Crypto/RNG.o ./cpucycles/cpucycles.o  ./arduino_core/core.a
avr-objcopy -O ihex -R .eeprom ./NewHope/test_NewHope_enc_speed.elf test_NewHope_enc_speed.hex
#Build Dec-Speed-Test
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./print -I./NewHope -I./Crypto -I./avr -I./cpucycles  ./NewHope/test_NewHope_dec_speed.cpp -o ./NewHope/test_NewHope_dec_speed.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./NewHope/test_NewHope_dec_speed.elf ./NewHope/test_NewHope_dec_speed.o  ./NewHope/NewHope.o  ./Crypto/BigNumberUtil.o  ./Crypto/ChaCha.o ./Crypto/Crypto.o ./print/print.o  ./Crypto/SHA3.o ./Crypto/Hash.o ./avr/avr.o ./Crypto/KeccakCore.o  ./Crypto/SHAKE.o ./Crypto/XOF.o ./Crypto/RNG.o ./cpucycles/cpucycles.o  ./arduino_core/core.a
avr-objcopy -O ihex -R .eeprom ./NewHope/test_NewHope_dec_speed.elf test_NewHope_dec_speed.hex
#Build Keygen-Stack-Test (with keys)
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./print -I./NewHope -I./Crypto -I./avr ./NewHope/test_NewHope_keygen_stack.cpp -o ./NewHope/test_NewHope_keygen_stack.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./NewHope/test_NewHope_keygen_stack.elf ./NewHope/test_NewHope_keygen_stack.o  ./NewHope/NewHope.o  ./Crypto/BigNumberUtil.o  ./Crypto/ChaCha.o ./Crypto/Crypto.o ./print/print.o  ./Crypto/SHA3.o ./Crypto/Hash.o ./avr/avr.o ./Crypto/KeccakCore.o  ./Crypto/SHAKE.o ./Crypto/XOF.o ./Crypto/RNG.o ./arduino_core/core.a
avr-objcopy -O ihex -R .eeprom ./NewHope/test_NewHope_keygen_stack.elf test_NewHope_keygen_stack.hex
#Build Enc-Stack-Test (with keys)
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./print -I./NewHope -I./Crypto -I./avr ./NewHope/test_NewHope_enc_stack.cpp -o ./NewHope/test_NewHope_enc_stack.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./NewHope/test_NewHope_enc_stack.elf ./NewHope/test_NewHope_enc_stack.o  ./NewHope/NewHope.o  ./Crypto/BigNumberUtil.o  ./Crypto/ChaCha.o ./Crypto/Crypto.o ./print/print.o  ./Crypto/SHA3.o ./Crypto/Hash.o ./avr/avr.o ./Crypto/KeccakCore.o  ./Crypto/SHAKE.o ./Crypto/XOF.o ./Crypto/RNG.o ./arduino_core/core.a
avr-objcopy -O ihex -R .eeprom ./NewHope/test_NewHope_enc_stack.elf test_NewHope_enc_stack.hex
#Build Dec-Stack-Test (with keys)
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./print -I./NewHope -I./Crypto -I./avr ./NewHope/test_NewHope_dec_stack.cpp -o ./NewHope/test_NewHope_dec_stack.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./NewHope/test_NewHope_dec_stack.elf ./NewHope/test_NewHope_dec_stack.o  ./NewHope/NewHope.o  ./Crypto/BigNumberUtil.o  ./Crypto/ChaCha.o ./Crypto/Crypto.o ./print/print.o  ./Crypto/SHA3.o ./Crypto/Hash.o ./avr/avr.o ./Crypto/KeccakCore.o  ./Crypto/SHAKE.o ./Crypto/XOF.o ./Crypto/RNG.o ./arduino_core/core.a
avr-objcopy -O ihex -R .eeprom ./NewHope/test_NewHope_dec_stack.elf test_NewHope_dec_stack.hex
#Build Keygen-Stack-Test (without keys)
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./print -I./NewHope -I./Crypto -I./avr ./NewHope/test_NewHope_keygen_stack_without_keys.cpp -o ./NewHope/test_NewHope_keygen_stack_without_keys.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./NewHope/test_NewHope_keygen_stack_without_keys.elf ./NewHope/test_NewHope_keygen_stack_without_keys.o  ./NewHope/NewHope.o  ./Crypto/BigNumberUtil.o  ./Crypto/ChaCha.o ./Crypto/Crypto.o ./print/print.o  ./Crypto/SHA3.o ./Crypto/Hash.o ./avr/avr.o ./Crypto/KeccakCore.o  ./Crypto/SHAKE.o ./Crypto/XOF.o ./Crypto/RNG.o ./arduino_core/core.a
avr-objcopy -O ihex -R .eeprom ./NewHope/test_NewHope_keygen_stack_without_keys.elf test_NewHope_keygen_stack_without_keys.hex
#Build Enc-Stack-Test (without keys)
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./print -I./NewHope -I./Crypto -I./avr ./NewHope/test_NewHope_enc_stack_without_keys.cpp -o ./NewHope/test_NewHope_enc_stack_without_keys.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./NewHope/test_NewHope_enc_stack_without_keys.elf ./NewHope/test_NewHope_enc_stack_without_keys.o  ./NewHope/NewHope.o  ./Crypto/BigNumberUtil.o  ./Crypto/ChaCha.o ./Crypto/Crypto.o ./print/print.o  ./Crypto/SHA3.o ./Crypto/Hash.o ./avr/avr.o ./Crypto/KeccakCore.o  ./Crypto/SHAKE.o ./Crypto/XOF.o ./Crypto/RNG.o ./arduino_core/core.a
avr-objcopy -O ihex -R .eeprom ./NewHope/test_NewHope_enc_stack_without_keys.elf test_NewHope_enc_stack_without_keys.hex
#Build Dec-Stack-Test (without keys)
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./print -I./NewHope -I./Crypto -I./avr ./NewHope/test_NewHope_dec_stack_without_keys.cpp -o ./NewHope/test_NewHope_dec_stack_without_keys.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./NewHope/test_NewHope_dec_stack_without_keys.elf ./NewHope/test_NewHope_dec_stack_without_keys.o  ./NewHope/NewHope.o  ./Crypto/BigNumberUtil.o  ./Crypto/ChaCha.o ./Crypto/Crypto.o ./print/print.o  ./Crypto/SHA3.o ./Crypto/Hash.o ./avr/avr.o ./Crypto/KeccakCore.o  ./Crypto/SHAKE.o ./Crypto/XOF.o ./Crypto/RNG.o ./arduino_core/core.a
avr-objcopy -O ihex -R .eeprom ./NewHope/test_NewHope_dec_stack_without_keys.elf test_NewHope_dec_stack_without_keys.hex