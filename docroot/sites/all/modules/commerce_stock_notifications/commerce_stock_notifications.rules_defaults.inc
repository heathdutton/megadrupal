<?php

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_stock_notifications_default_rules_configuration() {
  $rules['rules_send_commerce_stock_notifications'] = entity_import('rules_config', '{ "rules_send_commerce_stock_notifications" : {
    "LABEL" : "Send Commerce Stock Notifications",
    "PLUGIN" : "action set",
    "OWNER" : "rules",
    "REQUIRES" : ["rules"],
    "USES VARIABLES" : {
      "commerce_stock_notifications_product" : {
        "label" : "Product", "type" : "commerce_product" },
      "commerce_stock_notifications_email" : {
        "label" : "Email", "type" : "text" },
      "commerce_stock_notifications_path" : {
        "label" : "Path", "type" : "text" }
    },
    "ACTION SET" : [
      {
        "mail" : {
        "to" : ["commerce-stock-notifications-email"],
          "subject" : "The product you were waiting for on [site:url-brief] is now back in stock!",
          "message" : "You asked to be notified when \u003Ca href=\u0022[commerce-stock-notifications-path:value]\u0022\u003E[commerce-stock-notifications-product:title]\u003C\/a\u003E was back in stock.",
          "from" : ["site:mail"],
          "language" : [""]
        }
      }
    ]
  }
  }');

  $rules['rules_commerce_stock_notifications_send_notifications_on_product'] = entity_import('rules_config', '{ "rules_commerce_stock_notifications_send_notifications_on_product" : {
    "LABEL" : "Commerce Stock Notifications: Send notifications on product update",
    "PLUGIN" : "reaction rule",
    "OWNER" : "rules",
    "REQUIRES" : [ "rules", "commerce_stock_notifications", "entity" ],
    "ON" : { "commerce_product_update" : [] },
    "IF" : [
      { "entity_has_field" : { "entity" : [ "commerce-product" ], "field" : "commerce_stock_override" } },
      { "data_is" : { "data" : [ "commerce-product:commerce-stock-override" ], "value" : 1 } },
      { "OR" : [
          { "NOT entity_has_field" : {
              "entity" : [ "commerce-product-unchanged" ],
              "field" : "commerce_stock_override"
            }
          },
          { "data_is" : {
              "data" : [ "commerce-product-unchanged:commerce-stock-override" ],
              "value" : 0
            }
          }
        ]
      }
    ],
    "DO" : [
      { "notify_on_override_change" : { "commerce_product" : [ "commerce-product" ] } }
    ]
  }
}');

  return $rules;
}
