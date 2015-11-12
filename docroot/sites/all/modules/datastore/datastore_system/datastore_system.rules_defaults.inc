<?php

/**
 * @file
 * Provides default rules
 */

/**
 * Implementation of hook_default_rules_configuration().
 */
function datastore_system_default_rules_configuration() {
  $items = array();
  $items['rules_datastore_system_logging'] = entity_import('rules_config', 
   '{ "rules_datastore_system_logging" : {
    "LABEL" : "Datastore system logging",
    "PLUGIN" : "action set",
    "REQUIRES" : [ "rules", "rules_scheduler", "datastore_entity" ],
    "USES VARIABLES" : { "schedule_date" : { "label" : "Schedule date", "type" : "date" } },
    "ACTION SET" : [
      { "schedule" : {
          "component" : "rules_datastore_system_logging",
          "date" : { "select" : "schedule-date", "date_offset" : { "value" : 86400 } },
          "identifier" : "datastore system logging",
          "param_schedule_date" : { "select" : "schedule-date", "date_offset" : { "value" : 86400 } }
        }
      },
      { "datastore_entity_add_values" : {
          "domain" : "system",
          "category" : [ "site:datastore-system-node-count:category" ],
          "values" : [ "site:datastore-system-node-count:values" ]
        }
      },
      { "datastore_entity_add_values" : {
          "domain" : "system",
          "category" : [ "site:datastore-system-user-count:category" ],
          "values" : [ "site:datastore-system-user-count:values" ]
        }
      }
    ]
  }
}');

  return $items;
}