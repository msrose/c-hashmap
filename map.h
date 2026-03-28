#ifndef MAP
#define MAP

struct _iMap;

typedef struct _iMap** Map;

Map mapmake();
void mapset(Map map_ref, char* key, char* value);
char* mapget(Map map_ref, char* key);
int maphas(Map map_ref, char* key);
void mapdel(Map map_ref, char* key);
void mapfree(Map map_ref);
void mapprint(Map map_ref);

#endif
