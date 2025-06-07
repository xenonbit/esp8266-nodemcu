#ifndef ROMService_H
#define ROMService_H

#include <Arduino.h>

namespace ROMService
{
    void initialize(size_t romSize);
    String readString(int address, int maxLen);
    void writeString(int address, const String &data, unsigned int maxLen);
    void clear();
}
#endif
