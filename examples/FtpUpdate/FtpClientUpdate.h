#ifndef  ESPFTPCLIENTUPDATE_H_
#define ESPFTPCLIENTUPDATE_H_

#include <Arduino.h>
#include <WiFi.h>
#include <Update.h>
#include <FS.h>
#include <WiFiClient.h>
#include <TimeOutEvent.h>

#define DEBUG_ESP_FTPCLIENT_UPDATE
#define DEBUG_ESP_FTPCLIENT_PORT  Serial

#ifdef DEBUG_ESP_FTPCLIENT_UPDATE
#ifdef DEBUG_ESP_FTPCLIENT_PORT
#define DEBUG_FTPCLIENT_UPDATE(fmt, ...) DEBUG_ESP_FTPCLIENT_PORT.printf_P(PSTR("\r\n>FTP_CLIENT< " fmt), ##__VA_ARGS__)
#endif
#endif

#ifndef DEBUG_FTPCLIENT_UPDATE
#define DEBUG_FTPCLIENT_UPDATE(...)
#endif

#define FTP_UPDATE_ERROR_OK                 (0)
#define FTP_UPDATE_ERROR_WRITE              (1)
#define FTP_UPDATE_ERROR_ERASE              (2)
#define FTP_UPDATE_ERROR_READ               (3)
#define FTP_UPDATE_ERROR_SPACE              (4)
#define FTP_UPDATE_ERROR_SIZE               (5)
#define FTP_UPDATE_ERROR_STREAM             (6)
#define FTP_UPDATE_ERROR_MD5                (7)
#define FTP_UPDATE_ERROR_FLASH_CONFIG       (8)
#define FTP_UPDATE_ERROR_NEW_FLASH_CONFIG   (9)
#define FTP_UPDATE_ERROR_MAGIC_BYTE         (10)
#define FTP_UPDATE_ERROR_BOOTSTRAP          (11)

#define FTP_UPDATE_ERROR_COMMAND_SERVER     (100)
#define FTP_UPDATE_ERROR_SUPPORT_SERVER     (101)
#define FTP_UPDATE_ERROR_USER_SERVER        (102)
#define FTP_UPDATE_ERROR_PASS_SERVER        (103)
#define FTP_UPDATE_ERROR_SYST_SERVER        (104)
#define FTP_UPDATE_ERROR_TYPE_SERVER        (105)
#define FTP_UPDATE_ERROR_PASV_SERVER        (106)
#define FTP_UPDATE_ERROR_DATA_SERVER        (107)
#define FTP_UPDATE_ERROR_STOP_DATA_SERVER   (108)
#define FTP_UPDATE_ERROR_QUIT_SERVER        (109)
#define FTP_UPDATE_ERROR_SIZE_FILE          (110)

class ESPFTPCLIENTUpdate
{
public:
    ESPFTPCLIENTUpdate(void);
    bool update(const String& host, const String& user, const String& pass, const String& path, String& _md5);
    uint8_t getError(){ return _error; }
    void clearError(){ _error = FTP_UPDATE_ERROR_OK; }
    bool hasError(){ return _error != FTP_UPDATE_ERROR_OK; }
protected:
    uint8_t _error;
    void efail(WiFiClient& _Client);
    bool eRcv(WiFiClient& _Client, char *BufRx, byte Len);
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_FTPCLIENTUPDATE)
extern ESPFTPCLIENTUpdate FtpClientUpdate;
#endif

#endif
