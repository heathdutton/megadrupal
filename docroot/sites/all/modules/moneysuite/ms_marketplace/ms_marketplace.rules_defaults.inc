<?php

/**
 * @file
 * Licensed under the GNU GPLv2 License
 */

/**
 * Implements hook_default_rules_configuration().
 */
function ms_marketplace_default_rules_configuration() {
  $configs = array();

  $rules_marketplace_suite_grant_commission_for_sale = '
  { "rules_marketplace_suite_grant_commission_for_sale" : {
      "LABEL" : "Marketplace Suite - Grant Commission for Sale",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Marketplace Suite" ],
      "REQUIRES" : [ "ms_marketplace", "rules" ],
      "ON" : [ "ms_marketplace_event_new_sale" ],
      "DO" : [
        { "ms_marketplace_action_calculate_commission" : {
            "USING" : {
              "account" : [ "account" ],
              "order" : [ "order" ],
              "product" : [ "product" ]
            },
            "PROVIDE" : { "commission" : { "commission" : "Commission" } }
          }
        },
        { "ms_marketplace_action_add_balance" : {
            "account" : [ "account" ],
            "order" : [ "order" ],
            "product" : [ "product" ],
            "amount" : [ "commission" ]
          }
        },
        { "mail" : {
            "to" : [ "account:mail" ],
            "subject" : "New Sale!",
            "message" : "Your product [product:name] has sold, and you have earned $[commission:value] commission. You can view your sales reports and balance from your account page here: [account:url]\/ms_marketplace\/dashboard\r\n\r\nSincerely,\r\n[site:name] Team",
            "language" : [ "" ]
          }
        }
      ]
    }
  }';

  $configs['rules_marketplace_suite_grant_commission_for_sale'] = rules_import($rules_marketplace_suite_grant_commission_for_sale);

  return $configs;
}
