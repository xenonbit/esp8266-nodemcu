#include "HTTPRequestService.h"
#include <WiFiClient.h>
namespace HTTPRequestService
{
    HTTPResponse GET(const String &url)
    {
        HTTPClient http;
        WiFiClient client;
        http.begin(client, url);
        int httpCode = http.GET();
        http.end();
        return HTTPResponse(httpCode);
    }
}