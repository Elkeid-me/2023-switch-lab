#ifndef TYPES
#define TYPES

#include <cstdint>

#include <chrono>

constexpr std::size_t ETH_ALEN{6};
using  mac_addr_t = std::uint8_t[ETH_ALEN];
using timestamp_t = std::chrono::time_point<std::chrono::steady_clock>;

constexpr std::uint16_t ETHER_DATA_TYPE{0};
constexpr std::uint16_t ETHER_CONTROL_TYPE{1};

// MAC Table aging.
constexpr std::uint32_t ETHER_COMMAND_TYPE_AGING{0};
constexpr std::uint32_t ETHER_MAC_AGING_THRESHOLD{10};

// frame head byte
constexpr std::uint8_t FRAME_DELI{0xDE};
constexpr std::uint32_t MAX_PACKED_FRAME{3090};

struct ether_header
{
    mac_addr_t ether_dest;
    mac_addr_t ether_src;
    std::uint16_t ether_type;
    std::uint16_t length;
};

static_assert(sizeof(ether_header) == 16);

#endif // ! TYPES