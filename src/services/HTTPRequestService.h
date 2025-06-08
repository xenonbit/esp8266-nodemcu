#ifndef HTTPRequestService_H
#define HTTPRequestService_H

#include <Arduino.h>
#include <ESP8266HTTPClient.h>

namespace HTTPRequestService
{
    class HTTPResponse
    {
    public:
        HTTPResponse(int code) : code(code) {}
        int code;
        bool ok()
        {
            return code == HTTP_CODE_OK;
        }
    };

    HTTPResponse GET(const String &url);
}
#endif
