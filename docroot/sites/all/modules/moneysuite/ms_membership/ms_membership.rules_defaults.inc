<?php
/**
 * @file
 * ms_membership.rules_defaults.inc
 */

/**
 * Define a rule to send authenticated users
 * to membership purchase page
 * whenever they have no active membership
 *
 * Implements hook_default_rules_configuration().
 */
function ms_membership_default_rules_configuration() {
  $items = array();

  if (variable_get('ms_membership_force_login', FALSE)) {
    $items['ms_membership_enforce_membership'] = entity_import('rules_config', '{ "ms_membership_enforce_membership" : {
        "LABEL" : "Enforce subscription",
        "PLUGIN" : "reaction rule",
        "OWNER" : "rules",
        "REQUIRES" : [ "rules", "ms_products" ],
        "ON" : { "node_view" : [] },
        "IF" : [
          { "data_is" : { "data" : [ "site:current-user:uid" ], "op" : "\u003E", "value" : "1" } },
          { "NOT ms_products_condition_user_has_purchase_of_bundle" : { "bundle" : "ms_membership", "account" : [ "site:current-user" ] } }
        ],
        "DO" : [ { "redirect" : { "url" : "\/membership\/list", "force" : "0" } } ]
      }
    }');
  }
  return $items;
}