#pragma once 

class Arch
{
    public:
        inline const char* GetArchitecture()
        {
            return m_archStr;
        }
    private:
        const char* m_archStr = _ARCH_STR;
};

