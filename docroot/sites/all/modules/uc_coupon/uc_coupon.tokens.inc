<?php
/**
 * @file
 * Token support for uc_coupon.
 */

/**
 * Implements hook_token_info().
 */
function uc_coupon_token_info() {
  $types = array(
    'uc_coupon' => array(
      'name' => t('Coupon'),
      'description' => t('Tokens related to Ubercart discount coupons.'),
      'needs-data' => 'uc_coupon',
    ),
  );

  $tokens = array(
    'uc_coupon' => array(  
      'name' => array(
        'name' => t('Name'),
        'description' => t('The name of the coupon.'),
      ),
      'code' => array(
        'name' => t('Code'),
        'description' => t('The coupon code. For bulk coupons this will be the actual code applied, or the code 
        	prefix if the coupon has not yet been applied.'),
      ),
      'codes' => array(
        'name' => t('All Codes'),
        'description' => t('The coupon codes.  For bulk coupons, this will be a comma separated list of codes.'),
      ),
      'value' => array(
        'name' => t('Value'),
        'description' => t('The value of the coupon.  If the coupon has already been applied to an order, this will be
        	the actual currency value of the coupon.  Otherwise, it will be the nominal discount.'),
      ),
    ),
  
    'uc_order' => array(
      'coupon-code' => array(
        'name' => t('Coupon Code'),
        'description' => t('The coupon code used in the order.'),
      ),
    ),
  );

  return array('types' => $types, 'tokens' => $tokens);
}

/**
 * Implements hook_tokens().
 */
function uc_coupon_tokens($type, $tokens, $data = array(), $options = array()) {
  $sanitize = !empty($options['sanitize']);
  $values = array();

  switch ($type) {
    case 'uc_order':
      if (array_key_exists('coupon-code', $tokens) && !empty($data['uc_order']) && isset($data['uc_order']->data['coupon'])) {
        $values[$tokens['coupon-code']] = $sanitize ? check_plain($data['uc_order']->data['coupon']) : $data['uc_order']->data['coupon'];
      }
      break;

    case 'uc_coupon':
      if (!empty($data['uc_coupon'])) {
        $coupon =  $data['uc_coupon'];
        if (array_key_exists('name', $tokens)) {
          $values[$tokens['name']] = $sanitize ? check_plain($coupon->name) : $coupon->name;
        }
        if (array_key_exists('code', $tokens)) {
          $values[$tokens['code']] = $sanitize ? check_plain($coupon->code) : $coupon->code;
        }
        if (array_key_exists('codes', $tokens)) {
          $codes = array();
          if ($coupon->bulk) {
            for ($id = 0; $id < $coupon->data['bulk_number']; $id++) {
              $codes[] = uc_coupon_get_bulk_code($coupon, $id);
            }
          }
          else {
            $codes[] =  $coupon->code;
          }
          $values[$tokens['codes']] = implode("\n", $sanitize ? array_walk($codes, 'check_plain') : $codes);
        }
        if (array_key_exists('value', $tokens)) {
          $values[$tokens['value']] = isset($coupon->amount) ?
            theme('uc_price', array('price' => $coupon->amount)) : theme('uc_coupon_discount', array('coupon' => $coupon));
        }
      }
      break;
  }

  return $values;
}
