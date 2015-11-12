<?php

/**
 * @file
 * Custom rules validation handler.
 */

/**
 * Implements hook_rules_condition_info().
 */
function ideal_advanced_commerce_rules_condition_info() {
  return array(
    'ideal_advanced_commerce_config_validate' => array(
      'label' => t('Validate an iDEAL configuration'),
      'group' => t('Commerce Payment'),
      'parameter' => array(
        'commerce_order' => array(
          'type' => 'commerce_order',
          'label' => 'Commerce order',
          'description' => t('Validate the ideal configurations for this order.'),
        ),
      ),
    ),
  );
}

/**
 * Validate the ideal configuration. On error disable the payment method.
 *
 * @param object $order
 *   Commerce order object.
 * @param array $settings
 *   Settings from the rules condition.
 * @param RulesState $rules_state
 *   Rules state object.
 * @param RulesCondition $rules_condition
 *   Rules conditions and settings.
 *
 * @return bool True on success.
 *   True on success.
 */
function ideal_advanced_commerce_config_validate($order, array $settings, RulesState $rules_state, RulesCondition $rules_condition) {
  $valid = FALSE;
  $rule_root = $rules_condition->parentElement()->parentElement();

  foreach ($rule_root->getIterator() as $rule_element) {
    if (get_class($rule_element) == 'RulesAction' && isset($rule_element->settings['payment_method']['settings']['ideal_configuration'])) {
      $config_handler = new IdealAdvancedConfigHandler();
      $valid = $config_handler->configValidate($rule_element->settings['payment_method']['settings']['ideal_configuration']);
    }
    elseif (get_class($rule_element) == 'RulesAction') {
      watchdog('iDEAL', 'Could not load payment method. No ideal configuration is selected.', array(), WATCHDOG_ERROR);
    }
  }

  return $valid;
}
