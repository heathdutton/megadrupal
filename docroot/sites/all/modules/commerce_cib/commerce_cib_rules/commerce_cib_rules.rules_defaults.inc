<?php
/**
 * @file
 * commerce_cib_rules.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_cib_rules_default_rules_configuration() {
  $items = array();
  $items['rules_cib_bank_transaction_communication_failure'] = entity_import('rules_config', '{ "rules_cib_bank_transaction_communication_failure" : {
      "LABEL" : "CIB bank transaction communication failure",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "CIB" ],
      "REQUIRES" : [ "rules", "commerce_order", "commerce_cib" ],
      "ON" : { "commerce_cib_no_communication" : [] },
      "DO" : [
        { "drupal_message" : { "message" : "Sorry, we could not communicate with CIB. Please try again later!\\r\\n\\u003Ca href=\\u0022\\/checkout\\u0022\\u003EGo back to the checkout page\\u003C\\/a\\u003E" } },
        { "commerce_order_update_state" : { "commerce_order" : [ "commerce_order" ], "order_state" : "checkout" } }
      ]
    }
  }');
  $items['rules_cib_get_last_transaction'] = entity_import('rules_config', '{ "rules_cib_get_last_transaction" : {
      "LABEL" : "CIB Get last transaction",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "TAGS" : [ "CIB" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : {
        "order" : { "label" : "Order", "type" : "commerce_order" },
        "transaction" : {
          "label" : "Transaction",
          "type" : "commerce_payment_transaction",
          "parameter" : false
        }
      },
      "ACTION SET" : [
        { "entity_query" : {
            "USING" : {
              "type" : "commerce_payment_transaction",
              "property" : "order_id",
              "value" : [ "order:order-id" ]
            },
            "PROVIDE" : { "entity_fetched" : { "transactions" : "Transactions" } }
          }
        },
        { "LOOP" : {
            "USING" : { "list" : [ "transactions" ] },
            "ITEM" : { "list_item" : "Current list item" },
            "DO" : [
              { "data_set" : { "data" : [ "transaction" ], "value" : [ "list-item" ] } }
            ]
          }
        }
      ],
      "PROVIDES VARIABLES" : [ "transaction" ]
    }
  }');
  $items['rules_cib_succesful_transaction'] = entity_import('rules_config', '{ "rules_cib_succesful_transaction" : {
      "LABEL" : "CIB succesful transaction",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "CIB" ],
      "REQUIRES" : [ "rules", "commerce_checkout" ],
      "ON" : { "commerce_checkout_complete" : [] },
      "DO" : [
        { "component_rules_cib_get_last_transaction" : {
            "USING" : { "order" : [ "commerce-order" ] },
            "PROVIDE" : { "transaction" : { "transaction" : "Transaction" } }
          }
        },
        { "drupal_message" : { "message" : "The transaction id is: [transaction:remote-id].\\r\\nThe response code is: [transaction:remote-status] .\\r\\nThe response message is: [transaction:message].\\r\\nAn email has been sent to [commerce-order:mail] with the details.\\r\\n\\r\\nThe permission number is: [transaction:commerce-cib-anum].\\r\\nThe paid amount is: [transaction:amount] HUF." } },
        { "mail" : {
            "to" : [ "commerce-order:mail" ],
            "subject" : "Order on [site:name]",
            "message" : "Dear Customer, \\r\\n\\r\\nThe transaction id is: [transaction:remote-id].\\r\\nThe response code is: [transaction:remote-status] .\\r\\nThe response message is: [transaction:message].\\r\\n\\r\\nThe permission number is: [transaction:commerce-cib-anum].\\r\\nThe paid amount is: [transaction:amount] HUF.\\r\\n\\r\\nThank you for your purchase!\\r\\n[site:name] team.",
            "language" : [ "" ]
          }
        },
        { "redirect" : { "url" : "[site:url]" } }
      ]
    }
  }');
  $items['rules_cib_timeouted_transaction'] = entity_import('rules_config', '{ "rules_cib_timeouted_transaction" : {
      "LABEL" : "CIB timeouted transaction",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "CIB" ],
      "REQUIRES" : [ "rules", "commerce_cib" ],
      "ON" : { "commerce_cib_timeout" : [] },
      "DO" : [
        { "component_rules_cib_get_last_transaction" : {
            "USING" : { "order" : [ "commerce-order" ] },
            "PROVIDE" : { "transaction" : { "transaction" : "Transaction" } }
          }
        },
        { "mail" : {
            "to" : [ "commerce-order:mail" ],
            "subject" : "[site:name] transaction timeout",
            "message" : "Dear Customer,\\r\\nYou are receiving this email because your email address ([commerce-order:mail])  was given in an order at [site:url] with transaction ID [transaction:remote-id].\\r\\n\\r\\nWe apologize but your transaction did not get through. The error was the following:\\r\\n[transaction:message]\\r\\n\\r\\nImportant\\r\\nIf your card is processed during the transaction, the payment processor is automatically reversing your transaction within 10 minutes.\\r\\n\\r\\nThe products are kept in your shopping cart at [site:name]. \\r\\n\\r\\nThe transaction id is [transaction:remote-id].\\r\\nThe response code is [transaction:remote-status] .\\r\\nThe permission number is [transaction:commerce-cib-anum].\\r\\nThe paid amount is [transaction:amount] HUF.\\r\\nWe apologize for the inconvenience.\\r\\n\\r\\n-- [site:name]  team",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  $items['rules_cib_transaction_initialization_failure'] = entity_import('rules_config', '{ "rules_cib_transaction_initialization_failure" : {
      "LABEL" : "CIB transaction initialization failure",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "CIB" ],
      "REQUIRES" : [ "rules", "commerce_cib" ],
      "ON" : { "commerce_cib_initialization_fails" : [] },
      "DO" : [
        { "drupal_message" : { "message" : "Transaction initialization failed.", "type" : "warning" } },
        { "redirect" : { "url" : "checkout" } }
      ]
    }
  }');
  $items['rules_cib_unsuccesful_transaction'] = entity_import('rules_config', '{ "rules_cib_unsuccesful_transaction" : {
      "LABEL" : "CIB unsuccesful transaction",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "CIB" ],
      "REQUIRES" : [ "rules", "commerce_order", "commerce_cib" ],
      "ON" : { "commerce_cib_transaction_fails" : [] },
      "DO" : [
        { "component_rules_cib_get_last_transaction" : {
            "USING" : { "order" : [ "commerce-order" ] },
            "PROVIDE" : { "transaction" : { "transaction" : "Transaction" } }
          }
        },
        { "drupal_message" : {
            "message" : "The transaction id is: [transaction:remote-id].\\r\\nThe response code is: [transaction:remote-status] .\\r\\nThe response message is: [transaction:message].\\r\\nAn email has been sent to [commerce-order:mail] with the details.\\r\\n\\r\\nSomething went wrong. Please try again!",
            "type" : "warning"
          }
        },
        { "mail" : {
            "to" : "[commerce-order:mail]",
            "subject" : "Unsuccesful order on [site:name]",
            "message" : "Dear Customer, \\r\\n\\r\\nThe transaction id is: [transaction:remote-id].\\r\\nThe response code is: [transaction:remote-status] .\\r\\nThe response message is: [transaction:message].\\r\\n\\r\\nSomething went wrong with the order. Please try again!\\r\\n\\r\\n[site:name] team.",
            "language" : [ "" ]
          }
        },
        { "commerce_order_update_status" : {
            "commerce_order" : [ "commerce_order" ],
            "order_status" : "checkout_checkout"
          }
        },
        { "redirect" : { "url" : "[site:url]" } }
      ]
    }
  }');
  return $items;
}
