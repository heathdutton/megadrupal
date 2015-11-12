<?php
/**
 * @file
 * Volume Pricing administration pages.
 */

/**
 * Implements hook_admin().
 * 
 * Store-wide uc_volume-pricing settings
 */
function uc_volume_pricing_admin() {
  $form = array();

  $form['uc_volume_pricing_show_in_cart'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show possible savings message in cart review.'),
    '#default_value' => variable_get('uc_volume_pricing_show_in_cart', TRUE),
  );

  $form['uc_volume_pricing_cart_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Savings message.'),
    '#default_value' => variable_get('uc_volume_pricing_cart_message', 'Purchase [remaining] more <strong>[product_title]</strong> and save an additional [savings] per item!'),
    '#description' => t('Available tokens are: [remaining], [product_title] and [savings]'),
  );  
  
  return system_settings_form($form);
}
