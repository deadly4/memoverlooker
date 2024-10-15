#include "memcheck.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

static ht* proc_ht = NULL;
static addr_list* addr_l = NULL;

void exit_nomem(void) {
    fprintf(stderr, "out of memory\n");
    exit(1);
}

static void init_proc_ht(void) 
{
    proc_ht = ht_create();
    addr_l  = malloc(sizeof(addr_list));

    if (proc_ht == NULL) {
        exit_nomem();
    }
}

static void insert_addr_list(addr_list **l, addr_desc addr)
{
    addr_list* p;
    p = malloc(sizeof(addr_list));
    p->addr = addr;
    p->next = *l;
    *l = p;
}

static addr_list* search_addr_list(addr_list* l, addr_desc addr)
{
    if (l == NULL) return(NULL);
    if ((l->addr.addr == addr.addr))
        return(l);
    else
        return (search_addr_list(l->next, addr));
}

static procedure_statistics* init_ps(const char* name)
{
    procedure_statistics* ps = malloc(sizeof(procedure_statistics));

    ps->name = name;
    ps->allocated_mem = 0;
    ps->free_mem = 0;

    return ps;
}

static procedure_statistics* memcheck_refresh_proc_ht(const char* proc_key)
{
    procedure_statistics* set_ps = init_ps(proc_key);

    if (ht_set(proc_ht, set_ps->name, set_ps) == NULL) 
    {
        exit_nomem();
    }

    return set_ps;
}

static void iter_proc_ht(bool stat, bool destroy)
{
    hti proc_it = ht_iterator(proc_ht);

    while (ht_next(&proc_it)) {
        procedure_statistics* ps = (procedure_statistics*)proc_it.value;

        if (stat)
        {
            printf("============MEMCHECK DIAGNOSTIC REPORT============\n");
            printf("procedure_name: %s\n", ps->name);
            printf("--------------------------------------------------\n");
            printf("allocated_mem: %ld\n", ps->allocated_mem);
            printf("--------------------------------------------------\n");
            printf("free_mem: %ld\n\n", ps->free_mem);
        }

        if (destroy)
            free(proc_it.value);
    }

    if (destroy)
        ht_destroy(proc_ht);  
}

static void free_linked_list(void)
{
    addr_list* block;

    while (addr_l != NULL)
    {
        block = addr_l;
        addr_l = addr_l->next;
        free(block);
    }

    free(addr_l);
}

void memcheck(void)
{
    if (proc_ht == NULL)
        init_proc_ht();
}

void memcheck_destroy(void)
{
    iter_proc_ht(false, true);
    free_linked_list();
}

void* memcheck_malloc(size_t nbytes, const char* proc_key) 
{
    procedure_statistics* get_ps = ht_get(proc_ht, proc_key);
    addr_desc ad;

    if (get_ps == NULL)
        get_ps = memcheck_refresh_proc_ht(proc_key);

    void* ptr = malloc(nbytes);

    get_ps->allocated_mem += nbytes;

    ad.addr = ptr;
    ad.proc_name = proc_key;

    insert_addr_list(&addr_l, ad);

    return ptr;
}

void memcheck_free(void* ptr, const char* proc_key)
{
    procedure_statistics* get_ps = ht_get(proc_ht, proc_key);
    addr_desc ad;

    if (get_ps == NULL)
        get_ps = memcheck_refresh_proc_ht(proc_key);

    ad.addr = ptr;
    ad.proc_name = proc_key;

    addr_list* al = search_addr_list(addr_l, ad);
    procedure_statistics* search_ps = ht_get(proc_ht, al->addr.proc_name);
    get_ps->free_mem += search_ps->allocated_mem;

    free(ptr);
}

void memcheck_report(void)
{
    iter_proc_ht(true, false);
}