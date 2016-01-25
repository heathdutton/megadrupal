<?php

/**
 * @file
 * Implements checkout integration for ec_charge.
 */

/**
 * Implements hook_checkout_calculate().
 */
function ec_charge_checkout_calculate(&$form_state) {
  ctools_include('export');

  $txn = & $form_state['txn'];
  $charges = array();
  $items = array();

  if (!empty($txn->misc)) {
    $txn->misc = array_filter($txn->misc, '_ec_charge_checkout_strip_charges');
  }

  $charges = ctools_export_load_object('ec_charge', 'conditions', array('enabled' => 1, 'chg_type' => 0));
  uasort($charges, 'ec_sort');

  foreach ($charges as $chg) {
    if (ec_charge_filter_charges($chg, 'txn', $txn, $items)) {
      $misc = ec_charge_create_charges($chg, 'txn', $txn, $items);

      foreach ($misc as $item) {
        if (isset($charges[$item['type']])) {
          $charges[$item['type']]++;
          $item['type'] .= '-' . $charges[$item['type']];
        }
        else {
          $charges[$item['type']] = 0;
        }
        $txn->misc[] = (object) $item;
        $items[] = (object) $item;
      }
    }
  }
}

function _ec_charge_checkout_strip_charges($a) {
  return drupal_substr($a->type, 0, 3) != 'MT-';
}

/**
 * Implementation of hook_checkout_validate().
 */
function ec_charge_checkout_validate(&$form, &$form_state) {
  ctools_include('export');

  $txn =& $form_state['txn'];
  $txn->fields_used = array();
  $items = array();
  
  if (isset($form_state['values']['fields'])) {
    $charges = ctools_export_load_object('ec_charge', 'conditions', array('enabled' => 1, 'chg_type' => 0));
    uasort($charges, 'ec_sort');
  
    $field_values = $txn->fields;
    $txn->fields = $form_state['values']['fields'];
  
    foreach ($charges as $chg) {
      if (ec_charge_filter_charges($chg, 'txn', $txn, $items)) {
        $misc = ec_charge_create_charges($chg, 'txn', $txn, $items);

        foreach ($misc as $item) {
          if (isset($charges[$item['type']])) {
            $charges[$item['type']]++;
            $item['type'] .= '-'. $charges[$item['type']];
          }
          else {
            $charges[$item['type']] = 0;
          }
          $items[] = (object)$item;
        }
        if (isset($txn->fields_accessed)) {
          $txn->fields_used = array_unique(array_merge($txn->fields_used, $txn->fields_accessed));
        }
      }
      unset($txn->fields_accessed);
    }
  
    $txn->fields = $field_values;
  }
}
