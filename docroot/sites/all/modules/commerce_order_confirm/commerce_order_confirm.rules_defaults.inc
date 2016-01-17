<?php

  /**
   * Implements hook_default_rules_configuration().
   */
function commerce_order_confirm_default_rules_configuration() {
  $rules = array();

  // Add a reaction rule to send order confirmation thank you e-mail
  // upon order confirmation completion.
  $rule = rules_reaction_rule();

  $rule->label = t('Send order confirmation email to the customer');
  $rule->active = TRUE;

  $rule
    ->event('commerce_order_confirmed')
    ->action('mail', array(
    'to:select' => 'commerce-order:mail',
    'subject' => t('Order [commerce-order:order-number] at [site:name] confirmed'),
    'message' => t("Your order [commerce-order:order-number] at [site:name] successfully confirmed. Thank you!\n\nPlease contact us if you have any questions about your order."),
    'from' => '',
    'language' => '',
  ));

  $rule->weight = 0;
  $rules['commerce_order_confirmation_email'] = $rule;

  // Add a reaction rule to send order confirmation e-mail to the site admin
  // upon order confirmation completion.
  $rule = rules_reaction_rule();

  $rule->label = t('Send order confirmation email to the administrator');
  $rule->active = TRUE;

  $rule
    ->event('commerce_order_confirmed')
    ->action('mail', array(
    'to:select' => 'site:mail',
    'subject' => t('Customer order [commerce-order:order-number] at [site:name] confirmed'),
    'message' => t("A customer at [site:name] has just confirmed an order.\n\nThe order id is [commerce-order:order-number].\nYou can view the complete order at: [site:url]/admin/commerce/orders/[commerce-order:order-id]\n"),
    'from' => '',
    'language' => '',
  ));
  $rule->weight = 2;
  $rules['order_confirmation_admin_email'] = $rule;

  return $rules;
}
