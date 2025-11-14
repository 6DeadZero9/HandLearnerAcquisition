#include <cstring>
#include <iostream>
#include <stdarg.h>

#include <Logger.h>
#include <Tools.h>

namespace tools {
    std::vector<uint8_t> hex_to_bytes(const std::string& hex) {
        std::vector<uint8_t> bytes;

        if (hex.length() % 2 == 0) {
            bytes.reserve(hex.length() / 2);

            for (size_t i = 0; i < hex.length(); i += 2) {
                uint8_t byte = 0;
                if ((hex[i] <= 0xFF) && (hex[i + 1] <= 0xFF)) {
                    byte = std::stoi(hex.substr(i, 2), nullptr, 16);
                }
                bytes.push_back(byte);
            }
        }

        return bytes;
    }

    std::string bytes_to_hex(const std::string& buf) {
        return bytes_to_hex((const uint8_t*)buf.c_str(), buf.length());
    }

    std::string bytes_to_hex(const uint8_t* buf, size_t len) {
        std::ostringstream oss;

        oss << std::hex << std::setfill('0');
        for (size_t i = 0; i < len; i++) {
            oss << std::setw(2) << static_cast<int>(buf[i]);
        }
        return oss.str();
    }

    std::string format_string(const std::string& format, ...) {
        va_list args;

        va_start(args, format);
        int size = std::vsnprintf(nullptr, 0, format.c_str(), args);
        va_end(args);

        if (size <= 0) {
            return "";
        }

        std::vector<char> buffer(size + 1);

        va_start(args, format);
        std::vsnprintf(buffer.data(), buffer.size(), format.c_str(), args);
        va_end(args);

        return std::string(buffer.data(), size);
    }

    std::optional<SimpleBLE::Adapter> getAdapter() {
        if (!SimpleBLE::Adapter::bluetooth_enabled()) {
            LOG(logger::LOG_ERR, "Bluetooth is not enabled!");
            return {};
        }

        auto adapter_list = SimpleBLE::Adapter::get_adapters();

        if (adapter_list.empty()) {
            LOG(logger::LOG_ERR, "No adapter was found.");
            return {};
        }

        return adapter_list.at(0);
    }

    float map_range(float value, float min_in_range, float max_in_range, float min_out_range, float max_out_range) {
        float x = (value - min_in_range) / (max_in_range - min_in_range);
        return min_out_range + (max_out_range - min_out_range) * x;
    }

} // namespace tools
