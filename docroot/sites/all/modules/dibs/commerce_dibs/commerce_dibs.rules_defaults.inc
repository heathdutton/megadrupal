<?php

/**
 * @file
 * Default rules configuration for commerce dibs.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_dibs_default_rules_configuration() {
  $rules = array();

  $rules['commerce_dibs_capture_payments'] = entity_import('rules_config', '{ "commerce_dibs_capture_payments" : {
      "LABEL" : "Capture dibs payments",
      "PLUGIN" : "action set",
      "TAGS" : [ "commerce_dibs" ],
      "REQUIRES" : [ "commerce_dibs" ],
      "USES VARIABLES" : { "order" : { "label" : "Order", "type" : "commerce_order" } },
      "ACTION SET" : [ { "commerce_dibs_capture_order" : { "commerce_order" : [ "order" ] } } ]
    }
  }');

  return $rules;
}
