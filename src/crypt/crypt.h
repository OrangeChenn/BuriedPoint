#pragma once

#include <memory>
#include <string>

namespace buried {

class Crypt {
public:
    ~Crypt() = default;

    virtual std::string EnCrypt(const std::string& input) = 0;
    virtual std::string DeCrypt(const std::string& input) = 0;
    virtual std::string EnCrypt(const void* input, size_t input_size) = 0;
    virtual std::string DeCrypt(const void* input, size_t input_size) = 0;
};

class AESImpl;

// non thread safe
class AESCrypt : public Crypt {
public:
    static std::string GetKey(const std::string& salt, const std::string& password);

    explicit AESCrypt(const std::string& key);
    ~AESCrypt();
    AESCrypt(AESCrypt& crypt) = delete;
    AESCrypt& operator=(const AESCrypt& Crypt) = delete;

    std::string EnCrypt(const std::string& input) override;
    std::string DeCrypt(const std::string& input) override;
    std::string EnCrypt(const void* input, size_t input_size) override;
    std::string DeCrypt(const void* input, size_t input_size) override;

private:
    std::unique_ptr<AESImpl> impl_;
};

} // namespace buried
