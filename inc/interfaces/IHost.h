#ifndef __IHOSTINFO__H__
#define __IHOSTINFO__H__

#include <string>

using ushort = unsigned short;

enum HostIdType {
    HostName,
    IPAddress
};

class IHostInfo
{
    virtual const HostIdType& getType(void) const = 0;
    virtual bool setType(const HostIdType) = 0;

    virtual const std::string& getIdentifier(void) const = 0;
    virtual bool setIdentifier(const std::string&) = 0;

    virtual ~IHostInfo();

    IHostInfo() = delete;
};

#endif // __IHOST__H__