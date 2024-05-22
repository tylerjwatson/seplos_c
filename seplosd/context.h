#pragma once

#include <MQTTClient.h>
#include <stdint.h>

typedef struct seplosd_context {
    char *bms_device;
    char *topic;
    char *mqtt_uri;
    char *mqtt_client_id;
    uint64_t interval;
    MQTTClient client;
} seplosd_context_t;