<?php
/**
 * @file
 * Defines default example rule for Ogone.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_vantiv_default_rules_configuration() {
  // Reaction rule that will create recurring entity for transactions that were captured.
  if (module_exists('commerce_recurring')) {
    $rule = rules_reaction_rule();
    $rule->label = t('Create recurring entity for transaction that were captured.');
    $rule->tags = array('Commerce recurring');
    $rule->active = TRUE;
    $rule
      ->event('commerce_payment_transaction_insert')
      ->condition(rules_condition('commerce_recurring_order_contains_recurring_product', array(
           'commerce_order:select' => 'commerce-payment-transaction:order',
        )))
      ->condition(rules_condition('data_is', array(
           'data:select' => 'commerce-payment-transaction:status',
           'value' => 'CAPTURE',
        )))
      ->action('entity_fetch', array(
        'type' => 'commerce_payment_transaction',
        'property' => 'transaction_id',
        'id:select' => 'commerce-payment-transaction:transaction-id',
        //'value:select' => 'commerce-payment-transaction:transaction-id',
        'limit' => '1',
        'entity_fetched:label' => 'transaction',
        'entity_fetched:var' => 'transaction',
      ))
      ->action('entity_fetch', array(
        'type' => 'commerce_order',
        'property' => 'order_id',
        'id:select' => 'transaction:order-id',
        //'value:select' => 'transaction:0:order-id',
        'limit' => '1',
        'entity_fetched:label' => 'order',
        'entity_fetched:var' => 'order',
      ))
    ->action('drupal_message', array(
       'message' => 'Recurring for that order: [commerce-payment-transaction:order-id] was created successfully.',
    ));
    $loop = rules_loop(array(
      'list:select' => 'commerce-payment-transaction:order:commerce-line-items',
      'item:label' => t('Current line item'),
      'item:var' => 'current_line_item',
    ));

    $loop->action('commerce_recurring_generate_recurring_product', array(
      'commerce_order:select' => 'order',
      'commerce_line_item:select' => 'current-line-item',
      'fixed_price:select' => 'current-line-item:commerce-unit-price',
      'quantity:select' => 'current-line-item:quantity',
    ));

    $rule->action($loop);

    $rules['create_recurring_entity_for_transaction_that_were_captured'] = $rule;
  }
  // Reaction rule triggered on cron run that captures pending transactions.
  $rule = rules_reaction_rule();
  $rule->label = t('Capture transactions on cron run.');
  $rule->tags = array('Commerce Order');
  $rule->active = TRUE;
  $rule
    ->event('cron')
    ->action('batch_capture', array(
      'date' => 'now',
    ));

  $rules['commerce_vantiv_capture_transactions_on_cron_run'] = $rule;

  return $rules;

}