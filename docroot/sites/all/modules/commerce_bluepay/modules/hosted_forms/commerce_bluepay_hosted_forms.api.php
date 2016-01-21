<?php
/**
 * @file
 * API Documentation
 */

/**
 * Allow for the altering of iframe attributes before rendering.
 *
 * @param array $attributes
 *   An array of attributes that will be used to build the <iframe> tag.
 *
 * @param array $payment_method
 *   The payment method configuration array.
 *
 * @param object $order
 *   The commerce order object.
 */
function HOOK_commerce_bluepay_hosted_forms_iframe_attributes_alter(&$attributes, $payment_method, $order) {
  $attributes['width'] = '500px';
  $attributes['height'] = '500px';
}
