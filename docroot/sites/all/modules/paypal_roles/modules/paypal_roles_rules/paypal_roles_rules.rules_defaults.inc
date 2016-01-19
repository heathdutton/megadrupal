<?php
/**
 * @file
 * Default rule configurations for PayPal Roles Rules.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function paypal_roles_rules_default_rules_configuration() {
  $rules = array();

  $rule = '{ "rules_apply_tax" : {
    "LABEL" : "Apply Tax",
    "PLUGIN" : "reaction rule",
    "OWNER" : "rules",
    "TAGS" : [ "Paypal Roles Tax" ],
    "REQUIRES" : [ "paypal_roles", "paypal_roles_rules" ],
    "ON" : { "paypal_roles_memberships_alter" : [] },
    "DO" : [
        { "paypal_roles_rules_action_calculate_tax" : {
            "membership" : [ "membership" ],
            "tax_amount" : "0",
            "calculation_method" : "add",
            "tax_name" : "VAT"
            }}
        ]
    }}';

  $rules['rules_apply_tax'] = rules_import($rule);

  return $rules;
}
