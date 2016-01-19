<?php
/**
 * @file
 * cash_flow_income.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function cash_flow_income_default_rules_configuration() {
  $items = array();
  $items['rules_save_income_again'] = entity_import('rules_config', '{ "rules_save_income_again" : {
      "LABEL" : "Save income again",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "rules" ],
      "ON" : [ "node_insert", "node_update" ],
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "income" : "income" } } } },
        { "data_is_empty" : { "data" : [ "node:field-tax-override" ] } }
      ],
      "DO" : [ { "entity_save" : { "data" : [ "node" ] } } ]
    }
  }');
  return $items;
}
