#include "switch.hxx"
#include "types.hxx"
#include <bit>
#include <cstddef>
#include <cstdint>
// #include <cstring>
// #include <iomanip>
// #include <numeric>
// #include <sstream>
// #include <string>
// #include <string_view>

SwitchBase *CreateSwitchObject() { return new Switch; }

int PackFrame(char *unpacked_frame, char *packed_frame, int frame_length)
{
    // std::ostringstream os(std::ios::binary);
    // os << std::quoted<char>(std::string_view(unpacked_frame, frame_length), FRAME_DELI,
    //                         FRAME_DELI);
    // std::string s{os.str()};
    // std::uint8_t xor_result{static_cast<uint8_t>(std::reduce(
    //     s.begin(), s.end() - 1, '\0', [](char lhs, char rhs) { return lhs ^ rhs; }))};
    // s.back() = std::popcount(xor_result) % 2 != 0 ? '\1' : '\0';
    // std::memcpy(packed_frame, s.c_str(), s.length());
    // return s.length();
    std::uint8_t *packed_ptr{reinterpret_cast<std::uint8_t *>(packed_frame)};
    std::uint8_t *unpacked_ptr{reinterpret_cast<std::uint8_t *>(unpacked_frame)};
    std::size_t unpacked_size{static_cast<size_t>(frame_length)};
    std::uint8_t xor_result{FRAME_DELI};

    packed_ptr[0] = FRAME_DELI;
    std::size_t process_ptr{1};
    for (std::size_t i{0}; i < unpacked_size; i++)
    {
        if (unpacked_ptr[i] == FRAME_DELI)
        {
            packed_ptr[process_ptr] = FRAME_DELI;
            packed_ptr[process_ptr + 1] = FRAME_DELI;
            process_ptr += 2;
        }
        else
        {
            packed_ptr[process_ptr] = unpacked_ptr[i];
            xor_result ^= unpacked_ptr[i];
            process_ptr++;
        }
    }
    if (std::popcount(xor_result) % 2 != 0)
        packed_ptr[process_ptr] = '\1';
    else
        packed_ptr[process_ptr] = '\0';

    process_ptr++;
    return process_ptr;
}

int UnpackFrame(char *unpacked_frame, char *packed_frame, int frame_length)
{
    // if (static_cast<std::uint8_t>(packed_frame[0]) != FRAME_DELI)
    //     return -1;
    // std::uint8_t xor_result{
    //     static_cast<uint8_t>(std::reduce(packed_frame, packed_frame + frame_length, '\0',
    //                                      [](char lhs, char rhs) { return lhs ^ rhs; }))};
    // if (std::popcount(xor_result) % 2 != 0)
    //     return -1;
    // std::istringstream is(std::string(packed_frame, frame_length));
    // std::string result;
    // is >> std::quoted<char>(result, FRAME_DELI, FRAME_DELI);
    // if (!is.eof())
    //     return -1;
    // std::memcpy(unpacked_frame, result.c_str(), result.length() - 1);
    // return result.length() - 1;
    std::uint8_t *packed_ptr{reinterpret_cast<std::uint8_t *>(packed_frame)};
    std::uint8_t *unpacked_ptr{reinterpret_cast<std::uint8_t *>(unpacked_frame)};
    std::size_t packed_size{static_cast<size_t>(frame_length - 1)};
    std::size_t process_ptr{0};
    bool de_flag{false};
    std::uint8_t xor_result{FRAME_DELI};

    if (packed_ptr[0] != FRAME_DELI)
        return -1;

    for (std::size_t i{1}; i < packed_size; i++)
    {
        xor_result ^= packed_ptr[i];
        if (de_flag)
        {
            if (packed_ptr[i] != FRAME_DELI)
                return -1;
            unpacked_ptr[process_ptr] = packed_ptr[i];
            process_ptr++;
            de_flag = false;
        }
        else if (packed_ptr[i] == FRAME_DELI)
            de_flag = true;
        else
        {
            unpacked_ptr[process_ptr] = packed_ptr[i];
            process_ptr++;
        }
    }

    xor_result ^= packed_ptr[packed_size];

    if (std::popcount(xor_result) % 2 != 0)
        return -1;
    return process_ptr;
}

void Switch::InitSwitch(int numPorts) { ports = numPorts; }

int Switch::ProcessFrame(int inPort, char *framePtr)
{
    ether_header *header{reinterpret_cast<ether_header *>(framePtr)};
    if (header->get_type() == ETHER_CONTROL_TYPE)
    {
        if (header->get_length() == 0 && inPort == 1)
        {
            auto iter{table.begin()}, end{table.end()};
            while (iter != end)
            {
                iter->second.second--;
                if (iter->second.second == 0)
                    iter = table.erase(iter);
                else
                    ++iter;
            }
        }
        return -1;
    }

    std::uint64_t src{header->get_src()}, dst{header->get_dst()};
    auto outport_iter{table.find(dst)};
    table[src] = {inPort, 10};

    if (outport_iter == table.end())
        return 0;

    if (outport_iter->second.first == inPort)
        return -1;
    return outport_iter->second.first;
}
