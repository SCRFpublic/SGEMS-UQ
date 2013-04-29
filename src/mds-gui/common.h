#ifndef __METRICS_GUI_COMMON_H__
#define __METRICS_GUI_COMMON_H__

#ifdef WIN32
#pragma warning (disable: 4786)
#pragma warning (disable: 4231)
#pragma warning (disable: 4661)
#pragma warning (disable: 4660)
#pragma warning (disable: 4018)
#ifdef DISABLE_DLL_WARN
#pragma warning (disable: 4251)
#pragma warning (disable: 4275)
#endif
#endif

#if defined(_WIN32) || defined(WIN32)
  #ifdef LIB_STATIC
    #define METRICSGUI_DECL
  #else
    #ifdef METRICSGUI_EXPORTS
      #define METRICSGUI_DECL __declspec(dllexport)
    #else
      #define METRICSGUI_DECL __declspec(dllimport)
    #endif
  #endif
#else
    #define METRICSGUI_DECL
#endif

//!!WARNING!! Ugly hack to get around _debug definition conflict
#undef _CRT_MANIFEST_RETAIL

#endif