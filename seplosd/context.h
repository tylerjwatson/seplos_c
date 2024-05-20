#pragma once

#include <MQTTClient.h>

typedef struct seplosd_context {
    const char *bms_device;
    const char *topic;
    const char *mqtt_uri;
    const char *mqtt_client_id;
    const uint64_t interval;
    MQTTClient client;
} seplosd_context_t;