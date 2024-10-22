/******************************** INCLUDE FILES *******************************/
#include "cmsis_os2.h"
#include "es_system.h"
#include "es_sd_card.h"

#include "esclient.h"

/********************************** DEFINES ***********************************/
#define ES_MODULE_NAME  "es_tasks"
#define TEST_DATA_BUFFER_LEN 20 // bytes

/********************************* TYPPEDEFES *********************************/
typedef struct _es_freertos_task_t {
    osThreadFunc_t task;
    void *args;
    const osThreadAttr_t *attrs;
    osThreadId_t *tid;
} es_freertos_task_t;

/********************************* LOCAL DATA *********************************/
/* Definitions for defaultTask */
osThreadId_t default_task_handle;
const osThreadAttr_t default_task_attributes = {
  .name = "Default",
  .stack_size = 128,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t es_client_task_handle;
const osThreadAttr_t es_client_task_attributes = {
  .name = "ES Client",
  .stack_size = 10000,
  .priority = (osPriority_t) osPriorityNormal,
};

/******************************* LOCAL FUNCTIONS ******************************/
static void default_task(void * argument)
{
    /* Infinite loop */
    for(;;)
    {
        es_system_heartbeat();
        osDelay(1000);
    }
}

static int es_file_upload_download_test(void)
{
    int ret = 0;
    const char *test_file_name = "test_file.txt";
    es_sd_card_test_file_create(test_file_name, 1000);

    ES_LOG_MSG(ES_TRACE, "Uploading file: %s", test_file_name);
    ret = es_client_file_upload(test_file_name, "./test_file_new.txt");
    if (0 != ret) {
        ES_LOG_MSG(ES_ERROR, "File upload failed");
        return -1;
    }

    ES_LOG_MSG(ES_TRACE, "Downloading file: test_file_new.txt");
    ret = es_client_file_download("test_file_new.txt", "test_file_new.txt");
    if (0 != ret) {
        ES_LOG_MSG(ES_ERROR, "File download failed");
        return -1;
    }

    return 0;
}

static void es_client_task(void * argument)
{
    int ret = 0;

    /* Infinite loop */
    for(;;) {
        es_file_upload_download_test();
        osDelay(1000);
    }
}

/************************************* TASKS **********************************/
typedef enum _es_tasks_t {
    ES_DFLT_TASK = 0,
    ES_CLIENT_TASK,
    ES_TASKS_N
} es_tasks_t;

static es_freertos_task_t es_tasks[ES_TASKS_N] = {
    {
        default_task,
        NULL,
        &default_task_attributes,
        &default_task_handle,
    },
    {
        es_client_task,
        NULL,
        &es_client_task_attributes,
        &es_client_task_handle,
    }
};
/******************************* INTERFACE FUNCTIONS **************************/
void es_tasks_init(void)
{
    int i = 0;

    for (i = 0; i < ES_TASKS_N; i++) {
        ES_LOG_MSG(ES_INFO, "Starting: %s task", es_tasks[i].attrs->name);
        es_tasks[i].tid = osThreadNew(es_tasks[i].task, es_tasks[i].args, es_tasks[i].attrs);
    }
}
