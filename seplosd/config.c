#include "config.h"

#include <libconfig.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"

static int __config_fill_string(config_t *config, const char *key, char **into)
{
    const char *string_value;
    char *copied_value;

    if (config_lookup_string(config, key, &string_value))
    {
        if (!(copied_value = strdup(string_value)))
        {
            log_fatal("out of memory reading bms_device key from config file.");
            return -1;
        }

        if (*into)
        {
            free(*into);
        }
        *into = copied_value;
    }

    return 0;
}

static int __config_fill_u64(config_t *config, const char *key, uint64_t *into)
{
    long long value;

    if (config_lookup_int64(config, key, &value))
    {
        *into = (int64_t)value;
    }

    return 0;
}

int seplosd_config_fill(const char *path, seplosd_context_t *context)
{
    config_t config;
    int r = -1;
    const char *string_value;
    char *copied_value;

    config_init(&config);

    if (!config_read_file(&config, path))
    {
        log_fatal("%s: %s at line %d", path, config_error_text(&config), config_error_line(&config));
        r = -1;
        goto out;
    }

    log_trace("using config file at %s", path);

    if (__config_fill_string(&config, "bms_device", &context->bms_device) < 0 ||
        __config_fill_string(&config, "topic", &context->topic) < 0 ||
        __config_fill_string(&config, "mqtt_uri", &context->mqtt_uri) < 0 ||
        __config_fill_string(&config, "mqtt_client_id", &context->mqtt_client_id) < 0 ||
        __config_fill_u64(&config, "interval", &context->interval) < 0)
    {

        log_fatal("config file parsing failed.");
        r = -1;
        goto out;
    }

    r = 0;
/* fall through */
out:
    config_destroy(&config);
    return r;
}