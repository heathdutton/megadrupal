<?php

/**
 * @file
 * Provide checkout functionality to ec_checkout module.
 */

/**
 * Implements hook_checkout_form().
 */
function ec_checkout_checkout_form(&$form, &$form_state) {
  $txn = & $form_state['txn'];

  $txn->subtotal = 0;
  $txn->gross = $txn->balance = ec_store_transaction_calc_gross($txn);

  if (isset($txn->misc) && is_array($txn->misc)) {
    usort($txn->misc, 'ec_sort');
  }

  $form['ec_checkout_review']['items'] = array(
    '#tree' => TRUE,
  );

  foreach ((array) $txn->items as $product) {
    $price = ec_store_adjust_misc($txn, $product);
    $node = node_load($product->nid);
    $subtotal = ec_product_has_quantity($node) ? ($price * $product->qty) : $price;

    $form['ec_checkout_review']['items'][$product->nid] = array(
      '#node' => $product, // allow more info to be displayed by the theme
      'title' => array(
        '#markup' => $product->title,
      ),
      'qty' => ec_product_has_quantity($node) ?
        array(
        '#type' => 'textfield',
        '#default_value' => $product->qty,
        '#size' => 6,
      ) :
        array(
        '#value' => '',
      ),
      'price' => array(
        '#markup' => format_currency($price),
      ),
      'subtotal' => array(
        '#markup' => format_currency($subtotal),
      ),
      'options' => array(
        '#markup' => $txn->type == 'ec_cart' ? l(t('Change'), 'cart/view') : '',
      ),
    );

    $txn->subtotal += $subtotal;
  }

  $form['ec_checkout_review']['totals'] = array();

  // Since we may remove items, keep a track of the current key.
  $line = 0;

  // Create a subtotal line.
  // If the Total comes immediately afterwards, then it will be repressed later.
  $form['ec_checkout_review']['totals'][++$line] = array(
    'title' => array('#markup' => t('Sub-total')),
    'amount' => array('#markup' => format_currency($txn->subtotal)),
    '#attributes' => array(
      'class' => array('ec-checkout-review-form-row-subtotal'),
    ),
  );

  if (!empty($txn->misc)) {
    uasort($txn->misc, 'ec_sort');
    foreach ($txn->misc as $key => $misc) {
      if (empty($txn->misc[$key]->seen)) {
        // Only add the subtotal line if there are other items in misc.
        if (!empty($misc->subtotal_before) && ($form['ec_checkout_review']['totals'][$line -1]['info']['#row_type'] != $st['#row_type'])) {
          $form['ec_checkout_review']['totals'][++$line] = array(
            'title' => array('#markup' => t('Sub-total')),
            'value' => array('#markup' => format_currency($txn->subtotal)),
            '#attributes' => array(
              'class' => array('ec-checkout-review-form-row-subtotal'),
            ),
          );
        }
      }

      // Here we calculate the misc item.
      if (!empty($misc->callback) and function_exists($misc->callback)) {
        $f = $misc->callback;
        $amount = $f($txn, $misc, $txn->subtotal);
        // Apply the total of this charge to the transaction object for saving
        // later.
        $txn->misc[$key]->price = $amount;
      }
      elseif (!empty($misc->qty)) {
        $amount = ($misc->price * $misc->qty);
      }
      else {
        $amount = $misc->price;
      }
      if (empty($misc->already_added)) {
        $txn->subtotal += $amount;
      }

      if (empty($txn->misc[$key]->seen)) {
        // Misc Item goes here.
        $form['ec_checkout_review']['totals'][++$line] = array(
          'title' => array('#markup' => $misc->description),
          'amount' => array('#markup' => format_currency($amount)),
          '#attributes' => array(
            'class' => array('ec-checkout-review-form-row-charge'),
          ),
        );

        // Subtotal - check straight after the misc item.
        if (!empty($misc->subtotal_after)) {
          $form['ec_checkout_review']['totals'][++$line] = array(
            'title' => array('#markup' => t('Sub-total')),
            'amount' => array('#markup' => format_currency($txn->subtotal)),
            '#attributes' => array(
              'class' => array('ec-checkout-review-form-row-subtotal'),
            ),
          );
        }
      }
    }
  }

  // Remove any subtotal occurring before the total.
  if (!empty($form['ec_checkout_review']['totals'][$line]['#attributes']['class']) && in_array('ec-checkout-review-form-row-subtotal', $form['ec_checkout_review']['totals'][$line]['#attributes']['class'])) {
    unset($form['ec_checkout_review']['totals'][$line]);
  }
  // Grand total
  $form['ec_checkout_review']['totals'][] = array(
    'title' => array('#markup' => t('Total')),
    'amount' => array('#markup' => format_currency($txn->gross)),
    '#attributes' => array(
      'class' => array('ec-checkout-review-form-row-total'),
    ),
  );
  $form['ec_checkout_review']['#theme'] = 'ec_checkout_checkout_review_form';
}

/**
 * Implements hook_checkout_update().
 */
function ec_checkout_checkout_update(&$form, &$form_state) {
  $txn = & $form_state['txn'];

  if (isset($form_state['values']['items'])) {
    foreach ($form_state['values']['items'] as $nid => $item) {
      if (ec_product_has_quantity($txn->items[$nid])) {
        $txn->items[$nid]->qty = $item['qty'];
      }
    }
  }
}
