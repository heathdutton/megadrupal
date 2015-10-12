<?php
// $Id$

/**
 * @file
 * Admin page callbacks for the currency_converter module.
 */

 /**
 * Returns an administrative form with settings.
 */
function currency_converter_admin_settings() {

  $default_weight = array('BYR' => 0, 'RUB' => 1, 'EUR' => 2, 'USD' => 3, 'CAD' => 4, 'PLN' => 5, 'UAH' => 6, 'CNY' => 7, 'LTL' => 8, 'LVL' => 9);
  $weight = variable_get('currency_converter_weight', $default_weight);
  asort($weight);

  $default_currency = variable_get('currency_converter_main_currency', 'BYR');
  $currencies = array();
  $currencies[] = $default_currency;

  foreach ($weight as $curr => $key) {
    if ($curr != $default_currency)
      $currencies[] = $curr;
  }

  foreach ($currencies as $currency) {

    $form[$currency]['title'] = array(
      '#type' => 'item',
      '#markup' => $currency
    );
    $form[$currency][$currency .'_weight'] = array(
      '#type' => 'weight',
      '#default_value' => $weight[$currency],
      '#attributes' => array('class' => array('currency-converter-weight')),
    );
    $options[$currency] = '';

  }

  $form['currency_converter_main_currency'] = array(
    '#type' => 'radios',
    '#default_value' => variable_get('currency_converter_main_currency', 'BYR'),
    '#options' => $options,
  );
  $form['currency_converter_currency_types'] = array(
    '#type' => 'checkboxes',
    '#default_value' => variable_get('currency_converter_currency_types', $currencies),
    '#options' => $options,
  );

  // bank settings
  $form['currency_converter_bank'] = array(
    '#type' => 'fieldset',
    '#title' => t('Bank settings'),
  );
  $form['currency_converter_bank']['currency_converter_bank_default'] = array(
    '#type' => 'radios',
    '#default_value' => variable_get('currency_converter_bank_default', 0),
    '#options' => array(
      0 => t('National Bank of Belarus'),
      1 => t('Russian Central Bank'),
    ),
  );

  // css settings
  $form['currency_converter_css'] = array(
    '#type' => 'fieldset',
    '#title' => t('Css settings'),
  );
  $form['currency_converter_css'] ['currency_converter_use_css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use module css'),
    '#default_value' => variable_get('currency_converter_use_css', 1),
    '#description' => t('If checked, block will use module css'),
  );
  $form['currency_converter_css'] ['currency_converter_use_bg'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use module background'),
    '#default_value' => variable_get('currency_converter_use_bg', 1),
    '#description' => t('If checked, block will use module background'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['run_cron'] = array(
    '#type' => 'submit',
    '#value' => t('Run cron'),
  );

  return $form;
}

function currency_converter_admin_settings_submit($form, &$form_state) {

  $default_weight = array();
  foreach (element_children($form) as $key) {
    if (strlen($key) == 3) {
      $default_weight[$key] = $form_state['values'][$key .'_weight'];
    }
  }

  variable_set('currency_converter_weight', $default_weight);
  variable_set('currency_converter_main_currency', $form_state['values']['currency_converter_main_currency']);
  variable_set('currency_converter_currency_types', $form_state['values']['currency_converter_currency_types']);
  variable_set('currency_converter_bank_default', $form_state['values']['currency_converter_bank_default']);
  variable_set('currency_converter_use_css', $form_state['values']['currency_converter_use_css']);
  variable_set('currency_converter_use_bg', $form_state['values']['currency_converter_use_bg']);

  if ($form_state['values']['op'] == t('Run cron')) {
    $form_state['redirect'] = 'admin/reports/status/run-cron';
  }
  else {
    drupal_set_message(t('Configuration saved'));
  }
}

function theme_currency_converter_admin_settings($form) {

  $rows = array();
  foreach (element_children($form['form']) as $key) {
    if (strlen($key) == 3) {

      $row = array();
      $row[0] = drupal_render($form['form']['currency_converter_currency_types'][$key]);
      $row[1] = drupal_render($form['form'][$key]['title']);
      $row[2] = drupal_render($form['form'][$key][$key .'_weight']);
      $row[3] = drupal_render($form['form']['currency_converter_main_currency'][$key]);
      $rows[] = array(
        'data' => $row,
        'class' => array('draggable'),
      );
    }
  }

  $header = array(t('Enabled'), t('Currency'), t('Weight'), t('Set as default'));
  $table = array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'currency-converter-table'));
  $output = theme('table', $table);
  drupal_add_tabledrag('currency-converter-table', 'order', 'sibling', 'currency-converter-weight');
  return $output . drupal_render_children($form['form']);
}
