
#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif
#include <stdio.h>
#include "dlibs.h"


DLibLoader create_lib_loader() {
    DLibLoader l;
    l.loaded = create_vector(sizeof(void*));
    return l;
}

void dlibs_load(DLibLoader* l, char* file) {
    void* lib;
    #ifdef _WIN32
        lib = LoadLibrary(file);
    #else
        lib = dlopen(file, RTLD_LAZY);
    #endif
    if(lib == NULL) {
        printf("Unable to load dynamic library '%s'!\n", file);
        exit(1);
    }
    vector_push(&l->loaded, &lib);
}

void* dlibs_find(DLibLoader* l, char* name) {
    for(size_t lib_idx = 0; lib_idx < l->loaded.size; lib_idx += 1) {
        void** lib = vector_get(&l->loaded, lib_idx);
        void* found;
        #ifdef _WIN32
            found = GetProcAddress(*lib, name);
        #else
            found = dlsym(*lib, name);
        #endif
        if(found != NULL) {
            return found;
        }
    }
    return NULL;
}

void dlibs_free(DLibLoader* l) {
    for(size_t lib_idx = 0; lib_idx < l->loaded.size; lib_idx += 1) {
        void** lib = vector_get(&l->loaded, lib_idx);
        #ifdef _WIN32
            FreeLibrary(*lib);
        #else
            dlclose(*lib);
        #endif
    }
}