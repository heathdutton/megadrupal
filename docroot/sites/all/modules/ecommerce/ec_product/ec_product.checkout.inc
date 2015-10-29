<?php

/**
 * @file
 * Provide checkout functions for ec_product module.
 */

/**
 * Implements hook_checkout_init().
 *
 * Determine if any products are shippable
 * Apply any price adjustments
 * Do any final setup for product purchase
 *
 */
function ec_product_checkout_init(&$txn) {
  if (!isset($txn->items)) {
    $txn->items = array();
  }
  $txn->shippable = 0;
  foreach ($txn->items as $nid => $product) {
    if (ec_product_is_shippable($product->vid)) {
      $txn->shippable = 1;
      $txn->items[$nid]->shippable = 1;
    }
    $txn->items[$nid]->price = ec_product_price_adjust($product, 'ec_checkout', $txn);

    foreach (ec_product_get_all_functions($txn->items[$nid], 'product_checkout_init') as $function) {
      $function($txn, $txn->items[$nid]);
    }
  }
}

/**
 * Implements hook_checkout_calculate().
 *
 * do any last minute price adjustments
 *
 */
function ec_product_checkout_calculate(&$form_state) {
  $txn = & $form_state['txn'];

  if (!empty($txn->misc)) {
    $txn->misc = array_filter($txn->misc, '_ec_product_checkout_strip_charges');
  }
  // process any specials and add them to misc
  foreach ($txn->items as $item) {
    $functions = ec_product_get_all_functions($item, 'product_calculate');
    foreach ($functions as $function) {
      $function($txn, $item);
    }

    foreach (ec_product_specials_get($item, 'checkout', FALSE, $txn) as $type => $special) {
      if (!is_array($special)) {
        $special = array('price' => $special);
      }
      $misc = new stdClass;
      $misc->type = 'special_' . (isset($special['type']) ? $special['type'] : $type);
      $misc->vid = $item->vid;
      $misc->description = isset($special['description']) ? $special['description'] : 'special';
      $misc->invisible = isset($special['invisible']) ? $special['invisible'] : 1;
      $misc->price = $special['price'];
      $misc->qty = ec_product_has_quantity($item) ? ($special['qty'] ? $special['qty'] : $item->qty) : 1;
      $txn->misc[] = $misc;
    }

    $misc = module_invoke('ec_charge', 'product_charges', $item, 'checkout');
    if ($misc) {
      foreach ($misc as $charge) {
        $charge->type = 'PC-' . drupal_substr($charge->type, 3);
        $txn->misc[] = (object) $charge;
      }
    }
  }
}

function _ec_product_checkout_strip_charges($a) {
  return drupal_substr($a->type, 0, 3) != 'PC-' && drupal_substr($a->type, 0, 8) != 'special_';
}

/**
 * Implements hook_checkout_post_submit().
 *
 * The payment has been made at this point
 * invokes productapi and "product feature" checkout_post_submit hooks
 *
 */
function ec_product_checkout_post_submit(&$txn) {
  $ret = NULL;
  if (!empty($txn->items)) {
    foreach ($txn->items as $item) {
      $goto = ec_product_invoke_productapi($item, 'checkout_post_submit', $txn);
      if ($goto) {
        $ret = $goto;
      }
      if ($features = ec_product_get_types('features', $item)) {
        foreach ($features as $feature) {
          $goto = ec_product_invoke_feature($feature->ftype, $item, 'checkout_post_submit', $txn, $ret);
          if ($goto) {
            $ret = $goto;
          }
        }
      }
    }
  }

  if ($ret) {
    return $ret;
  }
}

