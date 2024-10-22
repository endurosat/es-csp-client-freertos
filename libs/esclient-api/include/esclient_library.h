/*
*/
#ifndef __ESCLIENT_LIBRARY_H_INCLUDED__
#define __ESCLIENT_LIBRARY_H_INCLUDED__

/* Set up environment for the application */

/* External dependencies */
#include <stdint.h>
#include <stddef.h>

/*  ESCLIENT version macros for compile-time API detection */
#define ESCLIENT_VERSION_MAJOR 0
#define ESCLIENT_VERSION_MINOR 1
#define ESCLIENT_VERSION_PATCH 0

#define ESCLIENT_MAKE_VERSION(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))

#define ESCLIENT_VERSION \
    ESCLIENT_MAKE_VERSION(ESCLIENT_VERSION_MAJOR, ESCLIENT_VERSION_MINOR, ESCLIENT_VERSION_PATCH)

#define ES_CLIENT_T_DEFINED
#define ES_LOG_T_DEFINED

/*  Public classes, each with its own header file */
#include "es_client.h"
#include "es_log.h"

#endif
