<?php

/**
 * Implementation of hook_default_rules_configuration().
 */
function commerce_fianet_default_rules_configuration() {
  $items = array();
  $items['rules_queue_order_fianet_verification'] = entity_import('rules_config', '{ "rules_queue_order_fianet_verification" : {
      "LABEL" : "Queue order for FIA-NET verification",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "rules", "commerce_fianet", "entity" ],
      "ON" : [ "commerce_order_update" ],
      "IF" : [
        { "data_is" : { "data" : [ "commerce-order:state" ], "value" : "pending" } }
      ],
      "DO" : [
        { "commerce_fianet_queue_order_verification" : { "commerce_order" : [ "commerce_order" ] } }
      ]
    }
  }');
  return $items;
}
