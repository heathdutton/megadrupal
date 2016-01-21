<?php
/**
 * @file
 * This files contains the auto created rule.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function economic_default_rules_configuration() {
  $configs = array();
  $rule = '{ "rules_transfer_order_to_economic" : {
    "LABEL" : "Transfer order to e-conomic",
    "PLUGIN" : "reaction rule",
    "ACTIVE" : false,
    "TAGS" : [ "e-conomic" ],
    "REQUIRES" : [ "rules", "economic", "entity" ],
    "ON" : [ "commerce_order_update" ],
    "IF" : [
      { "NOT data_is" : { "data" : [ "commerce-order-unchanged:status" ], "value" : "completed" } },
      { "data_is" : { "data" : [ "commerce-order:state" ], "value" : "completed" } }
    ],
    "DO" : [
      { "economic_order_transfer" : { "commerce_order" : [ "commerce_order" ] } }
    ]
  }
}';
  $configs['rules_transfer_order_to_economic'] = rules_import($rule);

  return $configs;
}
