/*
 * 版权属于：yitter(yitter@126.com)
 * 开源地址：https://github.com/yitter/idgenerator
 */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <SnowFlake/SnowFlake.h>
#include <SnowFlake/IdGenerator.h>
extern void SetIdGenerator(IdGeneratorOptions options)
{
    SetOptions(options);
}

extern void SetWorkerId(uint32_t workerId)
{
    IdGeneratorOptions options = BuildIdGenOptions(workerId);
    SetIdGenerator(options);
}

extern int64_t NextId()
{
    return GetIdGenInstance()->NextId();
    //    IdGenerator *generator = GetIdGenInstance();
    //    uint64_t id = generator->NextId();
    //    free(generator);
    //    return id;
}

void SnowFlakeInit(uint32_t workerId, uint8_t method, uint8_t SeqBitLength)
{

    IdGeneratorOptions options = BuildIdGenOptions(1);
    options.Method = method;
    options.WorkerId = workerId;
    options.SeqBitLength = 10;
    SetIdGenerator(options);
}

int64_t SnowFlake_IdGenerator()
{
    return GetIdGenInstance()->NextId();
    //    IdGenerator *generator = GetIdGenInstance();
    //    uint64_t id = generator->NextId();
    //    free(generator);
    //    return id;
}
char *SnowFlake_build()
{
    char *tmpstring = malloc(32);
    if (tmpstring)
    {
        memset(tmpstring, 0x00, 32);
        int64_t result = GetIdGenInstance()->NextId();
        sprintf(tmpstring, "%ld", result);
    }
    return tmpstring;
}
int64_t SnowFlakeFormat(char **pointer)
{
    int64_t id = GetIdGenInstance()->NextId();
    if ((*pointer) = malloc(sizeof(char) * 12))
    {
        memset((*pointer), 0x00, sizeof(char) * 12);
        sprintf((*pointer), "%ld", id);
    }
    return id;
}