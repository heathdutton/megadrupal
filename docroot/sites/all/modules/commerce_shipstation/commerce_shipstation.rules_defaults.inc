<?php
/**
 * @file
 *
 * Provides default rules for Commerce Shipstation.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_shipstation_default_rules_configuration() {
  $items = array();

  $items['commerce_shipstation_order_fulfilment_success'] = entity_import('rules_config', '{
    "commerce_shipstation_order_fulfilment_success" : {
      "LABEL" : "Order has been fulfilled",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Commerce ShipStation" ],
      "REQUIRES" : [ "rules", "commerce_order", "commerce_shipstation" ],
      "ON" : [ "commerce_shipstation_order_success" ],
      "DO" : [
        { "commerce_order_update_status" : { "commerce_order" : [ "commerce_order" ], "order_status" : "completed" } }
      ]
    }
  }');

  return $items;
}
