<?php

/**
 * @file
 * Default rule configurations for User addressbook commerce.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function user_addressbook_commerce_default_rules_configuration() {
  $items = array();
  $items['rules_rules_user_addressbook_commerce_select_user_address'] = entity_import('rules_config', '{ "rules_rules_user_addressbook_commerce_select_user_address" : {
      "LABEL" : "User addressbook commerce - Select User address for Customer profile",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "user_addressbook_commerce" ],
      "ON" : { "user_addressbook_commerce_user_address_select" : [] },
      "IF" : [
        { "entity_has_field" : { "entity" : [ "user-address" ], "field" : "user_addressbook_address" } },
        { "entity_has_field" : {
            "entity" : [ "commerce-customer-profile" ],
            "field" : "commerce_customer_address"
          }
        }
      ],
      "DO" : [
        { "data_set" : {
            "data" : [ "commerce-customer-profile:commerce-customer-address" ],
            "value" : [ "user-address:user-addressbook-address" ]
          }
        }
      ]
    }
  }');
  $items['rules_user_addressbook_commerce_add_new_customer_profile'] = entity_import('rules_config', '{ "rules_user_addressbook_commerce_add_new_customer_profile" : {
      "LABEL" : "User addressbook commerce - Save User address from Customer profile",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "user_addressbook_commerce" ],
      "ON" : { "user_addressbook_commerce_add_new_customer_profile" : [] },
      "IF" : [
        { "entity_has_field" : {
            "entity" : [ "commerce-customer-profile" ],
            "field" : "commerce_customer_address"
          }
        },
        { "entity_has_field" : { "entity" : [ "user-address" ], "field" : "user_addressbook_address" } }
      ],
      "DO" : [
        { "data_set" : {
            "data" : [ "user-address:user-addressbook-address" ],
            "value" : [ "commerce-customer-profile:commerce-customer-address" ]
          }
        }
      ]
    }
  }');
  return $items;
}
