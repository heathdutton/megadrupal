<?php

/**
 * @file
 * Documents hooks provided by the Commerce 4B module.
 */


/**
 * Let other modules load a different Commerce 4B rule.
 *
 * @param $rule_name
 *   The machine name of the rule to be loaded.
 */
function hook_commerce_4b_rule_alter(&$rule_name) {
  // The default rule provided by the module
  $rule_name = 'commerce_payment_commerce_4b';
}
