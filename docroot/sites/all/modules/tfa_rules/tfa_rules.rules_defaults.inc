<?php
/**
 * @file
 * Default rules for two-factor authentication.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function tfa_rules_default_rules_configuration() {
  $items = array();

  $items['rules_tfa_user_login_redirect_setup'] = entity_import('rules_config', '{ "rules_tfa_user_login_redirect_setup" : {
    "LABEL" : "User redirect to TFA setup on login",
    "PLUGIN" : "reaction rule",
    "OWNER" : "rules",
    "REQUIRES" : [ "tfa_rules", "rules" ],
    "ON" : { "user_login" : [] },
    "IF" : [ { "NOT tfa_rules_user_has_tfa" : { "account" : [ "account" ] } } ],
    "DO" : [
      { "drupal_message" : {
          "message" : "Two-factor authentication (TFA) is strongly recommended; please configure your account security.",
          "type" : "warning",
          "repeat" : "0"
        }
      },
      { "redirect" : { "url" : "\/user\/[account:uid]\/security\/tfa" } }
    ]
  }
}');

  return $items;
}
