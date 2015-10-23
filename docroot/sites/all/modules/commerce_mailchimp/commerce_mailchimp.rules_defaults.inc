<?php

/**
 * @file
 * Default rule configurations for Checkout.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_mailchimp_default_rules_configuration() {

  $rules = array();

  // Add a reaction rule to subscribe user to mailchimp lists of an order
  // upon checkout completion.
  $rule = rules_reaction_rule();

  $rule->label = t('Subscribe user to mailchimp lists of an order on checkout completion');
  $rule->active = TRUE;

  $rule
    ->event('commerce_checkout_complete')
    ->action('commerce_mailchimp_user_subscribe_order', array(
      'commerce_order:select' => 'commerce-order',
    ));

  $rules['commerce_checkout_commerce_mailchimp_user_subscribe_order'] = $rule;

  // Add a reaction rule to update an order information to MailChimp
  // upon checkout completion.
  $rule = rules_reaction_rule();

  $rule->label = t('Update Ecommerce Order Information to MailChimp on checkout completion');
  $rule->active = TRUE;

  $rule
    ->event('commerce_checkout_complete')
    ->action('commerce_mailchimp_ecom_update', array(
      'commerce_order:select' => 'commerce-order',
    ));

  $rules['commerce_checkout_commerce_mailchimp_ecom_update'] = $rule;

  // Add a reaction rule to delete an order information from MailChimp
  // upon  order concellation.
  $rule = rules_reaction_rule();

  $rule->label = t('Delete Ecommerce Order Information from MailChimp on order cancellation');
  $rule->active = TRUE;

  $rule
    ->event('commerce_order_update')
    ->condition('data_is', array('data:select' => 'commerce-order:state', 'value' => 'canceled'))
    ->action('commerce_mailchimp_ecom_delete', array(
      'commerce_order:select' => 'commerce-order',
    ));

  $rules['commerce_mailchimp_ecom_delete'] = $rule;

  // Add a reaction rule to update an order information to MailChimp
  // upon line item update.
  $rule = rules_reaction_rule();

  $rule->label = t('Update Ecommerce Order Information to MailChimp on line item update');
  $rule->active = TRUE;

  $rule
    ->event('commerce_line_item_update')
    ->condition('data_is', array('data:select' => 'commerce-line-item:type', 'value' => 'product'))
    ->action('commerce_mailchimp_ecom_delete', array(
      'commerce_order:select' => 'commerce-line-item:order',
    ))
    ->action('commerce_mailchimp_ecom_update', array(
      'commerce_order:select' => 'commerce-line-item:order',
    ));

  $rules['commerce_mailchimp_ecom_update'] = $rule;

  return $rules;
}
