<?php
/**
 * @file
 * Default rule configurations for Order FTP Export.
 */

/**
 * Implements hook_default_rules_configuration().
 *
 * adds a rule
 *   to export/upload an order and set status to 'processing'
 *   when checkout is complete
 */
function commerce_order_ftp_export_default_rules_configuration() {
  $rules = array();

  // Add a reaction rule to upload an order
  // when checkout is complete and update status to 'pending'
  $rule = rules_reaction_rule();

  $rule->label = t('Upload an order when checkout is complete');
  $rule->active = TRUE;
  $rule->weight = 10; //make sure this happens after everything else

  $rule
    ->event('commerce_checkout_complete')
    ->action('commerce_order_ftp_export_upload', array(
      'commerce_order:select' => 'commerce-order',
    ));

  $rules['commerce_order_ftp_export_upload'] = $rule;

  return $rules;
}
