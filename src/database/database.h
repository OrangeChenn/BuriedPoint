#pragma once

#include <stdint.h>

#include <memory>
#include <vector>

namespace buried {

class BuriedDbImpl;

class BuriedDb {
public:
    struct Data
    {
        int32_t id;
        int32_t priority;
        uint64_t timestamp;
        std::vector<char> content;
    };

public:
    BuriedDb(const std::string& db_path);
    ~BuriedDb();
    void InsertData(const BuriedDb::Data& data);
    void DeleteData(const BuriedDb::Data& data);
    void DeleteDatas(const std::vector<BuriedDb::Data>& datas);
    std::vector<BuriedDb::Data> QueryData(int32_t limit_size);

private:
    std::unique_ptr<BuriedDbImpl> impl_;
};

} // namespace buried
