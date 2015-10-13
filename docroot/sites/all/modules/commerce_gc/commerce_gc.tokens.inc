<?php

/**
 * @file
 * Provides token support for Giftcards.
 */

/*
 * Implements hook_token_info().
 */
function commerce_gc_token_info_alter(&$info) {
  $info['tokens']['commerce-coupon']['giftcard-balance'] = array(
    'name' => t('Giftcard balance'),
    'description' => t('The amount currently available on this giftcard')
  );
  $info['tokens']['user']['giftcards-url'] = array(
    'name' => t('Giftcards url'),
    'description' => t('The URL of the account giftcards page.')
  );
}

/*
 * Implements hook_tokens().
 */
function commerce_gc_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();    

  foreach ($tokens as $name => $original) {
    // Add a token for giftcard balance.
    if ($type == 'commerce-coupon' && $name == 'giftcard-balance' && !empty($data['commerce-coupon'])) {
      $coupon = $data['commerce-coupon'];
      $amount = commerce_gc_giftcard_balance($coupon->coupon_id);
      $replacements[$original] = commerce_currency_format($amount, commerce_default_currency());
    }

    // Add giftcards url token.
    if ($type == 'user' && $name == 'giftcards-url') {
      $url_options = array('absolute' => TRUE);
      $user = $data['user'];    
      $replacements[$original] = $user->uid ? url('user/' . $user->uid . '/giftcards', $url_options) : '';    
    }      
  }
  return $replacements;      
}
