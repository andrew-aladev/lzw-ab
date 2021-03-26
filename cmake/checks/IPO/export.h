
#ifndef CHECK_EXPORT_H
#define CHECK_EXPORT_H

#ifdef CHECK_STATIC_DEFINE
#  define CHECK_EXPORT
#  define CHECK_NO_EXPORT
#else
#  ifndef CHECK_EXPORT
#    ifdef CHECK_EXPORTS
        /* We are building this library */
#      define CHECK_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define CHECK_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef CHECK_NO_EXPORT
#    define CHECK_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef CHECK_DEPRECATED
#  define CHECK_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef CHECK_DEPRECATED_EXPORT
#  define CHECK_DEPRECATED_EXPORT CHECK_EXPORT CHECK_DEPRECATED
#endif

#ifndef CHECK_DEPRECATED_NO_EXPORT
#  define CHECK_DEPRECATED_NO_EXPORT CHECK_NO_EXPORT CHECK_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef CHECK_NO_DEPRECATED
#    define CHECK_NO_DEPRECATED
#  endif
#endif

#endif /* CHECK_EXPORT_H */
