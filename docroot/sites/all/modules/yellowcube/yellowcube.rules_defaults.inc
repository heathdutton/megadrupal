<?php
/**
 * @file
 * yellowcube.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function yellowcube_default_rules_configuration() {
  $items = array();
  $items['rules_yellowcube_send_orders'] = entity_import('rules_config', '{ "rules_yellowcube_send_orders" : {
      "LABEL" : "Send queued orders to YellowCube",
      "PLUGIN" : "reaction rule",
      "WEIGHT" : "7",
      "OWNER" : "rules",
      "TAGS" : [ "yellowcube" ],
      "REQUIRES" : [ "rules", "yellowcube", "entity" ],
      "ON" : { "commerce_order_update" : [] },
      "IF" : [
        { "data_is" : { "data" : [ "commerce-order:status" ], "value" : "yellowcube_send" } },
        { "NOT data_is" : {
            "data" : [ "commerce-order:status" ],
            "value" : [ "commerce-order-unchanged:status" ]
          }
        }
      ],
      "DO" : [
        { "yellowcube_send_order_action" : { "order" : [ "commerce-order" ] } }
      ]
    }
  }');
  $items['rules_yellowcube_send_product_data'] = entity_import('rules_config', '{ "rules_yellowcube_send_product_data" : {
      "LABEL" : "Send product data to YellowCube",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "yellowcube" ],
      "REQUIRES" : [ "yellowcube", "entity" ],
      "ON" : { "commerce_product_insert" : [], "commerce_product_update" : [] },
      "DO" : [
        { "yellowcube_update_product_action" : { "order" : [ "commerce-product" ] } }
      ]
    }
  }');
  return $items;
}
