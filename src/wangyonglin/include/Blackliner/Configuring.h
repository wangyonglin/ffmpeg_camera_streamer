/*
  Configuring *config = NULL;
  ConfiguringInit(&config, inif);
  ConfiguringString(config, &_logger->simple, "log", "simple");
  ConfiguringDestroy(config);
*/
#if !defined(INCLUDE_BLACKLINER_CONFIGURING_H)
#define INCLUDE_BLACKLINER_CONFIGURING_H
#include <Blackliner/Skeletons.h>
#include <Blackliner/String.h>

typedef struct Configuring
{
  CONF *conf;
  BIO *bio;
  long lELine;
  char *filename;
} Configuring;

Configuring *ConfiguringInit(Configuring **config, char *filename);
void ConfiguringDestroy(Configuring *config);
ok_t ConfiguringString(Configuring *config, String *skeleton, const char *section, const char *name);
ok_t ConfiguringInteger(Configuring *config, Integer *skeleton, const char *section, const char *name);
ok_t ConfiguringBoolean(Configuring *config, Boolean *skeleton, const char *section, const char *name);
#endif