/******************************** INCLUDE FILES *******************************/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#include "esclient_classes.h"
#include "es_plf.h"

/*********************************** DEFINES **********************************/
#define LOG_BUFFER_SIZE 200
#define HEXDUMP_COLS 16
#define ES_LOG_TX_TIMEOUT 1000 /* ms */

/*********************************** TYPEDEFS *********************************/
typedef struct _es_log_priv_t {
    es_log_lvl_t log_lvl;
    es_log_tx_hndlr_t tx;
    es_log_timestamp_hndlr_t timestamp;
    bool_t enabled;
} es_log_priv_t;
#define es_log_priv_init_default  {ES_WARNING, NULL, NULL, FALSE}

/*********************************** STATE ************************************/
static es_log_priv_t es_log_priv = es_log_priv_init_default;

/******************************* DATA *****************************************/
static const char *log_lvl_str[ES_LVLS] = {
    "TRACE",
    "DEBUG",
    "INFO",
    "WARNING",
    "ERROR",
    "CRITICAL"
};

/*************************** INTERFACE FUNCTIONS ******************************/

#define ES_MODULE_NAME "es_log"

void es_log_init (es_log_iface_t *log_iface)
{
    es_log_priv.tx = log_iface->tx;
    es_log_priv.timestamp = log_iface->timestamp;
    es_log_priv.enabled = TRUE;
}

void
es_log_msg (const char *module, int line, int level, const char *format, ...)
{
    if (!es_log_priv.enabled)
        return;

    /* Check log lvl */
    if ( ((es_log_lvl_t)level < es_log_priv.log_lvl) || (es_log_priv.tx == NULL))
        return;

    uint32_t timestamp = 0;
    size_t size = 0;
    const char *line_termination = "\r\n";
    uint8_t buffer[LOG_BUFFER_SIZE];
    memset(buffer, 0x00, LOG_BUFFER_SIZE);

    if (es_log_priv.timestamp != NULL) {
        timestamp = es_log_priv.timestamp();
    }

    size = snprintf((char *) buffer, LOG_BUFFER_SIZE, "%s [%s] (%d): ", module,
        log_lvl_str[level], line);

    va_list argp;
    va_start(argp, format);
    size += vsnprintf((char *) buffer + size, LOG_BUFFER_SIZE - size, format, argp);
    va_end(argp);

    /* Append carrige return & new line to the end of the formated message */
    strcat((char *) buffer, line_termination);
    size += sizeof(line_termination); /* \r\n */

    /* Print the message */
    es_log_priv.tx(buffer, size, ES_LOG_TX_TIMEOUT);

    return;
}

void es_log_hexdump(const char *module, const char *desc, void *mem, unsigned int len)
{
    if (!es_log_priv.enabled)
        return;

    uint8_t buffer[LOG_BUFFER_SIZE];
    size_t size = 0;
    const uint8_t *line_end = (const uint8_t *)"\r\n";

    if ((es_log_lvl_t)es_log_get_lvl() >= ES_INFO)
    {
        return;
    }

    if (desc != NULL)
    {
        ES_LOG_MSG(ES_TRACE, "[%s] %s", module, desc);
    }
    unsigned int i, j;

    for(i = 0; i < len + ((len % HEXDUMP_COLS) ? (HEXDUMP_COLS - len % HEXDUMP_COLS) : 0); i++)
    {
        /* print offset */
        if(i % HEXDUMP_COLS == 0)
        {
            size = snprintf((char *) buffer, LOG_BUFFER_SIZE, "0x%06x: ", i);
            es_log_priv.tx(buffer, size, ES_LOG_TX_TIMEOUT);
        }
        /* print hex data */
        if(i < len)
        {
            size = snprintf((char *) buffer, LOG_BUFFER_SIZE, "%02x ",
                0xFF & ((char*)mem)[i]);
            es_log_priv.tx(buffer, size, ES_LOG_TX_TIMEOUT);
        }
        else /* end of block, just aligning for ASCII dump */
        {
            es_log_priv.tx((const uint8_t *)"   ", 3, ES_LOG_TX_TIMEOUT);
        }

        /* print ASCII dump */
        if(i % HEXDUMP_COLS == (HEXDUMP_COLS - 1))
        {
            for(j = i - (HEXDUMP_COLS - 1); j <= i; j++)
            {
                if(j >= len) /* end of block, not really printing */
                {
                    es_log_priv.tx((const uint8_t *)" ", 1, ES_LOG_TX_TIMEOUT);
                }
                else if(isprint(((char *) mem)[j])) /* printable char */
                {
                    es_log_priv.tx((const uint8_t *)(0xFF & ((char*)mem)[j]), 1, ES_LOG_TX_TIMEOUT);
                }
                else /* other char */
                {
                    es_log_priv.tx((const uint8_t *)".", 1, ES_LOG_TX_TIMEOUT);
                }
            }
            es_log_priv.tx(line_end, 2, ES_LOG_TX_TIMEOUT);
        }
    }

    return;
}

void
es_log_set_lvl (es_log_lvl_t level)
{
    if (level < ES_TRACE || level > ES_LVLS)
    {
        int i = 0;

        ES_LOG_MSG(ES_WARNING, "Invalid log level %d", level);
        ES_LOG_MSG(ES_INFO, "Available log level options:");
        for (i = 0; i < ES_LVLS; i++)
        {
            ES_LOG_MSG(ES_INFO, "lvl: %s, priority: %d", log_lvl_str[i], i);
        }

        return;
    }

    ES_LOG_MSG(ES_DEBUG, "Setting log level to %s", log_lvl_str[level]);
    es_log_priv.log_lvl = level;

    return;
}

es_log_lvl_t
es_log_get_lvl (void)
{
    return es_log_priv.log_lvl;
}
