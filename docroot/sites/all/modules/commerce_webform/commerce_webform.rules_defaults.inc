<?php
/**
 * @file
 * commerce_webform.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_webform_default_rules_configuration() {
  $items = array();
  $items['rules_add_commerce_webform_product_to_cart'] = entity_import('rules_config', '{ "rules_add_commerce_webform_product_to_cart" : {
      "LABEL" : "Add commerce webform product to cart",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "commerce_webform" ],
      "REQUIRES" : [ "commerce_webform", "rules", "webform_rules" ],
      "ON" : [ "webform_rules_submit" ],
      "IF" : [
        { "commerce_webform_is_commerce_webform" : { "webform" : [ "node" ] } }
      ],
      "DO" : [ { "commerce_webform_order_create" : { "node" : [ "node" ], "data" : [ "data" ], "user" : [ "user" ] } } ]
    }
  }');
  $items['rules_mark_commerce_webform_as_paid'] = entity_import('rules_config', '{ "rules_mark_commerce_webform_as_paid" : {
      "LABEL" : "Mark commerce webform as paid",
      "PLUGIN" : "reaction rule",
      "WEIGHT" : "5",
      "TAGS" : [ "commerce_webform" ],
      "REQUIRES" : [ "commerce_webform", "commerce_checkout" ],
      "ON" : [ "commerce_payment_order_paid_in_full" ],
      "DO" : [ { "commerce_webform_mark_paid" : { "node" : [ "commerce-order" ] } } ]
    }
  }');
  $items['rules_update_an_order'] = entity_import('rules_config', '{ "rules_update_an_order" : {
      "LABEL" : "Update an order",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "commerce_webform" ],
      "REQUIRES" : [ "commerce_webform", "webform_rules" ],
      "ON" : [ "webform_rules_update" ],
      "IF" : [
        { "commerce_webform_is_commerce_webform" : { "webform" : [ "node" ] } }
      ],
      "DO" : [
        { "commerce_webform_order_update" : { "node" : [ "node" ], "data" : [ "data" ], "user" : [ "user" ] } }
      ]
    }
  }');
  return $items;
}
