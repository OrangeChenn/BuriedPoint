#include "common_service.h"

#include <cstdio>
#include <ctime>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <sys/utsname.h>

#ifdef __WIN32
#include <window.hpp>
#endif

#include "../buried_config.h"

namespace buried
{
    
CommonService::CommonService() { Init(); }

#ifdef _WIN32
static void WriteRegister(const std::string& key, const std::string& value) {
    HKEY h_key;
    LONG ret = ::RegCreateKeyExA(HKEY_CURRENT_USER, "Software\\Buried", 0, NULL,
                                REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,
                                &h_key, NULL);
    if (ret != ERROR_SUCCESS) {
        return;
    }
    ret = ::RegSetValueExA(h_key, key.c_str(), 0, REG_SZ,
                            reinterpret_cast<const BYTE*>(value.c_str()),
                            value.size());
    if (ret != ERROR_SUCCESS) {
        return;
    }
    ::RegCloseKey(h_key);
}

static std::string ReadRegister(const std::string& key) {
    HKEY h_key;
    LONG ret = ::RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Buried", 0,
                                KEY_ALL_ACCESS, &h_key);
    if (ret != ERROR_SUCCESS) {
        return "";
    }
    char buf[1024] = {0};
    DWORD buf_size = sizeof(buf);
    ret = ::RegQueryValueExA(h_key, key.c_str(), NULL, NULL,
                            reinterpret_cast<BYTE*>(buf), &buf_size);
    if (ret != ERROR_SUCCESS) {
        return "";
    }
    ::RegCloseKey(h_key);
    return buf;
}
#endif

static std::string GetDeviceId() {
#ifdef _WIN32
    static constexpr auto kDeviceIdKey = "device_id";
    static std::string device_id = ReadRegister(kDeviceIdKey);
    if (device_id.empty()) {
        device_id = CommonService::GetRandomId();
        WriteRegister(kDeviceIdKey, device_id);
    }
    return device_id;
#else
    std::string device_id = "";
    for(const auto& entry : std::filesystem::directory_iterator("/sys/class/net")) {
        std::string interface_name = entry.path().filename().string();
        if(interface_name != "lo") {
            std::string mac_address;
            std::ifstream ifs("/sys/class/net/" + interface_name + "/address");
            if(ifs) {
                std::getline(ifs, mac_address);
            }
            device_id = mac_address;
            break;
        }
    }
    return device_id;
#endif
}

static std::string GetSystemVersion() {
#ifdef _WIN32
    OSVERSIONINFOEXA os_version_info;
    ZeroMemory(&os_version_info, sizeof(OSVERSIONINFOEXA));
    os_version_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);
    ::GetVersionExA(reinterpret_cast<OSVERSIONINFOA*>(&os_version_info));
    std::string system_version =
        std::to_string(os_version_info.dwMajorVersion) + "." +
        std::to_string(os_version_info.dwMinorVersion) + "." +
        std::to_string(os_version_info.dwBuildNumber);
    return system_version;
#else
    struct utsname os_info;
    if(uname(&utsname) != 0) {
        return "";
    }
    std::string system_version = os_info.release;
    return system_version;
#endif
}

static std::string GetDeviceName() {
#ifdef _WIN32
    char buf[1024] = {0};
    DWORD buf_size = sizeof(buf);
    ::GetComputerNameA(buf, &buf_size);
    std::string device_name = buf;
    return device_name;
#else
    FILE file = fopen("/proc/sys/kernel/hostname", "r");
    if(file) {
        char device_name[256];
        if(fgets(device_name, sizeof(device_name), file)) {
            return device_name;
        } else {
            return "";
        }
    }
    return "";
#endif
}

std::string GetLifeCycleId() {
    static std::string life_cycle_id = CommonService::GetRandomId();
    return life_cycle_id;
}

std::string CommonService::GetProcessTime() {
#ifdef _WIN32
    DWORD pid = ::GetCurrentProcessId();
    HANDLE h_process =
        ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (h_process == NULL) {
        return "";
    }
    FILETIME create_time;
    FILETIME exit_time;
    FILETIME kernel_time;
    FILETIME user_time;
    BOOL ret = ::GetProcessTimes(h_process, &create_time, &exit_time,
                                &kernel_time, &user_time);
    ::CloseHandle(h_process);
    if (ret == 0) {
        return "";
    }

    FILETIME create_local_time;
    ::FileTimeToLocalFileTime(&create_time, &create_local_time);

    SYSTEMTIME create_sys_time;
    ::FileTimeToSystemTime(&create_local_time, &create_sys_time);
    char buf[128] = {0};
    // year month day hour minute second millisecond
    sprintf_s(buf, "%04d-%02d-%02d %02d:%02d:%02d.%03d", create_sys_time.wYear,
                create_sys_time.wMonth, create_sys_time.wDay, create_sys_time.wHour,
                create_sys_time.wMinute, create_sys_time.wSecond,
                create_sys_time.wMilliseconds);
    return buf;
#else
    timespec ts_creation;
    if(clock_gettime(CLOCK_REALTIME, &ts_creation) != 0) {
        return "";
    }
    time_t creation_time = ts_creation.tv_sec;
    struct tm* tm_creation = localtime(&creation_time);
    char buf[128] = {0};
    // year month day hour minute second
    sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d", 
            tm_creation->tm_year + 1900, tm_creation->tm_mon + 1, tm_creation->tm_mday,
            tm_creation->tm_hour, tm_creation->tm_min, tm_creation->tm_sec);
    return buf;
#endif
}

std::string CommonService::GetNowDate() {
    auto t =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    return std::ctime(&t);
}

std::string CommonService::GetRandomId() {
    static constexpr size_t len = 32;
    static constexpr auto chars =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    static std::mt19937_64 rng{std::random_device{}()};
    static std::uniform_int_distribution<size_t> dist{0, 60};
    std::string result;
    result.reserve(len);
    std::generate_n(std::back_inserter(result), len,
                    [&]() { return chars[dist(rng)]; });
    return result;
}

void CommonService::Init() {
    system_version = GetSystemVersion();
    device_name = GetDeviceName();
    device_id = GetDeviceId();
    buried_version = PROJECT_VER;
    lifecycle_id = GetLifeCycleId();
}
    
} // namespace buried
