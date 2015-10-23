<?php
/**
 * @file
 * Bitcoin Price module's admin funcions.
 */

/**
 * Administration settings form
 */
function bitcoin_price_admin_settings() {
  $form = array();

  $form['exchange_preference'] = array(
    '#type' => 'fieldset',
    '#title' => 'Exchange Preference Order',
    '#collapsible' => true,
  );
  $form['exchange_preference']['bitcon_price_exch_primary'] = array(
    '#type' => 'select',
    '#title' => 'Primary Exchange',
    '#options' => array(
      'bitcoinaverage' => 'Bitcoin Average (https://bitcoinaverage.com)',
      'bitcoincharts' => 'Bitcoin Charts (https://bitcoincharts.com)',
    ),
    '#default_value' => variable_get('bitcon_price_exch_primary', ''),
  );
  $form['exchange_preference']['bitcon_price_exch_secondary'] = array(
    '#type' => 'select',
    '#title' => 'Secondary Exchange',
    '#options' => array(
      'bitcoinaverage' => 'Bitcoin Average (https://bitcoinaverage.com)',
      'bitcoincharts' => 'Bitcoin Charts (https://bitcoincharts.com)',
    ),
    '#default_value' => variable_get('bitcon_price_exch_secondary', ''),
  );

  $exchanges = bitcoin_price_get_exchanges();

  foreach ($exchanges as $exchange_name => $exchange_info) {
    $form[$exchange_name] = array(
      '#type' => 'fieldset',
      '#title' => $exchange_info['label'],
      '#collapsible' => true,
    );
    $form[$exchange_name]['url'] = array(
      '#type' => 'markup',
      '#markup' => l($exchange_info['url'], $exchange_info['url']),
    );

    $currencies = bitcoin_price_get_currencies();
    $rates = call_user_func($exchange_info['callback'], $currencies);
    $options = array();
    foreach ($rates as $currency => $rate_info) {
      $options[$currency] = $currency . ' - ' . $rate_info['display'] . ' (' . $rate_info['span'] . ')';
    }

    $exchange_currencies_var = 'bitcoin_price_exch_currencies_' . $exchange_name;
    $form[$exchange_name][$exchange_currencies_var] = array(
      '#type' => 'checkboxes',
      '#title' => 'Enabled Currencies',
      '#options' => $options,
      '#default_value' => variable_get($exchange_currencies_var, array()),
    );
  }

  return system_settings_form($form);
}

