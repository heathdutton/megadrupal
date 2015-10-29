<?php

/**
 * @file
 * Charge functions for ec_store.
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_misc_group_filter_form(&$form_state, $settings = array()) {
  $form = array();
  $settings += array('group' => '');

  $form['group'] = array(
    '#type' => 'textfield',
    '#title' => t('Charge group'),
    '#default_value' => $settings['group'],
    '#required' => TRUE,
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_misc_group_filter_settings() {
  return array('group');
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_misc_group_filter_process($type, $settings, $object, $charges = array()) {
  $$type = & $object;

  switch ($type) {
    case 'txn':
      if (!empty($txn->misc)) {
        $charges = $txn->misc;
      }
      break;
  }

  ec_store_filter_misc(array('misc_group' => $settings['group']), 'init');

  $charges = array_filter($charges, 'ec_store_filter_misc');

  return !empty($charges);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_txn_type_filter_form($form_state, $settings = array()) {
  $settings += array(
    'txn_type' => array(),
  );

  $options = ec_store_transaction_types();

  $form['txn_type'] = array(
    '#type' => 'select',
    '#title' => t('Transaction type'),
    '#default_value' => $settings['txn_type'],
    '#options' => $options,
    '#description' => t('Select type of transactions to allow.'),
    '#required' => TRUE,
    '#multiple' => TRUE,
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_txn_type_filter_settings() {
  return array('txn_type');
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_txn_type_filter_process($type, $settings, $object, $charges = array()) {
  $$type = & $object;

  switch ($type) {
    case 'txn':
      if (in_array($txn->type, $settings['txn_type'])) {
        return TRUE;
      }
      break;
  }

  return FALSE;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_misc_group_variable_form(&$form_state, $settings = array()) {
  return ec_store_misc_group_filter_form($form_state, $settings);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_misc_group_variable_settings() {
  return array('group');
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_misc_group_variable_process($type, $settings, $object, $charges = array()) {
  $$type = & $object;

  switch ($type) {
    case 'txn':
      if (!empty($txn->misc)) {
        $charges = array_merge($charges, $txn->misc);
      }
      break;
  }

  ec_store_filter_misc(array('misc_group' => $settings['group']), 'init');

  $charges = array_filter($charges, 'ec_store_filter_misc');

  return array_sum(array_map('ec_store_map_price', $charges));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_store_misc_group_variable_description($chg, $type, $object, $variables) {
  return t('The summation of all the additional charges with the tax group %group', array('%group' => $settings['group']));
}

function ec_store_fields_filter_form(&$form_state, $settings) {
  ctools_include('export');
    
  $fields = ctools_export_crud_load_all('ec_fields');
  $options = array();
  foreach ($fields as $name => $field) {
    $options[$name] = $field->field_title;
  }
  
  $form = array();
  
  $form['name'] = array(
    '#type' => 'select',
    '#title' => t('Field'),
    '#default_value' => $settings['name'],
    '#options' => array_merge(array('' => t('- Select -')), $options),
    '#required' => TRUE,
  );
  
  $form['operation'] = array(
    '#value' => '='
  );
  
  $form['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Value'),
    '#default_value' => $settings['value'],
  );
  
  return $form;
}

function ec_store_fields_filter_settings() {
  return array('name', 'operation', 'value');
}

function ec_store_fields_filter_process($type, $settings, $object, $charges, $chg) {
  $$type =& $object;
  
  if ($type == 'txn') {
    $txn->fields_accessed[] = $settings['name'];
    if (isset($txn->fields[$settings['name']]) && $txn->fields[$settings['name']] == $settings['value']) {
      return TRUE;
    }
  }
  
  return FALSE;
}

function ec_store_address_filter_form(&$form_state, $settings) {
  $settings+= array('address_type' => 'billing', 'parts' => array());
  
  if (!isset($form_state['storage']['settings'])) {
    $form_state['storage']['settings'] = $settings;
  }
  
  $all_options = $options = array(
    'address1' => t('Address line 1'),
    'address2' => t('Address line 2'),
    'city' => t('City'),
    'state' => t('State/Province'),
    'zip' => t('Post/Zip code'),
    'country' => t('Country'),
  );
  
  $options = array_diff_key($options, $form_state['storage']['settings']['parts']);
	
  $form = array();
  
  $form['address_type'] = array(
    '#type' => 'select',
    '#title' => t('Type of address'),
    '#default_value' => $settings['billing'],
    '#options' => array(
      'billing' => t('Billing'),
      'shipping' => t('Shipping'),
    ),
  );
  
  $form['parts'] = array(
    '#tree' => TRUE,
  );
  foreach ($form_state['storage']['settings']['parts'] as $type => $part) {
    $form['parts'][$type] = array(
      '#prefix' => '<div class="container-inline form-item">',
      '#suffix' => '</div>',
    );
    $form['parts'][$type]['type'] = array(
      '#type' => 'value',
      '#value' => $type,
    );
    $form['parts'][$type]['title'] = array(
      '#prefix' => '<label>',
      '#suffix' => '</label>',
      '#value' => t('@title:', array('@title' => $all_options[$type])),
    );

    switch ($part['type']) {
      case 'country':
        $form['parts'][$type]['op'] = array(
          '#type' => 'value',
          '#value' => 'eq',
        );
        $form['parts'][$type]['value'] = array(
          '#type' => 'select',
          '#default_value' => $part['value'],
          '#options' => ec_store_region_country_list(),
          '#multiple' => TRUE,
        );
        break;
      
      default:
        $form['parts'][$type]['op'] = array(
          '#type' => 'select',
          '#default_value' => $part['op'],
          '#options' => array(
            '=' => t('Equals'),
            '!=' => t('No Equal'),
            'contains' => t('Contains'),
            'ncontains' => t('Not contains'),
            'in' => t('In'),
            'nin' => t('Not in'),
          ),
        );
        $form['parts'][$type]['value'] = array(
          '#type' => 'textfield',
          '#default_value' => $part['value'],
          '#description' => t('If you are using the in or not in operator then enter a comma separated list of values.'),
        );
        break;
    }
    $form['parts'][$type]['delete'] = array(
      '#type' => 'submit',
      '#value' => t('delete'),
      '#attributes' => array('style' => 'margin:0;'),
      '#submit' => array('ec_store_address_filter_form_delete'),
      '#name' => 'parts-' . $type . '-delete',
    );
  }
  
  if (!empty($options)) {
    $form['add'] = array(
      '#prefix' => '<div class="container-inline">',
      '#suffix' => '</div>',
    );
    $form['add']['part_type'] = array(
      '#type' => 'select',
      '#options' => $options,
    );
    $form['add']['add_part'] = array(
      '#type' => 'submit',
      '#value' => t('Add'),
      '#submit' => array('ec_store_address_filter_form_add'),
      '#name' => 'parts-add',
    );
  }
  
  return $form;
}

function ec_store_address_filter_form_add(&$form, &$form_state) {
  $part = $form_state['values']['part_type'];
  $form_state['storage']['settings']['parts'][$part] = array('type' => $part, 'op' => 'eq', 'value' => NULL);
  $form_state['rebuild'] = TRUE;
}

function ec_store_address_filter_form_delete(&$form, &$form_state) {
  unset($form_state['storage']['settings']['parts'][$form_state['clicked_button']['#parents'][1]]);
  $form_state['rebuild'] = TRUE;
}

function ec_store_address_filter_settings() {
  return array('address_type', 'parts');
}

function ec_store_address_filter_process($type, $settings, $object, $charges, $chg) {
  $$type =& $object;
  $ret = TRUE;
  
  switch ($type) {
    case 'txn':
      if (isset($txn->address[$settings['address_type']])) {
        $addr = $txn->address[$settings['address_type']];
        foreach ($settings['parts'] as $type => $part) {
          switch ($type) {
            case 'country':
              if (!isset($part['value'][$addr['country']]) || !$part['value'][$addr['country']]) {
                $ret = FALSE;
              }
              break;

            default:
              switch ($part['op']) {
                case '=':
                  if ($addr[$type] != $part['value']) {
                    $ret = FALSE;
                  }

                case '!=':
                  if ($addr[$type] == $part['value']) {
                    $ret = FALSE;
                  }
                
                case 'contains':
                  if (strpos($addr[$type], $part['value']) !== FALSE) {
                    $ret = TRUE;
                  }
                  
                case 'contains':
                  if (strpos($addr[$type], $part['value']) !== FALSE) {
                    $ret = TRUE;
                  }
                  
                case 'in':
                case 'nin':
                  $values = explode(',', $part['value']);
                  $values = array_map('trim', $values);
                  
                  $ret = in_array($addr[$type], $values);
                  $ret = $part['op'] == 'nin' ? !$ret : $ret;
                  break;
                
                default:
                  $ret = FALSE;
              }
          }
          
          if (!$ret) {
            break;
          }
        }
      }
      else {
        $ret = FALSE;
      }
  }
  
  return $ret;
}
