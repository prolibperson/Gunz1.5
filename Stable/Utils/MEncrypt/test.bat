@echo off
echo Generate public and private key...
MEncrypt -c
echo done.

echo Encrypt from test.htm to encrypted.bin...
MEncrypt -e publicKey.txt test.htm encrypted.bin
echo done.

echo Decrypt from encrypted.bin to test.htm...
MEncrypt -d privateKey.txt encrypted.bin decrypted.htm
echo done.

echo y | comp test.htm decrypted.htm

pause
