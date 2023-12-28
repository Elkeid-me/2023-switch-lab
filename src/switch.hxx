#ifndef SWITCH_HXX
#define SWITCH_HXX

#include <cstdint>
#include <unordered_map>

class SwitchBase
{
public:
    SwitchBase() = default;
    ~SwitchBase() = default;

    virtual void InitSwitch(int numPorts) = 0;
    virtual int ProcessFrame(int inPort, char *framePtr) = 0;
};

extern SwitchBase *CreateSwitchObject();
extern int PackFrame(char *unpacked_frame, char *packed_frame, int frame_length);
extern int UnpackFrame(char *unpacked_frame, char *packed_frame, int frame_length);

class Switch : public SwitchBase
{
private:
    int ports{0};
    std::unordered_map<std::uint64_t, std::pair<int, int>> table;

public:
    void InitSwitch(int numPorts) override;
    int ProcessFrame(int inPort, char *framePtr) override;
};

#endif
