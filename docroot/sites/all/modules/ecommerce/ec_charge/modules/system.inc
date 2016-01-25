<?php

/**
 * @file
 * Provide system intergration for charges to allow low level actions to be
 * done one changes
 */

/**
 * Implementation of hook_ec_charge_filter_info().
 */
/* function system_ec_charge_filter_info() {
 return array(
 'cookie' => array(
 'name' => t('Cookie*'),
 'description' => t('Validate that a cookie exists and it is equal to a defined value'),
 'module' => 'system_cookie',
 'file' => 'system.inc',
 'core' => TRUE,
 ),
 'session' => array(
 'name' => t('Session*'),
 'description' => t('Validate that a session variable exists and it is equal to a defined value'),
 'module' => 'system_session',
 'file' => 'system.inc',
 'core' => TRUE,
 ),
 );
 } */

/**
 * Implementation of hook_ec_charge_calculation_info().
 */
function system_ec_charge_calculation_info() {
  return array(
    'calc' => array(
      'name' => t('Calculation'),
      'description' => t('Provide a method to allow the calculation of the charge.'),
      'module' => 'system_calc',
      'file' => 'system.inc',
      'core' => TRUE,
    ),
  );
}

function system_ec_charge_modification_info() {
  return array(
    'lowest' => array(
      'name' => t('Lowest Value'),
      'description' => t('based up all the transactions that have been calculated, this will choose the lowest charge and only return that one.'),
      'module' => 'system_lowest',
      'file' => 'system.inc',
      'core' => TRUE,
    ),
    'highest' => array(
      'name' => t('Highest Value'),
      'description' => t('based up all the transactions that have been calculated, this will choose the highest charge and only return that one.'),
      'module' => 'system_highest',
      'file' => 'system.inc',
      'core' => TRUE,
    ),
    'displayonly' => array(
      'name' => t('Display only'),
      'description' => t('Change the transactions to only be display only'),
      'module' => 'system_displayonly',
      'file' => 'system.inc',
      'core' => TRUE,
    ),
    'title' => array(
      'name' => t('Title'),
      'description' => t('Change the transactions titles which will display on the transaction'),
      'module' => 'system_title',
      'file' => 'system.inc',
      'core' => TRUE,
    ),
    'clearprice' => array(
      'name' => t('Clear Price'),
      'description' => t('This is mainly used for display only values so that you can add a message with a calculation.'),
      'module' => 'system_clearprice',
      'file' => 'system.inc',
      'core' => TRUE,
    ),
    'additional' => array(
      'name' => t('Move title to additional'),
      'description' => t('Use this to move the additional text section of the invoice'),
      'module' => 'system_additional',
      'file' => 'system.inc',
      'core' => TRUE,
    ),
  );
}

