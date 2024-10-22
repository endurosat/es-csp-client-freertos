#ifndef __ES_PORT_H__
#define __ES_PORT_H__

/******************************** INCLUDE FILES *******************************/
#include <stdint.h>
#include <unistd.h>
#include "ff.h"

/*************************************** DEFINES ******************************/
#define ES_PORT_FILE_R_MODE FA_READ
#define ES_PORT_FILE_W_MODE FA_WRITE
#define ES_PORT_FILE_CREATE_ALWAYS FA_CREATE_ALWAYS

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/********************************* LOCAL DATA *********************************/

/*********************************** TYPEDEFS *********************************/
typedef FIL es_port_file_t;

/***************************** INTERFACE FUNCTIONS ****************************/
void es_port_delay(uint32_t val);

int8_t es_port_file_open(es_port_file_t *fp, const uint8_t *filename, uint8_t mode);
void es_port_file_close(es_port_file_t *fp);
int8_t es_port_file_exist(const uint8_t *filename);
int8_t es_port_file_remove(const uint8_t *filename);
// uint64_t es_port_file_size_get(const uint8_t *filename);
uint64_t es_port_file_size_get(es_port_file_t *fp);
ssize_t es_port_file_read(es_port_file_t *fp, uint8_t *data, uint32_t blksize);
ssize_t es_port_file_write(es_port_file_t *fp, uint8_t *data, uint32_t blksize);

#ifdef __cplusplus
}
#endif /* _cplusplus */
#endif /* __ES_PORT_H__ */
