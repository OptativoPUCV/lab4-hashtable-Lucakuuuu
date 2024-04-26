#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
    char *ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) 
{
  if(map == NULL) return;
  //if(map->size == map->capacity enlarge(map);
  long posicion = hash(key,map->capacity);
  while(map->buckets[posicion]!=NULL && map->buckets[posicion]->key!=NULL)
    {
      if(is_equal(map->buckets[posicion]->key, key) == 1)
        map->buckets[posicion]->value=value;
      posicion++;
      posicion = posicion%map->capacity;
    }
  map->buckets[posicion] = createPair(key,value);
  map->size++;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)


}


HashMap * createMap(long capacity) 
{
  HashMap *mapa = (HashMap *)malloc(sizeof(HashMap));
  mapa->buckets = (Pair **) calloc(capacity, sizeof(Pair *));
  mapa->size = 0;
  mapa->capacity = capacity;
  mapa->current = -1;
  return mapa;
}

void eraseMap(HashMap * map,  char * key) 
{
  if(map == NULL || map->buckets == NULL) return;
  long posicion = hash(key,map->capacity);
  while(map->buckets[posicion]!=NULL && map->buckets[posicion]->key!=NULL)
    {
      if(is_equal(map->buckets[posicion]->key, key) == 1)
      {
        map->buckets[posicion]->key = NULL;
        map->buckets[posicion]->value = NULL;
        map->size--;
      }
      else
      {
        posicion++;
        posicion = posicion % map->capacity;
      }
    }
}

Pair * searchMap(HashMap * map,  char * key) 
{
  if(map == NULL) return NULL;
  long posicion = hash(key,map->capacity);
  while(map->buckets[posicion]!=NULL && map->buckets[posicion]->key!=NULL)
    {
      if(is_equal(map->buckets[posicion]->key, key) == 1)
      {
        map->current = posicion;
        return map->buckets[posicion];
      }
      else
      {
      posicion++;
      posicion = posicion % map->capacity;
      }
    }
  return NULL;
}

Pair * firstMap(HashMap * map) 
{
  if(map == NULL || map->buckets == NULL) return NULL;
  while(map->current != -1)
    {
      if(map->buckets[map->current]!=NULL && map->buckets[map->current]->key!=NULL)
        return map->buckets[map->current];
      else
      {
        map->current++;
        map->current = map->current % map->capacity;
      }
    }
  return NULL;
}

Pair * nextMap(HashMap * map) 
{
  if (map == NULL || map->buckets == NULL) return NULL;

  long nextIndex = map->current + 1;
  while (nextIndex != map->current) {
      if (nextIndex >= map->capacity)
          nextIndex = 0;

      if (map->buckets[nextIndex] != NULL && map->buckets[nextIndex]->key != NULL) {
          map->current = nextIndex;
          return map->buckets[nextIndex];
      }

      nextIndex++;
  }

  return NULL;

