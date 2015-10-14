<?php

/**
 * @file
 * Defines the action set that admins can execute in e.g. VBO.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_account_balance_default_rules_configuration() {
  $items['commerce_account_balance_deposit_to_account'] = entity_import('rules_config', '{ "rules_commerce_account_balance_deposit_to_account" : {
    "LABEL" : "Commerce Account Balance: deposit to account",
    "PLUGIN" : "action set",
    "OWNER" : "rules",
    "REQUIRES" : [ "commerce_account_balance" ],
    "USES VARIABLES" : {
      "user" : { "label" : "User", "type" : "user" },
      "amount" : { "label" : "Amount", "type" : "decimal" },
      "commerce_order" : { "label" : "Order", "type" : "commerce_order" }
    },
    "ACTION SET" : [
      { "commerce_account_balance_deposit_to_account" : {
          "user" : [ "user" ],
          "amount" : [ "amount" ],
          "commerce_order" : [ "commerce_order" ]
        }
      }
    ]
  }
}'
  );
  return $items;
}
