<?php

/**
 * Implements hook_rules_action_info_alter().
 */
function bookit_option_rules_action_info_alter(&$actions) {
  $actions['bookit_price_calculate_amount']['callbacks']['execute'] = 'bookit_option_price_calculate_amount';
}
