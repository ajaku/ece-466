#include "sym_tab.h"

/*
    Return Values
    0 if able to create table
    1 if failed to create table
*/
unsigned jacc_hash_table_create(jacc_sym_list_t *list) {
    if (!list) {
        fprintf(stderr, "Passed invalid symbol list\n");
        return 1;
    }

    // List should be of size 0
    if (list->hash_table_size == 0) {

        jacc_sym_t **hash_table = (jacc_sym_t **)calloc(DEFAULT_HASH_SIZE, sizeof(jacc_sym_t *));

        if (!hash_table) {
            fprintf(stderr, "Failed to allocate memory\n");
            return 1;
        }

        list->hash_table = hash_table;
        list->hash_table_size = DEFAULT_HASH_SIZE;
        list->hash_table_capacity = 0;

        return 0;
    }

    fprintf(stderr, "Hash table already exists\n");
    return 1;
}

/*
    Return Values
    > 0 is a valid index
    -1 is invalid
*/
int jacc_hash_function(unsigned hash_table_size, const char *str) {
    if (!str) {
        fprintf(stderr, "Passed invalid string\n");
        return -1;
    }

    if (!hash_table_size) {
        fprintf(stderr, "Hash table not allocated\n");
        return -1;
    }

    size_t size = strlen(str); 

    int hash_val = 0;
    for (unsigned i = 0; i < size; i++) {
        hash_val = 37 * hash_val + str[i]; 
    }

    hash_val %= hash_table_size;

    if (hash_val < 0) hash_val += hash_table_size;

    return hash_val;
}

/*
    Return Values
    0 if table destroyed
    1 if failed to destroy table
*/
unsigned jacc_hash_table_destroy(jacc_sym_list_t *list) {
    if (!list) {
        fprintf(stderr, "Passed invalid list\n");
        return 1;
    }

    unsigned size = list->hash_table_size;

    for (unsigned i = 0; i < size; i++) free(list->hash_table[i]);

    free(list->hash_table);

    list->hash_table_size = 0;
    list->hash_table_capacity = 0;
    
    return 0;
}

/*
    Return Values
    pointer to symbol if successful
    null is unsuccessful
*/
jacc_sym_t *jacc_hash_table_lookup(jacc_sym_list_t *list, const char *str) {
    if (!list) {
        fprintf(stderr, "Passed invalid list\n");
        return NULL;
    }

    if (!str) {
        fprintf(stderr, "Passed invalid string\n");
        return NULL;
    }

    int idx = jacc_hash_function(list->hash_table_size, str);

    if (idx < 0) {
        fprintf(stderr, "Passed invalid string\n");
        return NULL;
    } 

    return list->hash_table[idx];
}

// 0 means match, 1 means no match
static unsigned jacc_cmp_syms(jacc_sym_t *a, jacc_sym_t *b) {

    if (strcmp(a->sym_name, b->sym_name) != 0)              return 1;
    if (strcmp(a->sym_type, b->sym_type) != 0)              return 1;
    if (strcmp(a->sym_file_name, b->sym_file_name) != 0)    return 1;
    if (a->sym_line_num != b->sym_line_num)                 return 1;

    return 0;
}

// Allocate sym before calling function
unsigned jacc_hash_table_enter(jacc_sym_list_t *list, jacc_sym_t *sym, int rep) {
    if (!list) {
        fprintf(stderr, "Passed invalid list\n");
        return 1;
    }

    if (!sym) {
        fprintf(stderr, "Passed invalid symbol\n");
        return 1;
    }

    // Check to see if a rehash is needed (capacity is running out)
    if ((double)list->hash_table_capacity/(double)list->hash_table_size > 0.5) {

        if (!jacc_rehash_table(list)) {
            fprintf(stderr, "Can't insert after a failed rehash\n");
            return 1;
        }
    }

    // Insert item
    int idx = jacc_hash_function(list->hash_table_size, sym->sym_name);

    if (idx < 0) {
        // Error occured, wasn't able to hash item
        fprintf(stderr, "Failed to enter item\n");
        return 1;
    }

    jacc_sym_t **cur_entry = &list->hash_table[idx];

    // Item does not already exist at that index
    if (!(*cur_entry)) {
        *cur_entry = sym;
        list->hash_table_capacity++;
        return 0;
    }

    jacc_sym_t *prev_entry = NULL;
    while (*cur_entry) {

        // Check to see if we've found a duplicate symbol
        if (!jacc_cmp_syms(*cur_entry, sym)) {

            // Don't want to replace the symbol
            if (rep == JACC_HASH_NO_REPLACEMENT) {
                // Found a match, but want to flag as error (rep = 0)
                fprintf(stderr, "Duplicate item, with replacements turned off\n");
                return 1;
            }

            // Want to replace the symbol

            // If there was a previous symbol, update the pointer to be the replaced sym
            if (prev_entry) {
                prev_entry->next_sym = sym;
            }

            // Maintain chain to next sym
            sym->next_sym = (*cur_entry)->next_sym;
            *cur_entry = sym;
            return 0;
        }

        // If not a dupe, continue iterating
        prev_entry = *cur_entry;
        cur_entry = &(*cur_entry)->next_sym;
    }

    // No dupe at this point and iterating through symbols with same hash
    *cur_entry = sym;
    return 0;
}

unsigned jacc_rehash_table(jacc_sym_list_t *list) {
    // in general its best to make hash tables prime number
    // for now, i'll just square the size each time

    unsigned new_size = 2*list->hash_table_size;
    
    // Create a new hash table with more elements
    jacc_sym_t **new_hash_table = (jacc_sym_t **)calloc(new_size, sizeof(jacc_sym_t *));

    if (!new_hash_table) {
        fprintf(stderr, "Failed to allocate memory\n");
        return 1;
    }

    // Save a copy of the old hash table so that we can add them to the new one
    jacc_sym_t **old_hash_table = list->hash_table;
    unsigned old_size = list->hash_table_size;

    // Update the list item to be the new hash table so that I can take advantage of existing insert function
    list->hash_table_size = new_size;
    list->hash_table_capacity = 0;
    list->hash_table = new_hash_table;

    // Now I need to re-insert every item from previous hash table into this new one
    for (unsigned i = 0; i < old_size; i++) {
        jacc_sym_t *entry = old_hash_table[i];
        if (entry != NULL) {

            // Insert entry into new hash table
            // Decided to not allow replacements, shouldn't trigger in this case ever
            if (jacc_hash_table_enter(list, entry, JACC_HASH_NO_REPLACEMENT) == 1) {
                fprintf(stderr, "Rehash failure while inserting old items\n");
                return 1;
            }

            // Insert linear probed entries into new hash table
            while (entry->next_sym != NULL) {
                if (jacc_hash_table_enter(list, entry->next_sym, JACC_HASH_NO_REPLACEMENT)) {
                    fprintf(stderr, "Rehash failure while inserting old items\n");
                    return 1;
                }
                entry = entry->next_sym;
            }
        }
    }
    free(old_hash_table);

    return 0;
}
