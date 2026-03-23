#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct PairListNode {
  char* key;
  char* value;
  struct PairListNode* next;
};

typedef struct {
  struct PairListNode** buckets;
  unsigned long size;
  unsigned long max_size;
  unsigned long bucket_count;
} iMap;

typedef iMap** Map;

const int AVERAGE_ITEMS_PER_BUCKET = 2;
const int GROWTH_FACTOR = 2;
const int STARTING_BUCKET_COUNT = 2;
const int STARTING_SIZE = STARTING_BUCKET_COUNT * AVERAGE_ITEMS_PER_BUCKET;

// djb2
unsigned long hashcode(char* key) {
  unsigned long val = 5381;
  size_t len = strlen(key);
  for (int i = 0; i < len; i++) {
    val = val * 33 + key[i];
  }
  return val;
}

unsigned long mapbucket(Map map_ref, char* key) {
  iMap* map = *map_ref;

  unsigned long code = hashcode(key);
  return code % map->bucket_count;
}

Map mapmake() {
  iMap* map = malloc(sizeof(iMap));
  *map = (iMap) {
    .max_size = STARTING_SIZE,
    .bucket_count = STARTING_BUCKET_COUNT,
    .buckets = malloc(sizeof(struct PairListNode*) * STARTING_BUCKET_COUNT),
    .size = 0
  };

  for (int i = 0; i < map->bucket_count; i++) {
    map->buckets[i] = NULL;
  }

  Map map_ref = malloc(sizeof(iMap*));
  *map_ref = map;
  return map_ref;
}

char* mapget(Map map_ref, char* key) {
  iMap* map = *map_ref;

  unsigned long code = mapbucket(map_ref, key);

  struct PairListNode* node = map->buckets[code];

  while (node) {
    if (strcmp(node->key, key) == 0) {
      return node->value;
    }
    node = node->next;
  }

  return NULL;
}

int maphas(Map map_ref, char* key) {
  return mapget(map_ref, key) != NULL;
}

struct PairListNode* makenode(char* key, char* value) {
  struct PairListNode* new_node = malloc(sizeof(struct PairListNode));
  *new_node = (struct PairListNode) {
    .key = strdup(key),
    .value = strdup(value),
    .next = NULL
  };

  return new_node;
}

int bucketset(Map map_ref, char* key, char* value) {
  iMap* map = *map_ref;

  unsigned long code = mapbucket(map_ref, key);

  struct PairListNode* node = map->buckets[code];

  if (node == NULL) {
    map->buckets[code] = makenode(key, value);
    map->size++;
    return 0;
  }

  while (1) {
    if (strcmp(node->key, key) == 0) {
      free(node->value);
      unsigned int value_size = sizeof(char) * (strlen(value) + 1);
      node->value = malloc(value_size);
      strlcpy(node->value, value, value_size);
      return 1;
    }

    if (node->next == NULL) {
      node->next = makenode(key, value);
      map->size++;
      return 0;
    }

    node = node->next;
  }
}

void mapdel(Map map_ref, char* key) {
  iMap* map = *map_ref;

  unsigned long code = mapbucket(map_ref, key);

  struct PairListNode* prev = NULL;
  struct PairListNode* node = map->buckets[code];

  while (node) {
    if (strcmp(node->key, key) == 0) {
      if (prev) {
        prev->next = node->next;
      } else {
        map->buckets[code] = node->next;
      }
      free(node->key);
      free(node->value);
      free(node);
      map->size--;
      return;
    }
    prev = node;
    node = node->next;
  }
}

void mapset(Map map_ref, char* key, char* value) {
  if (value == NULL) {
    if (maphas(map_ref, key)) {
      mapdel(map_ref, key);
    }
    return;
  }

  if (bucketset(map_ref, key, value)) {
    return;
  }

  iMap* map = *map_ref;

  if (map->size == map->max_size) {
    iMap* new_map = malloc(sizeof(iMap));
    // Designated initializer self-reference doesn't work with `leaks` CLI
    unsigned int new_bucket_count = map->bucket_count * GROWTH_FACTOR;
    *new_map = (iMap) {
      .bucket_count = new_bucket_count,
      .max_size = new_map->bucket_count * AVERAGE_ITEMS_PER_BUCKET,
      .buckets = malloc(sizeof(struct PairListNode*) * new_bucket_count),
      .size = 0
    };
    for (int i = 0; i < new_map->bucket_count; i++) {
      new_map->buckets[i] = NULL;
    }

    for (int i = 0; i < map->bucket_count; i++) {
      struct PairListNode* node = map->buckets[i];
      while (node) {
        bucketset(&new_map, node->key, node->value);
        free(node->key);
        free(node->value);
        struct PairListNode* to_free = node;
        node = node->next;
        free(to_free);
      }
    }
    free(map->buckets);
    free(map);

    *map_ref = new_map;
  }
}

void mapfree(Map map_ref) {
  iMap* map = *map_ref;

  for (int i = 0; i < map->bucket_count; i++) {
    struct PairListNode* node = map->buckets[i];
    while (node) {
      free(node->key);
      free(node->value);
      struct PairListNode* to_free = node;
      node = node->next;
      free(to_free);
    }
  }
  free(map->buckets);
  free(map);
  free(map_ref);
}

void mapprint(Map map_ref) {
  iMap* map = *map_ref;

  printf("map is size %lu, max size is %lu\n", map->size, map->max_size);
  for (int i = 0; i < map->bucket_count; i++) {
    struct PairListNode* node = map->buckets[i];

    printf("%d [", i);
    while (node) {
      printf("%s:%s", node->key, node->value);
      node = node->next;
      if (node) {
        printf(", ");
      }
    }
    printf("]\n");
  }

  printf("\n");
}
