#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "vananagui/platform/BinaryResource.h"
#include "vananagui/base.h"



extern void* hInstance;



BinaryResource::~BinaryResource() {
    // SAFE_DELETE_PTR(this->data);
}

BinaryResource* BinaryResource::Load(const char* id, const char* type) {

    // find resource location
    HRSRC resInfo = FindResource(reinterpret_cast<HMODULE>(hInstance), id, type);
    if (!resInfo) {
        #ifdef DEBUG
        printf("Binary resource %s of type %s not found.\n", id, type);
        #endif
        return nullptr;
    }

    // load resource into global memory
    HGLOBAL resLoaded = LoadResource(reinterpret_cast<HMODULE>(hInstance), resInfo);
    if (!resLoaded) {
        #ifdef DEBUG
        printf("Binary resource %s of type %s was found, but not loaded.\n", id, type);
        #endif
        return nullptr;
    }

    // Read into binary resource struct
    BinaryResource* binaryResource = static_cast<BinaryResource*>(malloc(sizeof(BinaryResource)));
    binaryResource->data = LockResource(resLoaded);
    binaryResource->size = SizeofResource(reinterpret_cast<HMODULE>(hInstance), resInfo);

    // #ifdef DEBUG
    // printf("Binary resource %s of type %s loaded:\nData ptr: %p\nData size: %lu\n",
    //     id, type, binaryResource->data, binaryResource->size);
    // #endif

    return binaryResource;
}
