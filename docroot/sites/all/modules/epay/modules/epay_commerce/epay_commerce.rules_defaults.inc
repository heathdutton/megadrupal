<?php

/**
 * @file
 * Default rule configurations for ePay Commerce.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function epay_commerce_default_rules_configuration() {
  $items = array();
  $items['epay_commerce_capture_payment'] = entity_import('rules_config', '{ "epay_commerce_capture_payment" : {
      "LABEL" : "ePay Commerce capture payment",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "ePay Commerce" ],
      "REQUIRES" : [ "rules", "epay_commerce" ],
      "USES VARIABLES" : { "commerce_order" : { "label" : "Order", "type" : "commerce_order" } },
      "IF" : [
        { "data_is" : {
            "data" : [ "commerce-order:created" ],
            "op" : "\\u003C",
            "value" : "-7 day"
          }
        }
      ],
      "DO" : [
        { "epay_commerce_capture_remaining_amount" : { "commerce_order" : [ "commerce_order" ] } }
      ]
    }
  }');
  $items['epay_commerce_capture_payments'] = entity_import('rules_config', '{ "epay_commerce_capture_payments" : {
      "LABEL" : "ePay Commerce capture payments",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "OWNER" : "rules",
      "TAGS" : [ "ePay Commerce" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "cron" : [] },
      "DO" : [
        { "entity_query" : {
            "USING" : {
              "type" : "commerce_order",
              "property" : "status",
              "value" : "completed",
              "limit" : "25"
            },
            "PROVIDE" : { "entity_fetched" : { "commerce_orders" : "Orders" } }
          }
        },
        { "LOOP" : {
            "USING" : { "list" : [ "commerce-orders" ] },
            "ITEM" : { "commerce_order" : "Order" },
            "DO" : [
              { "component_epay_commerce_capture_payment" : { "commerce_order" : [ "commerce_order" ] } }
            ]
          }
        }
      ]
    }
  }');

  return $items;
}