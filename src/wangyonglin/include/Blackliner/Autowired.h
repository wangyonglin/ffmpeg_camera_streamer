/*
typedef struct main
  {
    char *TopicFullName;
    char *DeviceName;
    long timeout_in_secs;
    Boolean display_errors;
  } AliyunConfig;

  AutowiredCommand commands[] = {
      AUTOWIRED_COMMAND_STRING("TopicFullName", offsetof(AliyunConfig, TopicFullName)),
      AUTOWIRED_COMMAND_STRING("DeviceName", offsetof(AliyunConfig, DeviceName)),
      AUTOWIRED_COMMAND_STRING("TopicFullName", offsetof(AliyunConfig, TopicFullName)),
      AUTOWIRED_COMMAND_INTEGER("timeout_in_secs", offsetof(AliyunConfig, timeout_in_secs)),
      AUTOWIRED_COMMAND_BOOLEAN("display_errors", offsetof(AliyunConfig, display_errors)),
      AUTOWIRED_COMMAND_NULL};
  Autowired *ini = AutowiredCreate(inif);

  AliyunConfig *config = NULL;
  SkeletonAllocate((void**)&config, sizeof(AliyunConfig));
  AutowiredLoader(ini, commands, config, "ALIIOT");

  fprintf(stdout, "\t[%s=>%s]\r\n", "TopicFullName", config->TopicFullName);
  fprintf(stderr, "\t[%s=>%ld]\r\n", "timeout_in_secs", config->timeout_in_secs);
  fprintf(stderr, "\t[%s=>%d]\r\n", "display_errors", config->display_errors);
  SkeletonDeallocate(config);
  AutowiredDelete(ini);
  */
#if !defined(INCLUDE_BLACKLINER_AUTOWIRED_H)
#define INCLUDE_BLACKLINER_AUTOWIRED_H
#include <Blackliner/Skeletons.h>
#include <Blackliner/String.h>


typedef struct Autowired
{
    CONF *conf;
    BIO *bio;
    long lELine;
    char *filename;
} Autowired;

    
// #define AUTOWIRED_COMMAND_FILE(data, addr) \
//     {                                   \
//         data, SKELETON_TYPE_FILE, addr    \
//     }
#define AUTOWIRED_COMMAND_STRING(data, addr) \
    {                                     \
        data, SKELETON_TYPE_STRING, addr    \
    }
#define AUTOWIRED_COMMAND_INTEGER(data, addr) \
    {                                      \
        data, SKELETON_TYPE_INTEGER, addr    \
    }
#define AUTOWIRED_COMMAND_BOOLEAN(data, addr) \
    {                                      \
        data, SKELETON_TYPE_BOOLEAN, addr    \
    }
#define AUTOWIRED_COMMAND_NULL          \
    {                                \
        NULL, SKELETON_TYPE_INVALID, 0 \
    }
typedef struct AutowiredCommand
{
    const char *name;
    Skeletons skell;
    size_t addr;
} AutowiredCommand;

Autowired *AutowiredCreate(char *filename);
void AutowiredDelete(Autowired *autowired);

void AutowiredLoader(Autowired *autowired, AutowiredCommand command[], void *skeleton, const char *section);
#endif