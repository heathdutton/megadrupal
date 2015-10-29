<?php
/**
 * @file
 * Default Rules configurations for uc_coupon_purchase.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function uc_coupon_purchase_default_rules_configuration() {
  $rule = rules_reaction_rule();
  $rule->label = t('Notify customer when a single coupon is purchased.');
  $rule->active = TRUE;
  $rule->event('uc_coupon_purchase')
    ->condition(rules_condition('uc_coupon_condition_is_bulk', array('coupon:select' => 'coupon'))->negate())
    ->action('uc_coupon_purchase_email', array(
      'order:select' => 'order',
      'coupon:select' => 'coupon',
      'from' => uc_store_email_from(),
      'addresses' => '[order:email]',
      'subject' => t('[store:name]: coupon purchase'),
      'message' => t("[order:first-name] [order:last-name], \n\nThanks to your order, [order:link], at [store:name] you now have a new coupon available to use with the code [coupon:codes].\n\nThanks again, \n\n[store:name]\n[site:slogan]"),
      'format' => filter_default_format(),
    ));

  $configs['uc_coupon_purchase_single'] = $rule;

  $rule = rules_reaction_rule();
  $rule->label = t('Notify customer when a bulk coupon is purchased.');
  $rule->active = TRUE;
  $rule->event('uc_coupon_purchase')
    ->condition(rules_condition('uc_coupon_condition_is_bulk', array('coupon:select' => 'coupon')))
    ->action('uc_coupon_purchase_email', array(
      'order:select' => 'order',
      'coupon:select' => 'coupon',
      'from' => uc_store_email_from(),
      'addresses' => '[order:email]',
      'subject' => t('[store:name]: bulk coupon purchase'),
      'message' => t("[order:first-name] [order:last-name], \n\nThanks to your order, [order:link], at [store:name] you now have a new set of coupons available to use with the following codes:\n\n[coupon:codes]\n\nThanks again, \n\n[store:name]\n[site:slogan]"),
      'format' => filter_default_format(),
    ));

  $configs['uc_coupon_purchase_bulk'] = $rule;
  
  return $configs;
}