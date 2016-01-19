<?php

/**
 * @file
 * Admin functions for Commerce postcode weight.
 */

/**
 * Builds the admin settings form for configuring postcode weight.
 *
 * @return array
 *   Drupal form for postcode weight settings.
 */
function commerce_shipping_postal_code_weight_settings_form() {
  $form = array();
  $info = field_info_instance('commerce_customer_profile', 'commerce_customer_address', 'shipping');
  if (empty($info['widget']['settings']['available_countries'])) {
    drupal_set_message(t(
      "Please set !available_countries first, shipping method is not available for countries without postal codes",
      array(
        '!available_countries' => l(
        t('Available countries'),
        'admin/commerce/customer-profiles/types/shipping/fields/commerce_customer_address',
        array('query' => drupal_get_destination())
        ),
      )
    ), 'warning');
    return;
  }
  $countries = _addressfield_country_options_list();
  $form['#available_countries'] = $info['widget']['settings']['available_countries'];
  foreach ($info['widget']['settings']['available_countries'] as $country_code) {
    $form[$country_code] = array(
      '#type' => 'fieldset',
      '#title' => $countries[$country_code],
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $weights = variable_get($country_code . '_weights', array(10, 20, 50));
    $postal_codes = variable_get($country_code . '_postal_codes', array());
    foreach ($postal_codes as $index => $p) {
      $p = is_array($p) ? $p : array($p);
      $postal_codes[$index] = implode(', ', $p);
    }
    $form[$country_code][$country_code . '_weights'] = array(
      '#type' => 'textfield',
      '#title' => t('Weights'),
      '#description' => t('a set of weights comma seperated'),
      '#default_value' => implode(',', $weights),
    );
    $form[$country_code]['#attached']['css'][] = drupal_get_path('module', 'physical') . '/theme/physical.css';
    $form[$country_code][$country_code . '_weights']['#prefix'] = '<div class="physical-weight-textfield">';

    $form[$country_code][$country_code . '_unit'] = array(
      '#type' => 'select',
      '#options' => physical_weight_unit_options(FALSE),
      '#default_value' => variable_get($country_code . '_unit', 'lb'),
      '#suffix' => '</div>',
    );
    $form[$country_code][$country_code . '_postal_codes'] = array(
      '#type' => 'textarea',
      '#title' => t('Postal Codes'),
      '#description' => t('sets of postal codes comma seperated, 11,22;33,44;55,66'),
      '#default_value' => implode('; ', $postal_codes),
      '#rows' => 5,
    );
    if (!empty($weights) && !empty($postal_codes)) {
      $cols_label = array();
      foreach ($weights as $index => $weight) {
        $cols_label[] = ($index ? $weights[$index - 1] : '0') . ' - ' . $weight;
      }
      $cols_label[] = '+';

      $rows = array();
      $rows_label = array();
      foreach ($postal_codes as $index => $p) {
        $rows_label[] = $p;
      }
      $rows_label[] = t('others');
      $default = variable_get($country_code . '_rates', array());
      $currency_code = variable_get($country_code . 'currency_code', commerce_default_currency());
      foreach ($rows_label as $i => $row_label) {
        $row = array($row_label);
        foreach ($cols_label as $j => $h) {
          $amount = isset($default[$country_code . '-' . $i . '-' . $j]) ? $default[$country_code . '-' . $i . '-' . $j] : '';
          $row[] = commerce_currency_format($amount, $currency_code);
        }
        $rows[] = $row;
      }
      $form[$country_code][$country_code . 'rates'] = array(
        '#theme' => 'table',
        '#header' => array_merge(array('Postal\weight'), $cols_label),
        '#rows' => $rows,
        '#suffix' => l(t('Edit'), "admin/commerce/config/shipping/methods/postal-code-weight-shipping-method/edit/$country_code", array('query' => drupal_get_destination())),
      );
    }
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Submit handler for postcode weight admin settings form.
 *
 * @param array $form
 *   The postcode weight admin settings form.
 * @param array $form_state
 *   The postcode weight admin settings form state.
 */
function commerce_shipping_postal_code_weight_settings_form_submit($form, $form_state) {
  foreach ($form['#available_countries'] as $country_code) {
    $weights = array_map('trim', explode(',', $form_state['values'][$country_code . '_weights']));
    $postal_codes = array();
    foreach (explode(';', $form_state['values'][$country_code . '_postal_codes']) as $p) {
      $postal_codes[] = array_map('trim', explode(',', $p));
    }
    sort($weights);
    variable_set($country_code . '_weights', $weights);
    variable_set($country_code . '_unit', $form_state['values'][$country_code . '_unit']);
    variable_set($country_code . '_postal_codes', $postal_codes);
  }

  drupal_set_message(t('The Postcode weight configuration options have been saved.'));
}

/**
 * Builds the admin settings form for configuring postcode weight.
 *
 * @param array $form
 *   The postcode weight admin settings form.
 * @param array $form_state
 *   The postcode weight admin settings form state.
 * @param string $country_code
 *   The postcode weight admin settings country code.
 *
 * @return array
 *   Drupal form for postcode weight settings.
 */
function commerce_shipping_postal_code_weight_settings_matrix_form($form, $form_state, $country_code) {
  $form = array();
  $form['#country_code'] = $country_code;
  $options = array();
  foreach (commerce_currencies(TRUE) as $currency_code => $currency) {
    $options[$currency_code] = check_plain($currency['code']);
  }
  $currency_code = variable_get($form['#country_code'] . '_currency_code', commerce_default_currency());
  $form['currency_code'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $currency_code,
  );
  $form['rates'] = array(
    '#tree' => TRUE,
    '#theme' => 'table',
  );
  $weights = variable_get($country_code . '_weights', array(10, 20, 50));
  $postal_codes = variable_get($country_code . '_postal_codes', array());
  $cols_label = array();
  foreach ($weights as $index => $weight) {
    $cols_label[] = ($index ? $weights[$index - 1] : '0') . ' - ' . $weight;
  }
  $cols_label[] = '+';
  $form['rates']['#header'] = array_merge(array('Postal\weight'), $cols_label);

  $rows = array();
  $rows_label = array();
  foreach ($postal_codes as $index => $p) {
    $rows_label[] = implode(',', $p);
  }
  $rows_label[] = t('others');
  $default = variable_get($form['#country_code'] . '_rates', array());
  foreach ($rows_label as $i => $row_label) {
    $row = array($row_label);
    foreach ($cols_label as $j => $h) {
      $amount = isset($default[$country_code . '-' . $i . '-' . $j]) ? $default[$country_code . '-' . $i . '-' . $j] : '';
      $form['rates'][$country_code . '-' . $i . '-' . $j] = array(
        '#type' => 'textfield',
        '#default_value' => commerce_currency_amount_to_decimal($amount, $currency_code),
        '#size' => '15',
      );
      $row[] = array('data' => &$form['rates'][$country_code . '-' . $i . '-' . $j]);
    }
    $form['rates']['#rows'][] = $row;
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Validate handler for postcode weight admin settings form.
 *
 * @param array $form
 *   The postcode weight admin settings form.
 * @param array $form_state
 *   The postcode weight admin settings form state.
 */
function commerce_shipping_postal_code_weight_settings_matrix_form_validate($form, &$form_state) {
  foreach ($form_state['values']['rates'] as $field => $value) {
    $element = $form['rates'][$field];
    if ($element['#value'] !== '') {
      // Ensure the price is numeric.
      if (!is_numeric($element['#value'])) {
        form_error($element, t('You must enter a numeric value for the price amount.'));
      }
      else {
        form_set_value(
          $element,
          commerce_currency_decimal_to_amount($element['#value'], $form['currency_code']['#value']),
          $form_state
        );
      }
    }
  }
}

/**
 * Submit handler for postcode weight admin settings form.
 *
 * @param array $form
 *   The postcode weight admin settings form.
 * @param array $form_state
 *   The postcode weight admin settings form state.
 */
function commerce_shipping_postal_code_weight_settings_matrix_form_submit($form, $form_state) {
  variable_set($form['#country_code'] . '_rates', $form_state['values']['rates']);
  variable_set($form['#country_code'] . '_currency_code', $form_state['values']['currency_code']);
}
