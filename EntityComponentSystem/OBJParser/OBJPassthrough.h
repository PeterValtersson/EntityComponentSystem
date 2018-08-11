// Passthough that accepts .obj format and turns it into .arf
#pragma once
#include <stdint.h>
#include <Error.h>
#include "ObjParser.h"

#define DLL_EXPORT extern "C" _declspec(dllexport) 
DLL_EXPORT Utilities::Error Parse(uint32_t guid, void* data, uint64_t size, void** parsedData, uint64_t* parsedSize);
DLL_EXPORT Utilities::Error DestroyParsedData(uint32_t guid, void* data, uint64_t size);

DLL_EXPORT Utilities::Error Load(uint32_t guid, void* data, uint64_t size, void** loadedDataRAM, uint64_t* loadedSizeRAM, uint64_t* loadedSizeVRAM);
DLL_EXPORT Utilities::Error Unload(uint32_t guid, void* dataRAM, uint64_t sizeRAM, uint64_t sizeVRAM);