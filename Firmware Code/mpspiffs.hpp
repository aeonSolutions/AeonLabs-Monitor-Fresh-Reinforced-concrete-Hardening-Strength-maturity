#pragma once

#include <Arduino.h>
#include <FS.h>

class SPIFFS : public FS
{
private:
    bool m_mounted;
    const char * m_partition = NULL;
public:
    SPIFFS(fs::FSImplPtr impl);
    bool begin(const char * partition = NULL, bool formatOnFail = false, const char * basePath="/spiffs", uint8_t maxOpenFiles=10);
    bool end();
    bool format(const char * partition = NULL);
    bool getSize(uint32_t &total, uint32_t &used);
    bool isMounted();
    const char* mountedPartition();
};

extern SPIFFS Spiffs;
