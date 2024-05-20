#pragma once

#include <MQTTClient.h>

typedef struct seplosd_context {
    const char *bms_device;
    const char *topic;
    MQTTClient client;
} seplosd_context_t;