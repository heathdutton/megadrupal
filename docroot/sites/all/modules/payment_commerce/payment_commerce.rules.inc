<?php

/**
 * @file
 * Rules integration.
 */

/**
 * Implements hook_rules_condition_info().
 */
function payment_commerce_rules_condition_info() {
  $conditions['payment_commerce_payment_method_validate'] = array(
    'label' => t('Payment method validates an order'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
        'description' => t('The order for which the payment should be validated by a payment method.'),
      ),
    ),
    'group' => t('Payment for Drupal Commerce'),
  );

  return $conditions;
}

/**
 * Implements Rules plugin callback.
 */
function payment_commerce_payment_method_validate($order, $foo, $RulesState, $RulesCondition, $operation) {
  if ($operation == 'execute') {
    $rule = $RulesCondition->parentElement()->parentElement();
    foreach ($rule->getIterator() as $child) {
      // Check if this is a rule action to enable a Payment payment method.
      if ($child instanceof RulesAction && strpos($child->getElementName(), 'commerce_payment_enable_payment_commerce_') === 0) {
        // Extract the PMID.
        $commerce_payment_method_name = $child->settings['payment_method'];
        $pmid = (int) str_replace('payment_commerce_', '', $commerce_payment_method_name);

        // Create and validate the payment for this order.
        $payment = payment_commerce_payment_create($order, $pmid);
        try {
          $payment->method->validate($payment);
          return TRUE;
        }
        catch (PaymentValidationException $e) {
          return FALSE;
        }
      }
    }
  }
}