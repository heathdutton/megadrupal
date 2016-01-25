<?php

/**
 * @file
 * Provide taxonomy intergration for charges to allow low level actions
 * to be done one changes
 */

/**
 * Implementation of hook_ec_charge_filter_info().
 */
function taxonomy_ec_charge_filter_info() {
  return array(
    'term' => array(
      'name' => t('Term Ids'),
      'description' => t('Validate that one of the products being purchased have the following term ids'),
      'module' => 'taxonomy_term',
      'file' => 'taxonomy.inc',
      'core' => TRUE,
    ),
  );
}

function taxonomy_ec_charge_variable_info() {
  return array(
    'term' => array(
      'name' => t('Term Ids'),
      'description' => t('Provide the value of all the products with the selected term ids'),
      'module' => 'taxonomy_term',
      'file' => 'taxonomy.inc',
      'core' => TRUE,
    ),
  );
}

function taxonomy_term_filter_form(&$form_state, $settings) {
  $form = array();

  $default = array('tids' => '');
  $settings += $default;

  $form['tids'] = array(
    '#type' => 'textfield',
    '#title' => t('Term ids'),
    '#default_value' => $settings['tids'],
    '#description' => t('Enter in a comma separated list of tids which will used to filter products.'),
    '#required' => TRUE,
  );

  return $form;
}

function taxonomy_term_filter_validate(&$form, &$form_state) {
  $tids = explode(',', $form_state['values']['tids']);

  foreach ($tids as $tid) {
    if (!$term = taxonomy_term_load($tid)) {
      form_set_error('tids', t('Term ids %tid is not found', array('%tid' => $tid)));
    }
  }
}

function taxonomy_term_filter_settings() {
  return array('tids');
}

function taxonomy_term_filter_process($type, $settings, $object) {
  $$type = & $object;

  $tids = explode(',', $settings['tids']);
  switch ($type) {
    case 'txn':
      $products = array_keys($txn->items);

      // FIXME: I Need to be checked as I am not sure if this is correct.
      $tid = db_select('node', 'n')
        ->join('taxonomy_term_node', 't', 'n.nid = t.nid')
        ->fields('t.tid')
        ->condition('n.nid', $products, 'IN')
        ->condition('t.tid', $tids, 'IN')
        ->execute()
        ->fetchCol();
      return $tid ? TRUE : FALSE;
      break;

    case 'node':
      $ids = array_keys($node->taxonomy);
      $tid = array_intersect($tids, $ids);
      return empty($tid) ? FALSE : TRUE;
      break;
  }
}

function taxonomy_term_variable_form(&$form_state, $settings) {
  $form = array();

  $default = array('tids' => '');
  $settings += $default;

  $form['tids'] = array(
    '#type' => 'textfield',
    '#title' => t('Term ids'),
    '#default_value' => $settings['tids'],
    '#description' => t('Enter in a comma separated list of tids which will used to select the products which make up the variable.'),
    '#required' => TRUE,
  );

  return $form;
}

function taxonomy_term_variable_validate(&$form, &$form_state) {
  $tids = explode(',', $form_state['values']['tids']);

  foreach ($tids as $tid) {
    if (!$term = taxonomy_term_load($tid)) {
      form_set_error('tids', t('Term ids %tid is not found', array('%tid' => $tid)));
    }
  }
}

function taxonomy_term_variable_settings() {
  return array('tids');
}

function taxonomy_term_variable_description($type, $settings, $object, $variables) {
  return array(
    'total' => t('Summation of products price with the terms %tids', array('%tids' => $settings['tids'])),
    'qty' => t('Summation of products quantities with the terms %tids', array('%tids' => $settings['tids'])),
  );
}

function taxonomy_term_variable_process($type, $settings, $object) {
  $$type = & $object;
  $qty = $value = 0;

  if (!$object) {
    return;
  }

  switch ($type) {
    case 'txn':
      $txn = clone $txn;

      $tids = explode(',', $settings['tids']);
      $products = array_keys($txn->items);

      // FIXME: I Need to be checked as I am not sure if this is correct.
      $result = db_select('node', 'n')
        ->join('taxonomy_term_node', 't', 'n.nid = t.nid')
        ->fields('n.nid')
        ->distinct()
        ->condition('n.nid', $products, 'IN')
        ->condition('t.tid', $tids, 'IN')
        ->execute();

      foreach ($result as $node) {
        if (isset($txn->items[$node->nid])) {
          $item = $txn->items[$node->nid];
          $value += ec_store_adjust_misc($txn, $item) * (ec_product_has_quantity($item) && isset($item->qty) ? $item->qty : 1);
          $qty += (ec_product_has_quantity($item) && isset($item->qty) ? $item->qty : 1);
        }

      }
      break;

    case 'node':
      $tids = array_keys($node->taxonomy);
      $tid = array_intersect($settings['tids'], $tids);

      if (!empty($tid)) {
        $qty += ec_product_has_quantity($node) && isset($node->qty) ? $node->qty : 1;
        $value += ec_product_get_final_price($node, 'product') * (ec_product_has_quantity($node) && isset($node->qty) ? $node->qty : 1);
      }

      break;
  }

  return array('total' => $value, 'qty' => $qty);
}

