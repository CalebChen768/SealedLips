#if 0

#include <stdio.h>

#include <iostream>
#include <fstream>
#include <sstream>


#include <cryptopp/filters.h>
#include <cryptopp/modes.h>

using namespace std;

byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ], iv[ CryptoPP::AES::BLOCKSIZE];

void initKV()
{
    memset( key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH );
    memset( iv, 0x00, CryptoPP::AES::BLOCKSIZE );

    // 或者也可以
    /*
    char tmpK[] = "1234567890123456";
    char tmpIV[] = "1234567890123456";
    for (int j = 0; j < CryptoPP::AES::DEFAULT_KEYLENGTH; ++j)
    {
        key[j] = tmpK[j];
    }

    for (int i = 0; i < CryptoPP::AES::BLOCKSIZE; ++i)
    {
        iv[i] = tmpIV[i];
    }
    */
}

string decrypt(string cipherTextHex)
{
    string cipherText;
    string decryptedText;

    int i = 0;
    while(true)
    {
        char c;
        int x;
        stringstream ss;
        ss<<hex<<cipherTextHex.substr(i, 2).c_str();
        ss>>x;
        c = (char)x;
        cipherText += c;
        if(i >= cipherTextHex.length() - 2)break;
        i += 2;
    }

    //
    CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption( aesDecryption, iv );
    CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink( decryptedText ));
    stfDecryptor.Put( reinterpret_cast<const unsigned char*>( cipherText.c_str() ), cipherText.size());

    stfDecryptor.MessageEnd();

    return decryptedText;
}

string readCipher()
{
    ifstream in("/tmp/cipher.data");

    string line;
    string decryptedText;
    while(getline(in, line))
    {
        if(line.length() > 1)
        {
            decryptedText += decrypt(line) + "\n";
        }
        line.clear();
    }

    cout<<"readCipher finish "<<endl;
    in.close();

    return decryptedText;
}

int main()
{
    initKV();
    string text = readCipher();
    cout<<"text : "<<text<<endl;
    return 0;
}
#endif
