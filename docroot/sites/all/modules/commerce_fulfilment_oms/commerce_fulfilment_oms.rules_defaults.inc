<?php
/**
 * @file
 * foo.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_fulfilment_oms_default_rules_configuration() {
  $items = array();

  $items['commerce_fulfilment_oms_order_has_been_fulfilled'] = entity_import('rules_config', '{
    "commerce_fulfilment_oms_order_has_been_fulfilled" : {
      "LABEL" : "Order has been fulfilled",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Commerce fulfilment" ],
      "REQUIRES" : [ "rules", "commerce_order", "commerce_fulfilment_oms" ],
      "ON" : [ "commerce_fulfilment_oms_rules_orderfulfiled_event" ],
      "DO" : [
        { "mail" : {
            "to" : "[commerce-order:mail]",
            "subject" : "Order [commerce-order:order-number] has been sent",
            "message" : "Your order [commerce-order:order-number] has just been posted.\n[commerce-order:tracking-url]",
            "language" : [ "" ]
          }
        },
        { "commerce_order_update_status" : { "commerce_order" : [ "commerce_order" ], "order_status" : "completed" } }
      ]
    }
  }');

  $items['commerce_fulfilment_oms_send_order_to_fulfilment'] = entity_import('rules_config', '{
    "commerce_fulfilment_oms_send_order_to_fulfilment" : {
      "LABEL" : "Send order to fulfilment",
      "PLUGIN" : "action set",
      "TAGS" : [ "Commerce fulfilment" ],
      "REQUIRES" : [ "commerce_fulfilment_oms" ],
      "USES VARIABLES" : { "order" : { "label" : "Order", "type" : "commerce_order" } },
      "ACTION SET" : [
        { "commerce_fulfilment_oms_rules_sendorder_action" : { "commerce_order" : [ "order" ] } }
      ]
    }
  }');

  $items['commerce_fulfilment_oms_schedule_sendorder'] = entity_import('rules_config', '{
    "commerce_fulfilment_oms_schedule_sendorder" : {
      "LABEL" : "Schedule sending order to fulfilment",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Commerce" ],
      "REQUIRES" : [ "rules", "rules_scheduler", "commerce_payment" ],
      "ON" : [ "commerce_payment_order_paid_in_full" ],
      "DO" : [
        { "schedule" : {
            "component" : "commerce_fulfilment_oms_send_order_to_fulfilment",
            "date" : "+5 minutes",
            "identifier" : "commerce_fulfilment_oms_sendorder_[commerce-order:order-id]",
            "param_order" : [ "commerce-order" ]
          }
        }
      ]
    }
  }');

  return $items;
}
