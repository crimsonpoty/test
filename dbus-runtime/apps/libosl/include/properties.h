
#ifndef PROPERTIES_H_
#define PROPERTIES_H_

/* ---------------------------------------------
 * author: smkim, buttonfly@gmail.com *
 * release date: 2008.4.14
 * --------------------------------------------*/


//#define HASHMAP_INIT_SIZE    10
//#define MAX_KEY_LEN            512
//#define MAX_VALUE_LEN        512

#ifdef __cplusplus
extern "C"
{
#endif


typedef void* properties;

/**
 * create an empty property list with no default values
 * @return this properties
 */
extern properties properties_new(void);

/**
 * read the key-value lists from the specified file path
 * @param prop this properties
 * @param path file path to load
 */
extern int properties_load(properties prop, const char* path);

/**
 * not implemented yet
 * write the current property list to the specified file path
 * @return
 */
extern int properties_store(properties prop, const char* path);

/**
 * calls the hashmap_w function hashmap_insert
 * @param prop this property list
 * @param key key
 * @param data value
 * @return
 */
extern int properties_put(properties prop, const char* key, const char* data);

/**
 * searches for the property with the specified key in this property list
 * @param prop this property list
 * @param key key
 * @return string value which is associated with the key
 */
extern const char* properties_get(properties prop, const char* key);

/**
 * call the hashmap_w function hashmap_remove
 * @param prop this property list
 * @param key key
 * @return
 */
extern int properties_remove(properties prop, const char* key);

/**
 * returns the number of this property list
 * @param prop this property list
 * @return the number of this property list
 */
extern int properties_get_size(properties prop);

/**
 * returns the element at the specified position in this property list
 * @param prop this property list
 * @param index index of element to return
 * @return the element at the specified position in this list. it returns null unless present
 */
extern const char* properties_get_key(properties prop, int index);

/**
 * property list use the crc32 value as a keycode internally
 * @param prop this property list
 * @param index index of element to return
 * @return the keycode at the specified position in this list.
 */
extern unsigned long properties_get_keycode(properties prop, int index);

/**
 * returns string value which is associated with the specified keycode
 * @param prop this property list
 * @param keycode
 * @return string value which is associated with the specified keycode
 */
extern const char* properties_get_by_keycode(properties prop, unsigned int keycode);

/**
 * close this property list
 * @param prop this property list to close
 */
extern void properties_delete(properties prop);

#ifdef __cplusplus
}
#endif


#endif /*PROPERTIES_H_*/

