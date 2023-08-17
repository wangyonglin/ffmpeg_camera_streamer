/*  KVTailq *tailq = KVTailqCreate();
  if (!tailq)
    return __err_fail;
  KVTailqAdd(tailq, "name", "wangyonglin");

  struct __kv_tailq_entry_t *current_item;
  TAILQ_FOREACH(current_item, &tailq->head, entries)
  {
    fprintf(stdout, "\t%s=>%s\r", current_item->key, current_item->value);
  }
  fprintf(stdout, "\n");
  KVTailqDestroy(tailq);*/
#if !defined(INCLUDE_UWANG_KVTAILQ_H)
#define INCLUDE_UWANG_KVTAILQ_H
#include <Helpers/String.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>

struct __kv_tailq_entry_t
{
    char *key;
    char *value;
    TAILQ_ENTRY(__kv_tailq_entry_t)
    entries;
} KVTailqEntry;


typedef struct __kv_tailq_head_t
{
    TAILQ_HEAD(head, __kv_tailq_entry_t)
    head;
} KVTailq, *pKVTailq;

KVTailq *KVTailqCreate();
void KVTailqDestroy(KVTailq *kvTailq);
void KVTailqAdd(KVTailq *kvTailq, const char *key, char *value);
#endif