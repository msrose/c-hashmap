#include <stdio.h>

#include "./map.c"

int main() {
  Map map = mapmake();

  mapset(map, "hey", "donkey");

  mapprint(map);

  mapset(map, "foo", "llama");
  mapset(map, "bar", "alpaca");
  mapset(map, "baz", "frog");
  mapprint(map);
  mapset(map, "chloe", "antelope");
  mapset(map, "michael", "spleen");
  mapset(map, "michael", "maggot");
  mapprint(map);
  mapset(map, "baz", "zulu");
  mapset(map, "a", "b");
  mapset(map, "c", "d");
  mapprint(map);
  mapset(map, "e", "f");
  mapset(map, "g", "h");
  mapset(map, "i", "j");
  mapprint(map);

  printf(
    "Map vals are hey:%s, foo:%s, bar:%s, baz:%s, chloe:%s, michael:%s\n",
    mapget(map, "hey"),
    mapget(map, "foo"),
    mapget(map, "bar"),
    mapget(map, "baz"),
    mapget(map, "chloe"),
    mapget(map, "michael")
  );

  printf("Map should have i: %d\n", maphas(map, "i"));

  mapdel(map, "i");
  mapdel(map, "c");

  mapprint(map);

  printf("Map should not have i: %d\n", maphas(map, "i"));

  printf("Map should never have SpanishGuy: %d\n", maphas(map, "SpanishGuy"));

  mapfree(map);
}
