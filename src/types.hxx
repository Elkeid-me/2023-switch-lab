#ifndef TYPES
#define TYPES

#include <cstdint>

constexpr std::uint16_t ETHER_DATA_TYPE{0};
constexpr std::uint16_t ETHER_CONTROL_TYPE{1};

// MAC Table aging.
constexpr std::uint32_t ETHER_COMMAND_TYPE_AGING{0};
constexpr std::uint32_t ETHER_MAC_AGING_THRESHOLD{10};

// frame head byte
constexpr std::uint8_t FRAME_DELI{0xDE};
constexpr std::uint32_t MAX_PACKED_FRAME{3090};

class ether_header
{
private:
    std::uint8_t data[16];

public:
    std::uint64_t get_dst() const
    {
        return (__builtin_bswap64(*reinterpret_cast<const std::uint64_t *>(data)) &
                0xffffffffffff0000) >>
               16;
    }

    std::uint64_t get_src() const
    {
        return (__builtin_bswap64(*reinterpret_cast<const std::uint64_t *>(data + 6)) &
                0xffffffffffff0000) >>
               16;
    }

    std::uint16_t get_type() const
    {
        return *reinterpret_cast<const std::uint16_t *>(data + 12);
    }

    std::uint16_t get_length() const
    {
        return *reinterpret_cast<const std::uint16_t *>(data + 14);
    }
};

#endif
