#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <uv.h>
#include <string.h>
#include <unistd.h>

#include "log.h"
#include "seplos.h"
#include "context.h"
#include "json.h"
#include "config.h"

static void __timer_on_tick(uv_timer_t *timer)
{
  int fd, r;
  SeplosData data;
  seplosd_context_t *context = (seplosd_context_t *)timer->data;
  MQTTClient_message message = MQTTClient_message_initializer;
  MQTTClient_deliveryToken token;

  log_trace("timer on tick");

  if ((fd = seplos_open(context->bms_device)) < 0)
  {
    log_error("cannot open device: %s, will try again next tick", strerror(errno));
    goto out;
  }

  log_trace("bms open. fd=%d", fd);

  if ((r = seplos_data(fd, 0, 1, &data)) < 0)
  {
    log_error("bms read failure, will try again next tick");
    goto out;
  }

  log_info("bms soc=%.2f i=%.2f v=%.2f",
           data.state_of_charge,
           data.charge_discharge_current,
           data.total_battery_voltage);

  json_object *root;

  if (!(root = json_object_new_object()))
  {
    log_error("cannot allocate new json object.");
    goto out;
  }

  if (!seplosd_json_serialize(&data, root))
  {
    log_error("cannot serialize bms data into json.");
    goto json_out;
  }

  message.qos = 0;
  message.retained = 0;
  message.payload = (char *)json_object_to_json_string(root);
  message.payloadlen = (int)strlen(message.payload);

  if ((r = MQTTClient_publishMessage(context->client, context->topic, &message, &token)) != MQTTCLIENT_SUCCESS ||
      (r = MQTTClient_waitForCompletion(context->client, token, 1000)) != MQTTCLIENT_SUCCESS)
  {
    log_error("error publishing message to topic. rc=%d", r);
    goto json_out;
  }

  log_info("mqtt: message published.  topic=%s", context->topic);

json_out:
  json_object_put(root);

out:
  log_trace("close fd %d", fd);
  close(fd);
}

static int __validate_context(seplosd_context_t *context)
{

  if (!context->bms_device || !strcmp(context->bms_device, ""))
  {
    log_error("configuration error, bms_device is required.");
    return -1;
  }

  if (!context->mqtt_uri || !strcmp(context->mqtt_uri, ""))
  {
    log_error("configuration error, mqtt_uri is required.");
    return -1;
  }

  if (!context->topic || !strcmp(context->topic, ""))
  {
    log_error("configuration error, topic is required.");
    return -1;
  }

  if (!context->mqtt_client_id || !strcmp(context->mqtt_client_id, ""))
  {
    log_error("configuration error, mqtt_client_id is required.");
    return -1;
  }

  return 0;
}

int main(int argc, char **argv)
{
  uv_loop_t *loop = uv_default_loop();
  uv_timer_t timer = {};
  int r, opt;
  const char *config_path = "/etc/seplosd.conf";
  seplosd_context_t context = {};

  while ((opt = getopt(argc, argv, "c:")) != -1)
  {
    switch (opt)
    {
    case 'c':
      config_path = optarg;
      break;
    }
  }

  if ((r = seplosd_config_fill(config_path, &context)) < 0)
  {
    log_fatal("Cannot read config file. rc=%d", r);
    goto config_out;
  }

  if (__validate_context(&context) < 0)
  {
    log_fatal("configuration validation failed.");
    goto out;
  }

  MQTTClient_connectOptions options = MQTTClient_connectOptions_initializer;

  if ((r = uv_timer_init(loop, &timer)) < 0)
  {
    log_fatal("uv timer initialization failed: %s", uv_strerror(r));
    goto out;
  }

  if ((r = MQTTClient_create(&context.client,
                             context.mqtt_uri,
                             context.mqtt_client_id,
                             MQTTCLIENT_PERSISTENCE_NONE,
                             NULL)) != MQTTCLIENT_SUCCESS)
  {
    log_fatal("cannot create MQTT client. rc=%d", r);
    goto out;
  }

  options.keepAliveInterval = 20;
  options.cleansession = true;

  if ((r = MQTTClient_connect(context.client, &options)) != MQTTCLIENT_SUCCESS)
  {
    log_fatal("mqtt client failed to connect.  rc=%d", r);
    goto out;
  }

  log_info("mqtt connected");

  timer.data = &context;

  if ((r = uv_timer_start(&timer,
                          __timer_on_tick,
                          0,
                          context.interval)) < 0)
  {
    log_fatal("uv timer start failure.");
    goto out;
  }

  uv_run(loop, UV_RUN_DEFAULT);

  r = 0;
  /* fall through */
mqtt_connect_out:
  if (MQTTClient_disconnect(context.client, 1000) != MQTTCLIENT_SUCCESS)
  {
    log_error("cannot disconnect from mqtt server. we're exiting anyway.");
  }
mqtt_out:
  MQTTClient_destroy(context.client);

out:
  if (context.bms_device)
  {
    free(context.bms_device);
  }
  if (context.topic)
  {
    free(context.topic);
  }
  if (context.mqtt_uri)
  {
    free(context.mqtt_uri);
  }
  if (context.mqtt_client_id)
  {
    free(context.mqtt_client_id);
  }

config_out:
  uv_loop_close(loop);

  return r;
}
