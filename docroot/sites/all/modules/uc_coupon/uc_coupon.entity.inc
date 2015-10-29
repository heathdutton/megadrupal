<?php
/**
 * Load a coupon object.
 *
 * @param $cid
 *   Unique coupon ID.
 *
 * @return $coupon
 *   A coupon object.
 */
function uc_coupon_load($cid) {
  $coupon = db_query("SELECT * FROM {uc_coupons} WHERE cid = :cid", array(':cid' => $cid))->fetchObject();
  $coupon->data = isset($coupon->data) ? unserialize($coupon->data) : array();

  // Convert old coupons that could not specify "per order" when restricted.
  if (!isset($coupon->data['apply_to'])) {
    if (isset($coupon->data['max_applicable_products_value']) && isset($coupon->data['max_applicable_products']) && $coupon->data['max_applicable_products']) {
      // Coupon was restricted to X cheapest or most expensive products.
      $coupon->data['apply_to'] = $coupon->data['max_applicable_products_value'];
      $coupon->data['apply_count'] = $coupon->data['max_applicable_products'];
    }
    elseif (isset($coupon->data['products']) || isset($coupon->data['skus']) || isset($coupon->data['terms']) || isset($coupon->data['product_types'])) {
      // Coupon has product restrictions, so was applied to each matching product.
      $coupon->data['apply_to'] = 'products';
    }
    else {
      // Coupon had no product restrictions, so was applied once to the subtotal.
      $coupon->data['apply_to'] = 'subtotal';
    }
  }
  unset($coupon->data['max_applicable_products']);
  unset($coupon->data['max_applicable_products_value']);

  // Allow other modules to alter the coupon data.
  drupal_alter('uc_coupon', $coupon);

  return $coupon;
}


/**
 * Save a coupon object.
 *
 * If the 'cid' field is set, then this will update an existing coupon.
 * Otherwise, a new bulk seed will be generated, the coupon will be
 * inserted into the database, and $coupon->cid will be set.
 *
 * @param $coupon
 *   The coupon to save.
 *
 * @param $edit
 *   An optional array of extra data that other modules may need to save.
 */
function uc_coupon_save(&$coupon, $edit = array()) {
  // Allow other modules to alter the coupon before saving.
  foreach (module_implements('uc_coupon_presave') as $module) {
    $callback = $module . '_uc_coupon_presave';
    $callback($coupon, $edit);
  }

  if (isset($coupon->cid)) {
    drupal_write_record('uc_coupons', $coupon, 'cid');
  }
  else {
    $coupon->created = REQUEST_TIME;
    $coupon->bulk_seed = md5(uniqid());
    drupal_write_record('uc_coupons', $coupon);
  }

  // Notify other modules that a coupon has been saved.
  module_invoke_all('uc_coupon_save', $coupon);
}

function uc_coupon_delete($cid) {
  $coupon = uc_coupon_load($cid);

  module_invoke_all('uc_coupon_delete', $coupon);

  db_delete('uc_coupons')
    ->condition('cid', $cid)
    ->execute();
  db_delete('uc_coupons_orders')
    ->condition('cid', $cid)
    ->execute();
    
  return $coupon;
}