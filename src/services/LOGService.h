#ifndef LOGService_H
#define LOGService_H

#include <Arduino.h>

namespace LOGService
{
    void info(const char *format, ...);
    void error(const char *format, ...);
    void success(const char *format, ...);
    void drawProgressBar(int current, int total, int barWidth);
}
#endif
