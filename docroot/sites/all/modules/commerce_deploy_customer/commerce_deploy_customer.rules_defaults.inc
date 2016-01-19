<?php

/**
 * @file
 * Default rules configurations for Commerce Deploy Customer
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_deploy_customer_default_rules_configuration() {
  $configs = array();
  $rule = '{ "rules_commerce_order_order_marked_complete" : {
    "LABEL" : "Commerce Order: order marked complete",
    "PLUGIN" : "reaction rule",
    "TAGS" : [ "Commerce Order Complete Notify" ],
    "REQUIRES" : [ "rules", "message_notify", "entity" ],
    "ON" : [ "commerce_order_presave" ],
    "IF" : [
      { "entity_is_of_type" : { "entity" : [ "commerce-order" ], "type" : "commerce_order" } },
      { "NOT data_is_empty" : { "data" : [ "commerce-order:owner" ] } },
      { "NOT data_is" : { "data" : [ "commerce-order-unchanged:status" ], "value" : "completed" } },
      { "data_is" : { "data" : [ "commerce-order:status" ], "value" : "completed" } }
    ],
    "DO" : [
      { "drupal_message" : { "message" : "Order notification has been sent" } },
      { "entity_create" : {
          "USING" : {
            "type" : "message",
            "param_type" : "commerce_order_complete_notify",
            "param_user" : [ "commerce-order:owner" ]
          },
          "PROVIDE" : { "entity_created" : { "entity_created" : "Created entity" } }
        }
      },
      { "data_set" : {
          "data" : [ "entity-created:message-commerce-order" ],
          "value" : [ "commerce-order" ]
        }
      },
      { "entity_save" : { "data" : [ "entity-created" ], "immediate" : 1 } },
      { "message_notify_process" : {
          "message" : [ "entity-created" ],
          "save_on_fail" : 0,
          "save_on_success" : 0,
          "rendered_body_field" : "message_commerce_body"
        }
      }
    ]
  }
}';
  $configs['rules_commerce_order_order_marked_complete'] = rules_import($rule);
  return $configs;
}
