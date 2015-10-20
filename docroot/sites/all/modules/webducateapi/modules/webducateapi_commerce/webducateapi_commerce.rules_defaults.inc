<?php
/**
 * @file
 * webducateapi_commerce.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function webducateapi_commerce_default_rules_configuration() {
  $items = array();
  $items['rules_webducateapi_commerce_create_token'] = entity_import('rules_config', '{ "rules_webducateapi_commerce_create_token" : {
      "LABEL" : "Create Webducate Token on checkout completion",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "WebducateAPI - Commerce" ],
      "REQUIRES" : [ "webducateapi_commerce", "commerce_checkout" ],
      "ON" : [ "commerce_checkout_complete" ],
      "DO" : [
        { "LOOP" : {
            "USING" : { "list" : [ "commerce-order:commerce-line-items" ] },
            "ITEM" : { "list_item" : "Current list item" },
            "DO" : [
              { "webducateapi_commerce_create_token" : { "line_item" : [ "list-item" ] } }
            ]
          }
        }
      ]
    }
  }');
  return $items;
}
