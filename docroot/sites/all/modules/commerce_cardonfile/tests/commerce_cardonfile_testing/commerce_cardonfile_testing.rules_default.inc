<?php

/**
 * @file
 * Default Rules configuration.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_cardonfile_testing_default_rules_configuration() {
  $items = array();

  // Reaction rule: Payment rule for Commerce Card on File Testing payment.
  $items['commerce_payment_commerce_cardonfile_testing_payment'] = entity_import('rules_config', '{ "commerce_payment_commerce_cardonfile_testing_payment" : {
      "LABEL" : "Commerce Card on File Test Payment",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Commerce Payment" ],
      "REQUIRES" : [ "commerce_payment" ],
      "ON" : [ "commerce_payment_methods" ],
      "DO" : [
        { "commerce_payment_enable_commerce_cardonfile_testing_payment" : {
            "commerce_order" : [ "commerce-order" ],
            "payment_method" : "commerce_cardonfile_testing_payment"
          }
        }
      ]
    }
  }');
}
