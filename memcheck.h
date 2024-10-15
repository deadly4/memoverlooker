#pragma once

#include <stdint.h>
#include <stdlib.h>

#include "hashtable.h"

typedef struct procedure_statistics {
    const char* name;
    size_t allocated_mem;
    size_t free_mem;
} procedure_statistics;

typedef struct addr_desc {
    void* addr;
    const char* proc_name;
} addr_desc;

typedef struct addr_list {
    addr_desc addr;
    struct addr_list* next;
} addr_list;

void  memcheck(void);
void  memcheck_destroy(void);
void* memcheck_malloc(size_t nbytes, const char* proc_key);
void  memcheck_free(void* ptr, const char* proc_key);
void  memcheck_report(void);