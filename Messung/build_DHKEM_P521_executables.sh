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
cd DH_KEM/
sh compile.sh
cd ..
#Build DH-KEM(P521)-Test
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./avr -I./print -I./DH_KEM -I./Crypto ./DH_KEM/test_P521.cpp -o ./DH_KEM/test_DHKEM_P521.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./DH_KEM/test_DHKEM_P521.elf ./DH_KEM/test_DHKEM_P521.o  ./DH_KEM/P521KEM.o ./Crypto/P521.o ./Crypto/BigNumberUtil.o ./Crypto/RNG.o ./Crypto/ChaCha.o ./Crypto/Crypto.o ./print/print.o ./Crypto/SHA512.o ./Crypto/HKDF.o ./Crypto/Hash.o ./avr/avr.o  ./arduino_core/core.a
avr-objcopy -O ihex -R .eeprom ./DH_KEM/test_DHKEM_P521.elf test_DHKEM_P521.hex
#Build Keygen-Speed-Test
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./cpucycles -I./avr -I./print -I./DH_KEM -I./Crypto ./DH_KEM/test_P521_keygen_speed.cpp -o ./DH_KEM/test_DHKEM_P521_keygen_speed.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./DH_KEM/test_DHKEM_P521_keygen_speed.elf ./DH_KEM/test_DHKEM_P521_keygen_speed.o  ./DH_KEM/P521KEM.o ./Crypto/P521.o ./Crypto/BigNumberUtil.o ./Crypto/RNG.o ./Crypto/ChaCha.o ./Crypto/Crypto.o ./print/print.o ./Crypto/SHA512.o ./Crypto/HKDF.o ./Crypto/Hash.o ./avr/avr.o ./cpucycles/cpucycles.o  ./arduino_core/core.a
avr-objcopy -O ihex -R .eeprom ./DH_KEM/test_DHKEM_P521_keygen_speed.elf test_DHKEM_P521_keygen_speed.hex
#Build Enc-Speed-Test
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./cpucycles -I./avr -I./print -I./DH_KEM -I./Crypto ./DH_KEM/test_P521_enc_speed.cpp -o ./DH_KEM/test_DHKEM_P521_enc_speed.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./DH_KEM/test_DHKEM_P521_enc_speed.elf ./DH_KEM/test_DHKEM_P521_enc_speed.o  ./DH_KEM/P521KEM.o ./Crypto/P521.o ./Crypto/BigNumberUtil.o ./Crypto/RNG.o ./Crypto/ChaCha.o ./Crypto/Crypto.o ./print/print.o ./Crypto/SHA512.o ./Crypto/HKDF.o ./Crypto/Hash.o ./avr/avr.o ./cpucycles/cpucycles.o  ./arduino_core/core.a
avr-objcopy -O ihex -R .eeprom ./DH_KEM/test_DHKEM_P521_enc_speed.elf test_DHKEM_P521_enc_speed.hex
#Build Dec-Speed-Test
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./cpucycles -I./avr -I./print -I./DH_KEM -I./Crypto ./DH_KEM/test_P521_dec_speed.cpp -o ./DH_KEM/test_DHKEM_P521_dec_speed.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./DH_KEM/test_DHKEM_P521_dec_speed.elf ./DH_KEM/test_DHKEM_P521_dec_speed.o  ./DH_KEM/P521KEM.o ./Crypto/P521.o ./Crypto/BigNumberUtil.o ./Crypto/RNG.o ./Crypto/ChaCha.o ./Crypto/Crypto.o ./print/print.o ./Crypto/SHA512.o ./Crypto/HKDF.o ./Crypto/Hash.o ./avr/avr.o ./cpucycles/cpucycles.o  ./arduino_core/core.a
avr-objcopy -O ihex -R .eeprom ./DH_KEM/test_DHKEM_P521_dec_speed.elf test_DHKEM_P521_dec_speed.hex
#Build Keygen-Stack-Test (with keys)
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./cpucycles -I./avr -I./print -I./DH_KEM -I./Crypto ./DH_KEM/test_P521_keygen_stack.cpp -o ./DH_KEM/test_DHKEM_P521_keygen_stack.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./DH_KEM/test_DHKEM_P521_keygen_stack.elf ./DH_KEM/test_DHKEM_P521_keygen_stack.o  ./DH_KEM/P521KEM.o ./Crypto/P521.o ./Crypto/BigNumberUtil.o ./Crypto/RNG.o ./Crypto/ChaCha.o ./Crypto/Crypto.o ./print/print.o ./Crypto/SHA512.o ./Crypto/HKDF.o ./Crypto/Hash.o ./avr/avr.o  ./arduino_core/core.a
avr-objcopy -O ihex -R .eeprom ./DH_KEM/test_DHKEM_P521_keygen_stack.elf test_DHKEM_P521_keygen_stack.hex
#Build Enc-Stack-Test (with keys)
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./cpucycles -I./avr -I./print -I./DH_KEM -I./Crypto ./DH_KEM/test_P521_enc_stack.cpp -o ./DH_KEM/test_DHKEM_P521_enc_stack.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./DH_KEM/test_DHKEM_P521_enc_stack.elf ./DH_KEM/test_DHKEM_P521_enc_stack.o  ./DH_KEM/P521KEM.o ./Crypto/P521.o ./Crypto/BigNumberUtil.o ./Crypto/RNG.o ./Crypto/ChaCha.o ./Crypto/Crypto.o ./print/print.o ./Crypto/SHA512.o ./Crypto/HKDF.o ./Crypto/Hash.o ./avr/avr.o  ./arduino_core/core.a
avr-objcopy -O ihex -R .eeprom ./DH_KEM/test_DHKEM_P521_enc_stack.elf test_DHKEM_P521_enc_stack.hex
#Build Dec-Stack-Test (with keys)
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./cpucycles -I./avr -I./print -I./DH_KEM -I./Crypto ./DH_KEM/test_P521_dec_stack.cpp -o ./DH_KEM/test_DHKEM_P521_dec_stack.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./DH_KEM/test_DHKEM_P521_dec_stack.elf ./DH_KEM/test_DHKEM_P521_dec_stack.o  ./DH_KEM/P521KEM.o ./Crypto/P521.o ./Crypto/BigNumberUtil.o ./Crypto/RNG.o ./Crypto/ChaCha.o ./Crypto/Crypto.o ./print/print.o ./Crypto/SHA512.o ./Crypto/HKDF.o ./Crypto/Hash.o ./avr/avr.o  ./arduino_core/core.a
avr-objcopy -O ihex -R .eeprom ./DH_KEM/test_DHKEM_P521_dec_stack.elf test_DHKEM_P521_dec_stack.hex
#Build Keygen-Stack-Test (without keys)
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./cpucycles -I./avr -I./print -I./DH_KEM -I./Crypto ./DH_KEM/test_P521_keygen_stack_without_keys.cpp -o ./DH_KEM/test_DHKEM_P521_keygen_stack_without_keys.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./DH_KEM/test_DHKEM_P521_keygen_stack_without_keys.elf ./DH_KEM/test_DHKEM_P521_keygen_stack_without_keys.o  ./DH_KEM/P521KEM.o ./Crypto/P521.o ./Crypto/BigNumberUtil.o ./Crypto/RNG.o ./Crypto/ChaCha.o ./Crypto/Crypto.o ./print/print.o ./Crypto/SHA512.o ./Crypto/HKDF.o ./Crypto/Hash.o ./avr/avr.o  ./arduino_core/core.a
avr-objcopy -O ihex -R .eeprom ./DH_KEM/test_DHKEM_P521_keygen_stack_without_keys.elf test_DHKEM_P521_keygen_stack_without_keys.hex
#Build Enc-Stack-Test (without keys)
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./cpucycles -I./avr -I./print -I./DH_KEM -I./Crypto ./DH_KEM/test_P521_enc_stack_without_keys.cpp -o ./DH_KEM/test_DHKEM_P521_enc_stack_without_keys.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./DH_KEM/test_DHKEM_P521_enc_stack_without_keys.elf ./DH_KEM/test_DHKEM_P521_enc_stack_without_keys.o  ./DH_KEM/P521KEM.o ./Crypto/P521.o ./Crypto/BigNumberUtil.o ./Crypto/RNG.o ./Crypto/ChaCha.o ./Crypto/Crypto.o ./print/print.o ./Crypto/SHA512.o ./Crypto/HKDF.o ./Crypto/Hash.o ./avr/avr.o  ./arduino_core/core.a
avr-objcopy -O ihex -R .eeprom ./DH_KEM/test_DHKEM_P521_enc_stack_without_keys.elf test_DHKEM_P521_enc_stack_without_keys.hex
#Build Dec-Stack-Test (without keys)
avr-g++ -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I./cpucycles -I./avr -I./print -I./DH_KEM -I./Crypto ./DH_KEM/test_P521_dec_stack_without_keys.cpp -o ./DH_KEM/test_DHKEM_P521_dec_stack_without_keys.o
avr-gcc -w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega2560 -o ./DH_KEM/test_DHKEM_P521_dec_stack_without_keys.elf ./DH_KEM/test_DHKEM_P521_dec_stack_without_keys.o  ./DH_KEM/P521KEM.o ./Crypto/P521.o ./Crypto/BigNumberUtil.o ./Crypto/RNG.o ./Crypto/ChaCha.o ./Crypto/Crypto.o ./print/print.o ./Crypto/SHA512.o ./Crypto/HKDF.o ./Crypto/Hash.o ./avr/avr.o  ./arduino_core/core.a
avr-objcopy -O ihex -R .eeprom ./DH_KEM/test_DHKEM_P521_dec_stack_without_keys.elf test_DHKEM_P521_dec_stack_without_keys.hex