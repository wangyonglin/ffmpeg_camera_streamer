#include <Blackliner/KVTailq.h>

KVTailq *KVTailqCreate()
{
    KVTailq *kvTailq = malloc(sizeof(KVTailq));
    if (kvTailq)
    {

        TAILQ_INIT(&kvTailq->head);
        return kvTailq;
    }

    return NULL;
}

void KVTailqDestroy(KVTailq *kvTailq)
{
    if (kvTailq)
    {
        while (!TAILQ_EMPTY(&kvTailq->head))
        {
            struct __kv_tailq_entry_t *item = TAILQ_FIRST(&kvTailq->head);
            if (item)
            {
                TAILQ_REMOVE(&kvTailq->head, item, entries);
                free(item);
            }
        }
        free(kvTailq);
    }
}
void KVTailqAdd(KVTailq *kvTailq, const char *key, char *value)
{
    if (kvTailq)
    {
        struct __kv_tailq_entry_t *item = malloc(sizeof(struct __kv_tailq_entry_t));
        if (item)
        {
            item->key = strdup(key);
            item->value = value;
            TAILQ_INSERT_TAIL(&kvTailq->head, item, entries);
        }
    }
}