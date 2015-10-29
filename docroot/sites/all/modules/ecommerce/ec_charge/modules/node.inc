<?php
/**
 * @file
 * Provide node based filters, variables, and calcs for node.
 */

/**
 * Implementation of hook_ec_charge_filter_info().
 */
function node_ec_charge_filter_info() {
  return array(
    'nid' => array(
      'name' => t('Node Id'),
      'description' => t('Filter transactions depending if the nid is one of the products purchased.'),
      'module' => 'node_nid',
      'file' => 'node.inc',
      'core' => TRUE,
    ),
  );
}

function node_nid_filter_form(&$form_state, $settings) {
  $form = array();

  $default = array('nids' => '');
  $settings += $default;

  $form['nids'] = array(
    '#type' => 'textfield',
    '#title' => t('Node ids'),
    '#default_value' => $settings['nids'],
    '#description' => t('Enter in a comma separated list of nid which will when they are a product will be selected.'),
    '#required_external' => TRUE,
  );

  return $form;
}

function node_nid_filter_validate(&$form, &$form_state) {
  if (!empty($form_state['values']['nids'])) {
    $nids = explode(',', $form_state['values']['nids']);

    foreach ($nids as $nid) {
      if ($node = node_load($nid)) {
        if (!isset($node->ptype)) {
          form_set_error('nids', t('Node [%nid] %title is not a product', array('%nid' => $nid, '%title' => $node->title)));
        }
      }
      else {
        form_set_error('nids', t('Node ids %nid is not found', array('%nid' => $nid)));
      }
    }
  }
  elseif ($form_state['clicked_button']['#title'] != t('Cancel')) {
    form_set_error('nids', t('!name field is required.', array('!name' => $form['nids']['#title'])));
  }
}

function node_nid_filter_settings() {
  return array('nids');
}

function node_nid_filter_process($type, $settings, $object) {
  $$type = & $object;
  $nids = explode(',', $settings['nids']);

  switch ($type) {
    case 'txn':
      $products = array_keys($txn->items);
      break;

    case 'node':
      $products = array($node->nid);
      break;
  }

  foreach ($nids as $nid) {
    if (in_array($nid, $products)) {
      return TRUE;
    }
  }

  return FALSE;
}

