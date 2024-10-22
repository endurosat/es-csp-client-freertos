/******************************** INCLUDE FILES *******************************/
#include "cmsis_os2.h"

#include "es_port.h"
/********************************* LOCAL DEFINES ******************************/

/******************************* LOCAL FUNCTIONS ******************************/

/***************************** INTERFACE FUNCTIONS ****************************/
void es_port_delay(uint32_t val)
{
	osDelay(val);
}

int8_t es_port_file_open(es_port_file_t *fp, const uint8_t *filename, uint8_t mode)
{
	FRESULT ret = FR_INVALID_PARAMETER;

	ret = f_open(fp, (const TCHAR*)filename, (BYTE) mode);
	if (FR_OK != ret)
		return -1;

	return 0;
}

void es_port_file_close(es_port_file_t *fp)
{
	f_close(fp);
}

int8_t es_port_file_remove(const uint8_t *filename)
{
	f_unlink(filename);
}

int8_t es_port_file_exist(const uint8_t *filename)
{
	FRESULT ret = FR_INVALID_PARAMETER;
	FILINFO file_info;

	ret = f_stat ((const TCHAR *)filename, &file_info);
	if (FR_OK != ret)
		return -1;

	return 0;
}

ssize_t es_port_file_read(es_port_file_t *fp, uint8_t *data, uint32_t blksize)
{
	FRESULT ret = FR_INVALID_PARAMETER;
	ssize_t bytes_read = 0;

	ret = f_read (fp, data, blksize, &bytes_read);
	if (FR_OK != ret)
		return -1;

    return bytes_read;
}

ssize_t es_port_file_write(es_port_file_t *fp, uint8_t *data, uint32_t blksize)
{
	FRESULT ret = FR_INVALID_PARAMETER;
	ssize_t bytes_written = 0;

	ret = f_write (fp, data, blksize, &bytes_written);
	if (FR_OK != ret)
		return -1;

	return bytes_written;
}
