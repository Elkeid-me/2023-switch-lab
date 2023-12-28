#ifndef SWITCH_HXX
#define SWITCH_HXX

#include "types.hxx"

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

// TODO : Implement your switch class.

#endif
