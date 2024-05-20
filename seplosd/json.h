
#pragma once

#include <json-c/json.h>

#include "seplos.h"

bool
seplosd_json_serialize(const SeplosData *const data, json_object *root);