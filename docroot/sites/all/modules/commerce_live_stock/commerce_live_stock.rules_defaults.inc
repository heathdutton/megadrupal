<?php
/**
 * @file
 * Default rule configurations for Commerce Live Stock.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_live_stock_default_rules_configuration() {
  $rules = array();

  $rules_export = '{ "rules_decrement_live_stock_completing_order" : {
      "LABEL" : "Stock: Decrease in live when completing the order process",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "commerce_ss", "commerce_checkout" ],
      "ON" : { "commerce_checkout_complete" : [] },
      "DO" : [
        { "LOOP" : {
            "USING" : { "list" : [ "commerce-order:commerce-line-items" ] },
            "ITEM" : { "list_item" : "Current list item" },
            "DO" : [
              { "rules_commerce_live_stock" : { "commerce_line_item" : [ "list-item" ] } }
            ]
          }
        }
      ]
    }
  }';

  $rules['rules_decrement_live_stock_completing_order'] = rules_import($rules_export);

  return $rules;
}
