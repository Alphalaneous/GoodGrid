#pragma once

#ifdef GEODE_IS_WINDOWS
    #ifdef GOOD_GRID_API_EXPORTING
        #define GOOD_GRID_API_DLL __declspec(dllexport)
    #else
        #define GOOD_GRID_API_DLL __declspec(dllimport)
    #endif
#else
    #define GOOD_GRID_API_DLL __attribute__((visibility("default")))
#endif
