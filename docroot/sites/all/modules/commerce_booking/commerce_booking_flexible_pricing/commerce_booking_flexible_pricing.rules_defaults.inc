<?php
/**
 * @file
 * Default rules configuration for flexible pricing.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_booking_flexible_pricing_default_rules_configuration() {
  $configs = array();

  $rule = '{ "commerce_booking_standard" : {
      "LABEL" : "Standard",
      "PLUGIN" : "and",
      "WEIGHT" : "90",
      "TAGS" : [ "Ticket Class" ],
      "USES VARIABLES" : { "ticket" : { "label" : "Ticket", "type" : "commerce_booking_ticket" } },
      "AND" : []
    }
  }';
  $configs['commerce_booking_standard'] = rules_import($rule);

  $rule = '{ "commerce_booking_guest" : {
      "LABEL" : "Guest",
      "PLUGIN" : "and",
      "WEIGHT" : "100",
      "TAGS" : [ "Ticket Class" ],
      "USES VARIABLES" : { "ticket" : { "label" : "Ticket", "type" : "commerce_booking_ticket" } },
      "AND" : []
    }
  }';
  $configs['commerce_booking_guest'] = rules_import($rule);

  return $configs;
}