function system_calc_calculation_form(&$form_state, $settings) {
  $form = array();
  $default = array('calc' => '');
  $settings += $default;

  $form['calc'] = array(
    '#type' => 'textfield',
    '#title' => t('Calculation'),
    '#default_value' => $settings['calc'],
  );

  $chg = ec_charge_admin_edit_get_page_cache($form_state['name']);
  $variables = ec_charge_get_variables($chg, $chg->chg_type ? 'node' : 'txn', NULL);
  $descriptions = ec_charge_get_variables_description($chg, $chg->chg_type ? 'node' : 'txn', NULL, $variables);

  $form['replacements'] = array(
    '#type' => 'fieldset',
    '#title' => t('Replacement Tokens'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE, array(
      '#markup' => theme('ec_charge_variable_description', array('descriptions' => $descriptions)),
    ),
    '#weight' => 5,
  );

  return $form;
}

function system_calc_calculation_settings() {
  return array('calc');
}

function system_calc_calculation_process($type, $settings, $variables, $object) {
  $vars = array();
  if (!empty($variables)) {
    $keys = array_map('_system_calc_map_variables', array_keys($variables));
    $vars = array_combine($keys, $variables);
  }

  $calc = strtr($settings['calc'], $vars);

  $price = @eval('return ' . $calc . ';');

  if (!is_scalar($price)) {
    $price = 0;
    drupal_set_message(t('Calculation for %name as failed', array('%name' => $settings['name'])), 'error');
    watchdog('ec_charge', 'Calculation for %name has failed', array('%name' => $settings['name']), WATCHDOG_ERROR);
  }

  $misc = array(
    'description' => $settings['name'],
    'price' => $price,
  );

  return $misc;
}

function _system_calc_map_variables($a) {
  return '[' . $a . ']';
}

function system_lowest_modification_process($type, $settings, $misc, $object) {
  $lowest = reset($misc);

  foreach ($misc as $item) {
    if (abs($lowest['price']) > abs($item['price'])) {
      $lowest = $item;
    }
  }

  return array($lowest);
}

function system_highest_modification_process($type, $settings, $misc, $object) {
  $highest = reset($misc);

  foreach ($misc as $item) {
    if (abs($highest['price']) < abs($item['price'])) {
      $highest = $item;
    }
  }

  return array($highest);
}

function system_displayonly_modification_process($type, $settings, $misc, $object) {
  if (is_array($misc)) {
    return array_map('_system_displayonly_map', $misc);
  }
}

function _system_displayonly_map($a) {
  $a['displayonly'] = 1;
  return $a;
}

function system_title_modification_form(&$form_state, $settings) {
  $form = array();
  $settings += array('title' => '');

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $settings['title'],
    '#size' => 60,
    '#maxlength' => 255,
    '#required' => TRUE,
  );

  return $form;
}

function system_title_modification_settings() {
  return array('title');
}

function system_title_modification_process($type, $settings, $misc, $object) {
  if (is_array($misc)) {
    _system_title_map('init', $settings);
    return array_map('_system_title_map', $misc);
  }
}

function _system_title_map($a, $params = NULL) {
  $settings = & drupal_static(__FUNCTION__ . '_settings');

  if ($a == 'init') {
    $settings = $params;
    return;
  }

  $keys = array_map('_system_calc_map_variables', array_keys($a));
  $params = array_combine($keys, $a);

  $params['[price]'] = format_currency($a['price']);

  $a['description'] = strtr($settings['title'], $params);

  return $a;
}

function system_clearprice_modification_process($type, $settings, $misc, $object) {
  if (is_array($misc)) {
    return array_map('_system_clearprice_map', $misc);
  }
}

function _system_clearprice_map($a) {
  unset($a['price']);
  return $a;
}

function system_additional_modification_form(&$form_state, $settings) {
  $form = array();
  $settings += array(
    'divid' => '',
    'class' => '',
    'title' => '',
  );

  $form['divid'] = array(
    '#type' => 'textfield',
    '#title' => t('Id'),
    '#default_value' => $settings['divid'],
    '#size' => 60,
    '#maxlength' => 255,
  );

  $form['class'] = array(
    '#type' => 'textfield',
    '#title' => t('Class'),
    '#default_value' => $settings['class'],
    '#size' => 60,
    '#maxlength' => 255,
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $settings['title'],
    '#size' => 60,
    '#maxlength' => 255,
    '#required' => TRUE,
  );

  return $form;
}

function system_additional_modification_settings() {
  return array('divid', 'class', 'title');
}

function system_additional_modification_process($type, $settings, $misc, &$object) {
  $$type = & $object;

  if (is_array($misc)) {
    _system_title_map('init', $settings);
    $misc = array_map('_system_title_map', $misc);

    $attributes = array();
    foreach (array(
      'divid' => 'id',
      'class' => 'class',
    ) as $id => $field) {
      if (isset($settings[$id])) {
        $attributes[$field] = $settings[$id];
      }
    }

    foreach ($misc as $item) {
      $txn->additional['ec_charge_' . $settings['chgid']] = array(
        '#prefix' => '<div' . drupal_attributes($attributes) . '>',
        '#value' => $item['description'],
        '#suffix' => '</div>',
      );
    }

    return array();
  }
}

