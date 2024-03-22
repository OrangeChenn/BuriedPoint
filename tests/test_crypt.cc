#include "gtest/gtest.h"
#include "../src/crypt/Crypt.h"

#include <string>
#include <iostream>

TEST(CryptTest, basicTest) {
    std::cout << "crypt test" << std::endl;
    std::string key = buried::AESCrypt::GetKey("salt", "password");
    EXPECT_TRUE(!key.empty());

    buried::Crypt* crypt = new buried::AESCrypt(key);
    std::string str = "Orange Chen";
    std::string encrypt = crypt->EnCrypt(str);
    EXPECT_TRUE(!encrypt.empty());
    std::string decrypt = crypt->DeCrypt(encrypt);
    EXPECT_TRUE(!decrypt.empty());
    EXPECT_EQ(str, decrypt);
    delete crypt;
}
