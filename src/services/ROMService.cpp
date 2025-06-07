#include "ROMService.h"
#include <EEPROM.h>
namespace ROMService
{
    size_t _eepromSize = 0;

    void initialize(size_t romSize)
    {
        _eepromSize = romSize;
        EEPROM.begin(_eepromSize);
    }

    String readString(int address, int maxLen)
    {
        String data = "";
        char charRead;
        for (int i = 0; i < maxLen; i++)
        {
            charRead = EEPROM.read(address + i);
            if (charRead == '\0')
            {
                break;
            }
            data += charRead;
        }
        return data;
    }

    void writeString(int address, const String &data, unsigned int maxLen)
    {
        for (unsigned int i = 0; i < maxLen; i++)
        {
            if (i < data.length())
            {
                EEPROM.write(address + i, data.charAt(i));
            }
            else
            {
                EEPROM.write(address + i, '\0');
            }
        }
        EEPROM.commit();
    }

    void clear()
    {
        if (_eepromSize == 0)
        {
            return;
        }

        for (size_t i = 0; i < _eepromSize; i++)
        {
            EEPROM.write(i, '\0');
        }
        EEPROM.commit();
    }
}