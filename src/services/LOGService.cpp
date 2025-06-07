#include "LOGService.h"
namespace LOGService
{
#define LOG_BUFFER_SIZE 256

    void info(const char *format, ...)
    {
        char log_buffer[LOG_BUFFER_SIZE];
        Serial.print(F("[INFO] "));
        va_list args;
        va_start(args, format);
        int printed_len = vsnprintf(log_buffer, sizeof(log_buffer), format, args);
        va_end(args);
        if (printed_len > 0)
        {
            Serial.print(log_buffer);
        }
    }

    void error(const char *format, ...)
    {
        char log_buffer[LOG_BUFFER_SIZE];
        Serial.print(F("[ERROR] "));
        va_list args;
        va_start(args, format);
        int printed_len = vsnprintf(log_buffer, sizeof(log_buffer), format, args);
        va_end(args);
        if (printed_len > 0)
        {
            Serial.print(log_buffer);
        }
    }

    void success(const char *format, ...)
    {
        char log_buffer[LOG_BUFFER_SIZE];
        Serial.print(F("[SUCCESS] "));
        va_list args;
        va_start(args, format);
        int printed_len = vsnprintf(log_buffer, sizeof(log_buffer), format, args);
        va_end(args);
        if (printed_len > 0)
        {
            Serial.print(log_buffer);
        }
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