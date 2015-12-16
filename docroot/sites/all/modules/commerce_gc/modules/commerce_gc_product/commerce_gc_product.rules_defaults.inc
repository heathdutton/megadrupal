<?php

/**
 * @file
 * Provides default rules for giftcard purchasing.
 */

/*
 * Implements hook_default_rules_configuration().
 */
function commerce_gc_product_default_rules_configuration() {
  // Checkout rule for generating giftcard coupons for purchased giftcards.
  $rule = rules_reaction_rule();
  $rule->label = t('Generate giftcard for purchased giftcard product');
  $rule->tags = array('Commerce Giftcard');
  // This needs to run as late as possible so that the user-create action has
  // already gone.
  $rule->weight = 10;
  
  $rule
    ->event('commerce_gc_product_process_giftcard_line_item')
    ->action('commerce_gc_product_line_item_generate_giftcard', array(
      'commerce_line_item:select' => 'commerce-line-item',
      'giftcard_coupon:var' => 'giftcard_coupon',
      'giftcard_coupon:label' => t('Giftcard coupon')
    ))
    ->action('commerce_gc_product_giftcard_email_variables', array(
      'commerce_line_item:select' => 'commerce-line-item',
      'giftcard_mail:var' => 'giftcard_mail',
      'giftcard_mail:label' => t('Giftcard email'),
    ))
    ->action('commerce_gc_product_attach_giftcard_to_user_recipient', array(
      'mail:select' => 'giftcard-mail',
      'commerce_coupon:select' => 'giftcard-coupon',
      'email_type' => 'register_admin_created',
      'user_recipient:var' => 'user_recipient',
      'user_recipient:label' => t('User recipient')        
    ))
    ->action('mail', array(
      'to:select' => 'giftcard-mail',
      'from' => '',
      'subject' =>  t('You have received a gift certificate at [site:name]'),
      'message' => t('You have received a gift certificate to use at [site:url]. Please use the following code to redeem your gift: [giftcard-coupon:code].

[commerce-line-item:giftcard-message-text]The balance of your giftcard is [giftcard-coupon:giftcard-balance]. When logged in, you can track the balance of this certificate here: [user-recipient:giftcards-url].')
    ))
    ->action('drupal_message', array(
       'message' => t('Created giftcard [giftcard-coupon:code].'),
       'type' => 'status'
    ));
  
  $rules['commerce_gc_product_default_process_gift_card_line_item'] = $rule;
  
  return $rules;
}
