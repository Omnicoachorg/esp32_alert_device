#include "UpdateOverMqtt.h"

namespace omnicoach
{
    void UpdateOverMqtt::update(String url, int port)
    {
        String bin = getBinName(url);
        String host = getHostName(url);

        DEBUG_PRINTLN("Connecting to: " + host);
        if (client.connect(host.c_str(), port))
        {
            // Connection Succeed.
            // Fecthing the bin
            DEBUG_PRINTLN("Fetching Bin: " + bin);

            // Get the contents of the bin file
            client.print(String("GET ") + bin + " HTTP/1.1\r\n" +
                         "Host: " + host + "\r\n" +
                         "Cache-Control: no-cache\r\n" +
                         "Connection: close\r\n\r\n");

            unsigned long timeout = millis();

            while (client.available() == 0)
            {
                if (millis() - timeout > 5000)
                {
                    DEBUG_PRINTLN("Client Timeout !");
                    client.stop();
                    return;
                }
            }
            while (client.available())
            {
                // read line till /n
                String line = client.readStringUntil('\n');
                // remove space, to check if the line is end of headers
                line.trim();

                // if the the line is empty,
                // this is end of headers
                // break the while and feed the
                // remaining `client` to the
                // Update.writeStream();
                if (!line.length())
                {
                    // headers ended
                    break; // and get the OTA started
                }

                // Check if the HTTP Response is 200
                // else break and Exit Update
                if (line.startsWith("HTTP/1.1"))
                {
                    DEBUG_PRINTLN(line);
                    if (line.indexOf("200") < 0)
                    {
                        DEBUG_PRINTLN("Got a non 200 status code from server. Exiting OTA Update.");
                        break;
                    }
                }

                // extract headers here
                // Start with content length
                if (line.startsWith("Content-Length: "))
                {
                    contentLength = atoi((getHeaderValue(line, "Content-Length: ")).c_str());
                    DEBUG_PRINTLN("Got " + String(contentLength) + " bytes from server");
                }

                // Next, the content type
                if (line.startsWith("Content-Type: "))
                {
                    String contentType = getHeaderValue(line, "Content-Type: ");
                    DEBUG_PRINTLN("Got " + contentType + " payload.");
                    if (contentType == "application/octet-stream")
                    {
                        isValidContentType = true;
                    }
                }
            }
        }
        else
        {
            // Connect failed
            DEBUG_PRINTLN("Connection to " + host + " failed. Please check your setup");
            // retry??
        }

        // Check what is the contentLength and if content type is `application/octet-stream`
        DEBUG_PRINTLN("contentLength : " + String(contentLength) + ", isValidContentType : " + String(isValidContentType));

        // check contentLength and content type
        if (contentLength && isValidContentType)
        {
            // Check if there is enough to OTA Update
            bool canBegin = Update.begin(contentLength);
            if (canBegin)
            {
                DEBUG_PRINTLN("Begin OTA. This may take 2 - 5 mins to complete. Things might be quite for a while.. Patience!");
                size_t written = Update.writeStream(client);

                if (written == contentLength)
                {
                    DEBUG_PRINTLN("Written : " + String(written) + " successfully");
                }
                else
                {
                    DEBUG_PRINTLN("Written only : " + String(written) + "/" + String(contentLength) + ". Retry?");
                    // retry??
                }

                if (Update.end())
                {
                    DEBUG_PRINTLN("OTA done!");
                    if (Update.isFinished())
                    {
                        DEBUG_PRINTLN("Update successfully completed. Rebooting.");
                        ESP.restart();
                    }
                    else
                    {
                        DEBUG_PRINTLN("Update not finished? Something went wrong!");
                    }
                }
                else
                {
                    DEBUG_PRINTLN("Error Occurred. Error #: " + String(Update.getError()));
                }
            }
            else
            {
                // not enough space to begin OTA
                // Understand the partitions and
                // space availability
                DEBUG_PRINTLN("Not enough space to begin OTA");
                client.flush();
            }
        }
        else
        {
            DEBUG_PRINTLN("There was no content in the response");
            client.flush();
        }
    }

    String UpdateOverMqtt::getHeaderValue(String header, String headerName)
    {
        return header.substring(strlen(headerName.c_str()));
    }

    String UpdateOverMqtt::getBinName(String url)
    {
        int index = 0;

        // Search for last /
        for (int i = 0; i < url.length(); i++)
        {
            if (url[i] == '/')
            {
                index = i;
            }
        }

        String binName = "";

        // Create binName
        for (int i = index; i < url.length(); i++)
        {
            binName += url[i];
        }

        return binName;
    }

    String UpdateOverMqtt::getHostName(String url)
    {
        int index = 0;

        // Search for last /
        for (int i = 0; i < url.length(); i++)
        {
            if (url[i] == '/')
            {
                index = i;
            }
        }

        String hostName = "";

        // Create binName
        for (int i = 0; i < index; i++)
        {
            hostName += url[i];
        }

        return hostName;
    }

} // omnicoach