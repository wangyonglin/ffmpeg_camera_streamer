#include <Blackliner/String.h>




char *StringRegister(char **deststring, char *datastring, size_t datalength)
{
    if ((*deststring) = malloc(datalength + 1))
    {
        memset((*deststring), 0x00, datalength + 1);
        memcpy((*deststring), datastring, datalength);
        return (*deststring);
    }
    return NULL;
}

void StringDestory(char *deststring)
{
    if (deststring)
    {
        free(deststring);
    }
}

DataString DataStringRegister(char *datastring, size_t datalength)
{
    DataString deststring;
    memset(&deststring, 0x00, sizeof(DataString));
    StringRegister(&(deststring.datastring), datastring, datalength);
    deststring.datalength = datalength;
    return deststring;
}



void DataStringDestory(DataString deststring)
{
    if (deststring.datastring)
    {
        StringDestory(deststring.datastring);
    }
    deststring.datalength = 0;
}

char *BufferRegister(uint8_t **deststring, uint8_t *datastring, size_t datalength)
{
    if ((*deststring) = malloc(datalength + 1))
    {
        memset((*deststring), 0x00, datalength + 1);
        memcpy((*deststring), datastring, datalength);
        return (*deststring);
    }
    return NULL;
}

void BufferDestory(uint8_t *deststring)
{
    if (deststring)
    {
        free(deststring);
    }
}

DataBuffer DataBufferRegister(uint8_t *databuffer, size_t datalength)
{
    DataBuffer destbuffer;
    memset(&destbuffer, 0x00, sizeof(DataBuffer));
    BufferRegister(&(destbuffer.ptr), databuffer, datalength);
    destbuffer.size = datalength;
    return destbuffer;
}

void DataBufferDestory(DataBuffer databuffer)
{
    if (databuffer.ptr)
    {
        BufferDestory(databuffer.ptr);
    }
    databuffer.size = 0;
}
// char *StringTimeutc()
// {
//     char tmpstring[80];
//     memset(tmpstring, 0x00, sizeof(tmpstring));
//     UtcTime result;
//     time_t rawtime;
//     time(&rawtime);
//     result = unix32_to_UTC(rawtime);
//     sprintf(tmpstring, "%04d-%02d-%02dT%02d:%02d:%02dZ", result.year, result.month, result.day,
//             result.hour, result.minute, result.second); // 以年月日_时分秒的形式表示当前时间
//     return StringCreate(tmpstring, strlen(tmpstring));
// }
// char *string_timestamp()
// {

//     char tmpstring[16] = {0};
//     struct timeval tv;
//     gettimeofday(&tv, NULL);
//     // long result = tv.tv_sec * 1000 + tv.tv_usec / 1000;
//     // printf("second:%ld\n", tv.tv_sec); // 秒
//     // printf("millisecond:%ld\n", tv.tv_sec * 1000 + tv.tv_usec / 1000); // 毫秒
//     // printf("microsecond:%ld\n", tv.tv_sec * 1000000 + tv.tv_usec); // 微秒
//     sprintf(tmpstring, "%ld", tv.tv_sec);
//     return string_create(tmpstring, strlen(tmpstring));
// }

// char *string_nonce(size_t bits)
// {
//     char *deststring = NULL;
//     if (bits >= 2)
//     {
//         char tmpstring[bits];
//         bzero(tmpstring, sizeof(char) * bits);
//         int i, j;
//         unsigned char str[2 + 1] = {0};
//         const char *hex_digits16 = "0123456789ABCDEF";
//         const char *hex_digits8 = "01234567";
//         const char *hex_digits4 = "0123";
//         const char *hex_digits2 = "01";
//         srand(time(NULL));

//         for (j = 0; j < bits / 2; j++)
//         {
//             str[1] = hex_digits16[rand() % 16];
//             str[0] = hex_digits4[rand() % 4];
//             strcat(tmpstring, str);
//         }
//         deststring = StringCreate(tmpstring, strlen(tmpstring));
//     }

//     return deststring;
// }