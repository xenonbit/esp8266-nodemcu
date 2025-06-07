#include "LOGService.h"
namespace LOGService
{
    void info(const char *format, ...)
    {
        Serial.print(F("[INFO] "));

        va_list args;
        va_start(args, format);
        Serial.printf(format, args);
        va_end(args);
    }

    void error(const char *format, ...)
    {
        Serial.print(F("[ERROR] "));

        va_list args;
        va_start(args, format);
        Serial.printf(format, args);
        va_end(args);
    }

    void success(const char *format, ...)
    {
        Serial.print(F("[SUCCESS] "));

        va_list args;
        va_start(args, format);
        Serial.printf(format, args);
        va_end(args);
    }

    void drawProgressBar(int current, int total, int barWidth)
    {
        float progress = (float)current / total;
        int filledWidth = (int)(progress * barWidth);

        if (current > total)
            current = total;
        if (filledWidth > barWidth)
            filledWidth = barWidth;
        if (filledWidth < 0)
            filledWidth = 0;

        Serial.print("\r");

        Serial.print("[");

        for (int i = 0; i < filledWidth; ++i)
        {
            Serial.print("=");
        }
        for (int i = filledWidth; i < barWidth; ++i)
        {
            Serial.print("-");
        }

        Serial.printf("] %3d%% (%d/%d)", (int)(progress * 100), current, total);

        if (current == total)
        {
            Serial.println();
        }
    }
}