/*****************************************************************************
 * **File:** es_log.h
 *
 * **Description:** ES Client lib logging utility. To use this module it is required to
 * 'attach' it to a configured UART interface. The `tx` handler of the
 * `log_iface` should be assigned to a functional UART transmit handler.
 *
 * **Example Usage**
 *
 * .. code-block:: C
 *      :caption: Initialize the ES Client log module
 *      :linenos:
 *      :lineno-start: 1
 *      :emphasize-lines: 6
 *      :name: <es_log>
 *
 *      uint32_t baudrate = 115200;
 *
 *      es_log_iface_t log_iface = es_log_iface_init_default;
 *      es_uart_dbg_drv_init(baudrate); // Target specific UART init function
 *      log_iface.timestamp = es_system_get_tick;  // Target specific get tick handler
 *      log_iface.drv_iface.tx = es_uart_dbg_drv_tx; // Target specific UART transmit handler
 *      es_log_init(&log_iface);
 *      es_log_set_lvl(ES_INFO);
 *      ES_LOG_MSG(ES_INFO, "Logging Initialized");
 *
 ****************************************************************************/

#ifndef ES_LOG_H_INCLUDED
#define ES_LOG_H_INCLUDED

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*********************************** DEFINES **********************************/

/**
 * ES log levels
 */
typedef enum _es_log_lvl_t
{
  ES_TRACE = 0, /**< A log level describing events showing step by step execution of your code that can be ignored during the standard operation, but may be useful during extended debugging sessions. */
  ES_DEBUG, /**< A log level used for events considered to be useful during software debugging when more
                granular information is needed. */
  ES_INFO, /**< An event happened, the event is purely informative and can be ignored during normal operations.*/
  ES_WARNING, /**< Unexpected behavior happened inside the application, but it is continuing its work and the key business features are operating as expected. */
  ES_ERROR, /**< One or more functionalities are not working, preventing some functionalities from working correctly.*/
  ES_CRITICAL, /**< One or more key business functionalities are not working and the whole system doesn’t fulfill the business functionalities. */
  ES_LVLS
} es_log_lvl_t;

/*********************************** MACROS ***********************************/

/**
 * Log a message
 *
 * Args:
 *  level: Log level of the message
 *  msg: VA_ARGS formated message
 */
#define ES_LOG_MSG(level, msg, ...) es_log_msg(ES_MODULE_NAME, __LINE__, level, msg, ## __VA_ARGS__)


/**
 * Hexdump a memory region
 *
 * Args:
 *  desc: Description of the memory in free form text
 *  mem_ptr: Memory to dump
 *  len: Size of the memory to dump starting from address mem_ptr
 */
#define ES_LOG_HEXDUMP(desc, mem_ptr, len) es_log_hexdump(ES_MODULE_NAME, desc, mem_ptr, len)

/*********************************** TYPEDEFS *********************************/

typedef int (*es_log_tx_hndlr_t)(const uint8_t *, size_t, uint32_t);
typedef uint32_t (*es_log_timestamp_hndlr_t)(void);

/**
 * Es log interface
 *
 * .. note:: In order to use the logging utility it is necessary to initialize
 *           the `tx` handler.
 */
typedef struct _es_log_iface_t {
    es_log_timestamp_hndlr_t timestamp; /**< timestamp handler */
    es_log_tx_hndlr_t tx; /**< TX handler */
} es_log_iface_t;
#define es_log_iface_init_default {NULL, NULL}
/******************************* INTERFACE FUNCTIONS **************************/

/**
 * Initialize es log interface.
 *
 * @param[in] log_iface: Reference to intialized `es_log_iface_t` structure
  */
void es_log_init (es_log_iface_t *log_iface);

/**
 * Set log level
 *
 * @param[in] level: Log level
 */
void
es_log_set_lvl (es_log_lvl_t level);

/**
 * Get the current log level
 *
 * :bdg-warning-line:`Will be deprecated`
 *
 * @returns Returns the current log level
 */
es_log_lvl_t
es_log_get_lvl (void);

void
es_log_msg (const char *module, int line, int level, const char *format, ...);
void
es_log_hexdump(const char* module, const char *desc, void *mem, unsigned int len);

#ifdef __cplusplus
}
#endif

#endif
