<?php

/**
 * @file
 * Default rule configurations for Commerce recurring.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_recurring_bonus_default_rules_configuration() {
  $configs = array();
  $rule = '{ "rules_commerce_recurring_bonus_deactivate_ended" : {
    "LABEL" : "Deactivate ended recurring entities",
    "PLUGIN" : "reaction rule",
    "TAGS" : [ "commerce_recurring" ],
    "REQUIRES" : [ "rules", "commerce_recurring", "commerce_recurring_bonus" ],
    "ON" : [ "commerce_recurring_cron" ],
    "DO" : [
      { "commerce_recurring_bonus_get_expired_items" : {
          "USING" : { "number_items" : "5", "timestamp" : "now" },
          "PROVIDE" : { "commerce_recurring_entities" : { "commerce_recurring_entities_expired" : "Commerce Recurring Entit\u00e4ten Expired" } }
        }
      },
      { "LOOP" : {
          "USING" : { "list" : [ "commerce-recurring-entities-expired" ] },
          "ITEM" : { "list_item" : "Current list element" },
          "DO" : [
            { "data_set" : { "data" : [ "list-item:status" ], "value" : 0 } },
            { "commerce_recurring_stop_recurring" : { "data" : [ "list-item" ] } }
          ]
        }
      }
    ]
  }
  }';
  $configs['rules_commerce_recurring_bonus_deactivate_ended'] = rules_import($rule);

  return $configs;
}
