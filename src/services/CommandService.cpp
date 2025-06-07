#include "CommandService.h"
namespace CommandService
{
    String _command = "";

    String waitForInput()
    {
        while (true)
        {
            while (Serial.available())
            {
                char i = Serial.read();

                if (i == '\n')
                {
                    _command.trim();
                    String result = _command;
                    _command = "";

                    return result;
                }
                else
                {
                    _command += i;
                }
            }
        }
    }
}