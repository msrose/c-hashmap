#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Consider copying the keys and values before storing (and then freeing in
 * mapfree) so that the map doesn't end up holding dangling pointer references
 * if the strings passed in are later freed.
 */

struct Pair {
  char* key;
  char* value;
};

struct PairListNode {
  struct Pair* value;
  struct PairListNode* next;
};

struct iMap {
  struct PairListNode** buckets;
  unsigned long size;
  unsigned long max_size;
  unsigned long bucket_count;
};

typedef struct iMap** Map;

unsigned long hashcode(Map map_ref, char* key) {
  struct iMap* map = *map_ref;

  unsigned long val = 1;

  for (int i = 0; i < strlen(key); i++) {
    val *= key[i];
  }

  return val % map->bucket_count;
}

const int AVERAGE_ITEMS_PER_BUCKET = 2;

Map mapmake() {
  const int STARTING_BUCKET_COUNT = 2;
  const int STARTING_SIZE = STARTING_BUCKET_COUNT * AVERAGE_ITEMS_PER_BUCKET;

  struct iMap* map = malloc(sizeof(struct iMap));
  map->max_size = STARTING_SIZE;
  map->bucket_count = STARTING_BUCKET_COUNT;
  map->buckets = malloc(sizeof(struct PairListNode*) * map->bucket_count);
  map->size = 0;

  for (int i = 0; i < map->bucket_count; i++) {
    map->buckets[i] = NULL;
  }

  Map map_ref = malloc(sizeof(struct iMap*));
  *map_ref = map;
  return map_ref;
}

char* mapget(Map map_ref, char* key) {
  struct iMap* map = *map_ref;

  unsigned long code = hashcode(map_ref, key);

  struct PairListNode* bucket = map->buckets[code];

  while (bucket) {
    if (strcmp(bucket->value->key, key) == 0) {
      return bucket->value->value;
    }
    bucket = bucket->next;
  }

  return NULL;
}

int maphas(Map map_ref, char* key) {
  return mapget(map_ref, key) != NULL;
}

int bucketset(Map map_ref, char* key, char* value) {
  struct iMap* map = *map_ref;

  unsigned long code = hashcode(map_ref, key);

  struct PairListNode* bucket = map->buckets[code];

  if (bucket == NULL) {
    struct Pair* pair = malloc(sizeof(struct Pair));
    pair->key = key;
    pair->value = value;

    struct PairListNode* node = malloc(sizeof(struct PairListNode));
    node->value = pair;
    node->next = NULL;

    map->buckets[code] = node;
    map->size++;
    return 0;
  }

  while (1) {
    if (strcmp(bucket->value->key, key) == 0) {
      bucket->value->value = value;
      return 1;
    }

    if (bucket->next == NULL) {
      struct Pair* pair = malloc(sizeof(struct Pair));
      pair->key = key;
      pair->value = value;

      struct PairListNode* node = malloc(sizeof(struct PairListNode));
      node->value = pair;
      node->next = NULL;

      bucket->next = node;
      map->size++;
      return 0;
    }

    bucket = bucket->next;
  }
}

void mapset(Map map_ref, char* key, char* value) {
  if (!maphas(map_ref, key) && value == NULL) {
    return;
  }

  if (bucketset(map_ref, key, value)) {
    return;
  }

  struct iMap* map = *map_ref;

  if (map->size == map->max_size) {
    struct iMap* new_map = malloc(sizeof(struct iMap));
    new_map->bucket_count = map->bucket_count * 2;
    new_map->max_size = new_map->bucket_count * AVERAGE_ITEMS_PER_BUCKET;
    new_map->buckets = malloc(sizeof(struct PairListNode*) * new_map->bucket_count);
    new_map->size = 0;
    for (int i = 0; i < map->bucket_count; i++) {
      new_map->buckets[i] = NULL;
    }

    for (int i = 0; i < map->bucket_count; i++) {
      struct PairListNode* bucket = map->buckets[i];
      while (bucket) {
        bucketset(&new_map, bucket->value->key, bucket->value->value);
        free(bucket->value);
        struct PairListNode* to_free = bucket;
        bucket = bucket->next;
        free(to_free);
      }
    }
    free(map->buckets);
    free(map);

    *map_ref = new_map;
  }
}

void mapdel(Map map_ref, char* key) {
  struct iMap* map = *map_ref;

  unsigned long code = hashcode(map_ref, key);

  struct PairListNode* prev = NULL;
  struct PairListNode* node = map->buckets[code];

  while (node) {
    if (strcmp(node->value->key, key) == 0) {
      if (prev) {
        prev->next = node->next;
      } else {
        map->buckets[code] = NULL;
      }
      free(node->value);
      free(node);
      map->size--;
      return;
    }
    prev = node;
    node = node->next;
  }
}

void mapfree(Map map_ref) {
  struct iMap* map = *map_ref;

  for (int i = 0; i < map->bucket_count; i++) {
    struct PairListNode* bucket = map->buckets[i];
    while (bucket) {
      free(bucket->value);
      struct PairListNode* to_free = bucket;
      bucket = bucket->next;
      free(to_free);
    }
  }
  free(map->buckets);
  free(map);
  free(map_ref);
}

void mapprint(Map map_ref) {
  struct iMap* map = *map_ref;

  printf("map is size %lu, max size is %lu\n", map->size, map->max_size);
  for (int i = 0; i < map->bucket_count; i++) {
    struct PairListNode* bucket = map->buckets[i];

    printf("%d [", i);
    while (bucket) {
      printf("%s:%s", bucket->value->key, bucket->value->value);
      bucket = bucket->next;
      if (bucket) {
        printf(", ");
      }
    }
    printf("]\n");
  }

  printf("\n");
}
