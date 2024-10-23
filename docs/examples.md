# Examples

The following examples illustrates how to use the `esclient-api` library to upload and download files.
The `esclient-api` library doesn't take care of initializing
the transport layer itself. We assume that the `CSP` library has been initialized
and configured with a valid physical interface. The `esclient-api` doesn't 'understand'
what is the physical layer attach underneath it works with any of the available CSP
[interfaces](https://github.com/libcsp/libcsp/tree/libcsp-1/include/csp/interfaces).

## File Transfer

```C
/* esclient-api lib header */
#include "esclient.h"

int es_client_file_test(void)
{
    int ret = 0;
    uint16_t csp_address = 10;

    es_client_init();
    es_client_csp_tl_init(csp_address);

    /* Upload a file with path `/path/to/host/filestem/filename`
    on location: `/path/to/remote/filestem/filename` */
    ret = es_client_file_upload("/path/to/host/filesystem/filename",
     "/path/to/remote/filesystem/filename");
    if (0 != ret) {
        ES_LOG_MSG(ES_ERROR, "File upload failed");
    }

    /* Download a file from "/path/to/host/filesystem/filename" to
    `/path/to/host/filesystem/filename` */
    ret = es_client_file_download("/path/to/host/filesystem/filename",
        "/path/to/host/filesystem/filename");
    if (0 != ret) {
        ES_LOG_MSG(ES_ERROR, "File download failed");
    }

    return 0;
}
```
