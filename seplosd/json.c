
#include <json-c/json.h>
#include <math.h>

#include "json.h"
#include "log.h"

bool seplosd_json_serialize(const SeplosData *const data, json_object *root)
{
    float power = data->charge_discharge_current * data->total_battery_voltage;
    float dv = data->highest_cell_voltage - data->lowest_cell_voltage;

    return !(json_object_object_add(root, "i",
                                    json_object_new_double(data->charge_discharge_current)) < 0 ||
             json_object_object_add(root, "v",
                                    json_object_new_double(data->total_battery_voltage)) < 0 ||
             json_object_object_add(root, "dv",
                                    json_object_new_double(dv)) < 0 ||
             json_object_object_add(root, "p",
                                    json_object_new_double(power)) < 0 ||
             json_object_object_add(root, "soc",
                                    json_object_new_int(round(data->state_of_charge))) < 0 ||
             json_object_object_add(root, "soh",
                                    json_object_new_int(round(data->state_of_health))) < 0 ||
             json_object_object_add(root, "cap",
                                    json_object_new_int(round(data->battery_capacity))) < 0 ||
             json_object_object_add(root, "ncycles",
                                    json_object_new_int(data->number_of_cycles)) < 0 ||
             json_object_object_add(root, "cap_residual",
                                    json_object_new_int(round(data->residual_capacity))) < 0 ||
             json_object_object_add(root, "cap_rated",
                                    json_object_new_int(round(data->rated_capacity))) < 0 ||
             json_object_object_add(root, "bal",
                                    json_object_new_boolean(data->equilibrium_state)) < 0 ||
             json_object_object_add(root, "hot",
                                    json_object_new_boolean(data->hot)) < 0 ||
             json_object_object_add(root, "cold",
                                    json_object_new_boolean(data->cold)) < 0 ||
             json_object_object_add(root, "shutdown",
                                    json_object_new_boolean(data->shutdown)) < 0 ||
             json_object_object_add(root, "standby",
                                    json_object_new_boolean(data->standby)) < 0 ||
             json_object_object_add(root, "charge",
                                    json_object_new_boolean(data->charge)) < 0 ||
             json_object_object_add(root, "discharge",
                                    json_object_new_boolean(data->discharge)) < 0 ||
             json_object_object_add(root, "charge_sw",
                                    json_object_new_boolean(data->charge_switch)) < 0 ||
             json_object_object_add(root, "discharge_sw",
                                    json_object_new_boolean(data->discharge_switch)) < 0 ||
             json_object_object_add(root, "max_temp",
                                    json_object_new_int(round(data->highest_temperature))) < 0 ||
             json_object_object_add(root, "min_temp",
                                    json_object_new_int(round(data->lowest_temperature))) < 0 ||
             json_object_object_add(root, "environment_temp",
                                    json_object_new_int(round(data->temperature[4]))) < 0 ||
             json_object_object_add(root, "bms_temp",
                                    json_object_new_int(round(data->temperature[5]))) < 0

    );
}