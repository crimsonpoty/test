#include "properties.h"
#include "hashmap.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "ioutil.h"
#include "stringtokenizer.h"
#include "linkedlist.h"
#include <stdio.h>

/* ---------------------------------------------
 * author: smkim, buttonfly@gmail.com *
 * release date: 2008.4.14
 * --------------------------------------------*/

typedef struct
{
    hashmap                table;
    linkedlist            keyset;

} properties_t;


typedef struct
{
	char* key;
	char* value;

} kv_pair_t;


static int properties_parse(properties_t* prop, const char* buffer)
{
    const static char* __DELIM__ = "=\n";

    stringtokenizer        tokenizer;
    unsigned long crc32;
    char temp[512];
    char key[512];
    char value[512];
    int ret;
    int i;
#if unused
    int pair_size;
#endif

    memset(temp, '\0', sizeof(temp));
    tokenizer = stringtokenizer_new(&buffer);
    for(i = 0;(ret = stringtokenizer_next_token(tokenizer, __DELIM__, 2, temp)) != -1; i++)
    {
        if((i % 2) == 0)
        {
            memset(key, '\0', sizeof(key));
            strcpy(key, temp);
        }
        else
        {
        	memset(value, '\0', sizeof(value));
            crc32 = ioutil_get_crc32(key, strlen(key));
            properties_put(prop, key, value);
        }
    }
    stringtokenizer_delete(tokenizer);
    return 0;
}


properties properties_new(void)
{
    properties_t* prop;
    int size;

    size = sizeof(properties_t);

    prop = (properties_t*) malloc(size);
    memset(prop, 0, size);
    prop->table = hashmap_new(10);
    prop->keyset = linkedlist_new();//
    return prop;
}


int properties_load(properties prop, const char* path)
{
    char* buffer;

    buffer = ioutil_read_file(path, NULL);
    if(buffer == NULL)
    {
        return -1;
    }

    properties_parse(prop, buffer);

    free(buffer);
    return 0;
}

//TODO: not implemented yet
int properties_store(properties prop, const char* path)
{
	int fd = open(path, O_RDWR | O_CREAT,   S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH);
	if(fd == -1)
	{
		fprintf(stderr, "failed to open %s\n", path);
		return -1;
	}

	int size = properties_get_size(prop);
	int i;
	for(i = 0; i < size; i++)
	{
		const char* key = properties_get_key(prop, i);
		const char* value = properties_get(prop, key);
		if(key != NULL)
		{
			write(fd, key, strlen(key));
			write(fd, "=", strlen("="));
			write(fd, value, strlen(value));
			write(fd, "\n", strlen("\n"));
		}
	}
	close(fd);
    return 0;
}


static void hashmap_fandel_callback(void* data)
{
	kv_pair_t* p = (kv_pair_t*) data;
	if(p != NULL)
	{
		if(p->key != NULL) {
			free(p->key);
			p->key = NULL;
		}

		if(p->value != NULL) {
			free(p->value);
			p->value = NULL;
		}
	}
    free(data);
}


void properties_delete(properties prop)
{
    properties_t*    p;
    p = (properties_t*) prop;

    hashmap_fandel(p->table, hashmap_fandel_callback);
    linkedlist_delete(p->keyset, NULL);
}


int properties_put(properties prop, const char* key, const char* value)
{
    properties_t* p;
    unsigned long crc32;
    int pair_size;
    kv_pair_t* pair;

    p = (properties_t*) prop;
    crc32 = hash((char*)key, strlen(key));

    pair_size = sizeof(kv_pair_t);

    pair = (kv_pair_t*) malloc(pair_size);
    memset(pair, 0, pair_size);
    pair->key = strdup(key);
    pair->value = strdup(value);
    hashmap_insert(p->table, pair, crc32);
    linkedlist_add(p->keyset, (void*)crc32);
    return 0;
}


const char* properties_get(properties prop, const char* key)
{
    properties_t* p;
    unsigned long crc32;
    kv_pair_t*    pair;

    p = (properties_t*) prop;
    crc32 = ioutil_get_crc32((char*)key, strlen(key));
    pair = (kv_pair_t*) hashmap_get(p->table, crc32);
    if(pair == NULL)
    {
        return NULL;
    }
    return pair->value;
}


unsigned long properties_get_keycode(properties prop, int index)
{
    properties_t* p;
    p = (properties_t*) prop;
    unsigned long key;
    key = (unsigned long)linkedlist_get(p->keyset, index);

    return key;
}


const char* properties_get_by_keycode(properties prop, unsigned int keycode)
{
    properties_t* p;
    kv_pair_t*    pair;
    p = (properties_t*) prop;
    pair = (kv_pair_t*) hashmap_get(p->table, keycode);
    if(pair == NULL)
    {
        return NULL;
    }
    return pair->value;
}


int properties_get_size(properties prop)
{
    properties_t* p;
    p = (properties_t*) prop;
    return hashmap_count(p->table);
}


const char* properties_get_key(properties prop, int index)
{
    properties_t* p;
    p = (properties_t*) prop;
    unsigned long key;
    key = (unsigned long)linkedlist_get(p->keyset, index);
    kv_pair_t* pair;
    pair = hashmap_get(p->table, key);
    return pair->key;
}


int properties_remove(properties prop, const char* key)
{
    properties_t* p;
    unsigned long crc32;
    kv_pair_t*    pair;

    p = (properties_t*) prop;
    crc32 = ioutil_get_crc32((char*)key, strlen(key));
    pair = hashmap_remove(p->table, crc32);
    if(pair == NULL)
    {
        return -1;
    }
    linkedlist_remove(p->keyset, (void*)crc32);
    if(pair->key != NULL)
    {
    	free(pair->key);
    	pair->key = NULL;
    }

    if(pair->value != NULL)
    {
    	free(pair->value);
    	pair->value = NULL;
    }
    free(pair);
    return 0;
}

