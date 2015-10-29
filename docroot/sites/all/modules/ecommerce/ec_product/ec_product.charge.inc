<?php

/**
 * @file
 * Provides charge rules for ec_product module.
 */

/**
 * Implements hook_filter_process().
 */
function ec_product_shippable_filter_process($type, $settings, $object) {
  $$type = & $object;

  switch ($type) {
    case 'txn':
      return $txn->shippable ? TRUE : FALSE;
      break;

    case 'node':
      return ec_product_is_shippable($node->vid);
      break;
  }
}

/**
 * Implements hook_variable_process().
 */
function ec_product_shippable_variable_process($type, $settings, $object) {
  $$type = & $object;
  $qty = $value = 0;

  if (!$object) {
    return;
  }

  switch ($type) {
    case 'txn':
      $txn = clone $txn;

      $items = array_filter($txn->items, '_ec_product_shippable_filter');

      if (!empty($items)) {
        foreach ($items as $item) {
          $value += ec_store_adjust_misc($txn, $item) * (ec_product_has_quantity($item) ? $item->qty : 1);
          $qty += (ec_product_has_quantity($item) ? $item->qty : 1);
        }
      }
      break;

    case 'node':
      if (ec_product_is_shippable($node)) {
        $qty = ec_product_has_quantity($node) && isset($node->qty) ? $node->qty : 1;
        $value = ec_product_get_final_price($node, 'product') * $qty;
      }
      break;
  }

  return array('total' => $value, 'qty' => $qty);
}

function _ec_product_shippable_filter($a) {
  return $a->shippable;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_product_sold_filter_form(&$form_state, $settings = array()) {
  $form = array();
  $settings += array(
    'operator' => NULL,
    'value' => '',
  );

  $form['operator'] = array(
    '#type' => 'select',
    '#title' => t('Operator'),
    '#default_value' => $settings['operator'],
    '#options' => array(
      '==' => t('Equal'),
      '<' => t('Less than'),
      '<=' => t('Less than equal'),
      '>' => t('Greater than'),
      '>=' => t('Greater than equal'),
    ),
  );

  $form['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Number'),
    '#default_value' => $settings['value'],
    '#element_validate' => array('valid_number'),
    '#required' => TRUE,
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_product_sold_filter_settings() {
  return array('operator', 'value');
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_product_sold_filter_process($type, $settings, $object) {
  $nodes = & drupal_static(__FUNCTION__ . '_nodes', array());

  $$type = & $object;

  switch ($type) {
    case 'node':
      if (!isset($nodes[$node->nid])) {
        $nodes[$node->nid] = db_query('SELECT SUM(qty) FROM {ec_transaction_product} WHERE nid = :nid', array(':nid' => $node->nid))->fetchField();
      }

      return @eval('return $nodes[$node->nid] ' . $settings['operator'] . ' ' . $settings['value'] . ';');
      break;

    case 'txn':
      // this will not work on transaction as which node do you choose.
      break;
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_product_shippable_variable_description($type, $settings, $object, $variables) {
  return array(
    'total' => t('The value of all shippable products'),
    'qty' => t('The quantity of all the shippable products.'),
  );
}
