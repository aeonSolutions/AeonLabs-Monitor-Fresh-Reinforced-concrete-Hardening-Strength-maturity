#include "mpspiffs.hpp"

#include <esp_spiffs.h>
#include <esp_task_wdt.h>
#include <freertos/task.h>
#include <FS.h>
#include <vfs_api.h>

SPIFFS::SPIFFS(fs::FSImplPtr impl) : FS(impl) {}

bool SPIFFS::begin(const char *partition, bool formatOnFail, const char *basePath, uint8_t maxOpenFiles)
{
    // don't do anything if partition is already mounted
    if (esp_spiffs_mounted(partition))
    {
        return true;
    }

    // the underlying fs code does not work well with mounting multiple partitions at the same time - if something is mounted that isn't the desired partition, abort
    if (m_mounted)
    {
        return false;
    }

    // try mounting the partition
    esp_vfs_spiffs_conf_t conf = {
        .base_path = basePath,
        .partition_label = partition,
        .max_files = maxOpenFiles,
        .format_if_mount_failed = false};
    auto error = esp_vfs_spiffs_register(&conf);

    // try to reformat (and register again) on error
    if(error == ESP_FAIL && formatOnFail)
    {
        if(format(partition))
        {
            error = esp_vfs_spiffs_register(&conf);
        }
    }

    // if still error, abort
    if(error != ESP_OK)
    {
        return false;
    }

    // success - update state and report success
    m_mounted = true;
    m_partition = partition;
    _impl->mountpoint(basePath);
    return true;
}

bool SPIFFS::end()
{
    if (esp_spiffs_mounted(m_partition))
    {
        esp_err_t error = esp_vfs_spiffs_unregister(m_partition);
        if (error != ESP_OK)
        {
            return false;
        }
        _impl->mountpoint(NULL);
        m_mounted = false;
        m_partition = NULL;
    }

    return true;
}

void enableWDT(int32_t core)
{
    auto idle = xTaskGetIdleTaskHandleForCPU(core);
    if(idle != NULL)
    {
        esp_task_wdt_add(idle);
    }
}

void disableWDT(int32_t core)
{
    auto idle = xTaskGetIdleTaskHandleForCPU(core);
    if(idle != NULL)
    {
        esp_task_wdt_delete(idle);
    }
}

bool SPIFFS::format(const char * partition)
{
    auto core = xPortGetCoreID();
    enableWDT(1 - core);
    esp_err_t error = esp_spiffs_format(partition);
    disableWDT(1 - core);
    return error == ESP_OK;
}

bool SPIFFS::getSize(uint32_t &total, uint32_t &used)
{
    if(!m_mounted)
    {
        return false;
    }

    auto code = esp_spiffs_info(m_partition, &total, &used);
    return code == ESP_OK;
}

bool SPIFFS::isMounted()
{
    return m_mounted;
}

const char* SPIFFS::mountedPartition()
{
    return m_partition;
}

SPIFFS Spiffs = SPIFFS(FSImplPtr(new VFSImpl()));
