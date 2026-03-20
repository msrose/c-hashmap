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
  mapset(map, "k", "l");
  mapset(map, "m", "n");
  mapset(map, "o", "p");
  mapset(map, "q", "s");
  mapset(map, "t", "u");
  mapset(map, "v", "w");
  mapset(map, "x", "y");
  mapset(map, "z", "1");
  mapset(map, "2", "3");
  mapset(map, "4", "5");
  mapset(map, "6", "7");
  mapset(map, "8", "9");
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

  printf("Map should have i: %d\n\n", maphas(map, "i"));

  mapdel(map, "i");
  mapdel(map, "g");
  mapset(map, "c", NULL);

  mapprint(map);

  printf("Map should not have i: %d\n", maphas(map, "i"));

  printf("Map should never have SpanishGuy: %d\n", maphas(map, "SpanishGuy"));

  mapfree(map);
}
