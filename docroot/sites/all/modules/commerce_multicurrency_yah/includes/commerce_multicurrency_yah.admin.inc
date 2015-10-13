<?php

/**
 * @file
 * Provides the administrative settings for the Yahoo Finance module.
 */

/**
 * Administrator settings form for the Yahoo Finance module.
 */
function commerce_multicurrency_yah_settings_form($form, &$form_state) {
  $form = array();
  $enabled_sync_provider = variable_get('commerce_multicurrency_sync_provider', 'ecb');
  $exchange_rates_table = commerce_multicurrency_yah_display_rates(TRUE);

  $form['yahoo_finance_status'] = array(
    '#type' => 'fieldset',
    '#title' => t('Yahoo Finance Status'),
    '#weight' => 1,
    '#collapsible' => FALSE,
    '#collapsed' => TRUE,
  );

  if ($enabled_sync_provider == 'yahoo_finance') {
    $form['yahoo_finance_status']['#attributes'] = array(
      'id' => array('commerce-multicurrency-yah-enabled'),
    );
    $form['yahoo_finance_status']['enabled_text'] = array(
      '#prefix' => '<div id = "commerce-multicurrency-yah-enabled-text">',
      '#markup' => t('The Yahoo Finance currency sync provider is currently enabled.'),
      '#suffix' => '</div>',
    );
    $form['yahoo_finance_status']['enable_yahoo_finance'] = array(
      '#type' => 'checkbox',
      '#title' => t('Yahoo Finance for Multicurrency.'),
      '#description' => t('Enables/Disables the Yahoo Finance currency sync provider. Be sure of what you are doing before changing this field.'),
      '#default_value' => TRUE,
    );
  }
  else {
    $form['yahoo_finance_status']['#attributes'] = array(
      'id' => array('commerce-multicurrency-yah-disabled'));
    $form['yahoo_finance_status']['disabled_text'] = array(
      '#prefix' => '<div id = "commerce-multicurrency-yah-disabled-text">',
      '#markup' => t('The Yahoo Finance currency sync provider is currently disabled.'),
      '#suffix' => '</div>',
    );
    $form['yahoo_finance_status']['enable_yahoo_finance'] = array(
      '#type' => 'checkbox',
      '#title' => t('Yahoo Finance for Multicurrency.'),
      '#description' => t('Enables/Disables the Yahoo Finance currency sync provider. Be aware of what you are doing before changing this field.'),
      '#default_value' => FALSE,
    );
  }

  // Synchronize the selected rates with the yahoo finance.
  $form['yahoo_finance_status']['yahoo_finance_sync_now'] = array(
    '#type' => 'checkbox',
    '#title' => t('Synchronize rates on save.'),
    '#description' => t('The rates will only be synchronized if the Yahoo Finance sync provider is enabled.'),
  );

  $form['yahoo_finance_site_currency_rates'] = array(
    '#type' => 'fieldset',
    '#title' => t('Currency Rates'),
    '#description' => t("The currency exchange rates of all the enabled currencies for the store's default currency."),
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['yahoo_finance_site_currency_rates']['rates_table'] = array(
    '#markup' => $exchange_rates_table,
  );

  $form['#attached']['css'] = array(
    drupal_get_path('module', 'commerce_multicurrency_yah') .
    '/css/commerce_multicurrency_yah.css',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Settings'),
    '#weight' => 5,
  );

  return $form;
}

/**
 * Submit handler for the yahoo finance settings form.
 */
function commerce_multicurrency_yah_settings_form_submit($form, &$form_state) {
  // Check if the Yahoo Finance for Multicurrency checkbox is clicked.
  if ($form_state['values']['enable_yahoo_finance'] == TRUE) {
    variable_set('commerce_multicurrency_sync_provider', 'yahoo_finance');
    drupal_set_message(t('The Yahoo Finance sync provider has been enabled successfully.'), 'status');
    if ($form_state['values']['yahoo_finance_sync_now'] == TRUE) {
      commerce_multicurrency_sync_exchange_rates_now();
      drupal_set_message(t('The currency rates has been syncronized with Yahoo Finance successfully.'), 'status');
    }
    else {
      // Ensure the settings are in sync.
      commerce_currencies(FALSE, TRUE);
    }
  }
  // Check if the Yahoo Finance for Multicurrency checkbox is not clicked &
  // Synchronize rates on save checkbox is clicked.
  elseif ($form_state['values']['enable_yahoo_finance'] == FALSE &&
      $form_state['values']['yahoo_finance_sync_now'] == TRUE) {
    $msg = t('The Yahoo Finance sync provider should be enabled for the rates to be synchronized with Yahoo Finance.');
    drupal_set_message($msg, 'warning');
  }
  else {
    $enabled_sync_provider = variable_get('commerce_multicurrency_sync_provider');
    if ($form_state['values']['enable_yahoo_finance'] == FALSE &&
    $enabled_sync_provider == 'yahoo_finance') {
      variable_set('commerce_multicurrency_sync_provider', 'ecb');
      $msg = t('The Yahoo Finance sync provider has been disabled successfully.');
      $msg1 = t('The default European Central Bank sync provider is now enabled. Visit <a href="@ecb-settings-link">Currency conversion page</a> to configure it.', array(
        '@ecb-settings-link' => url('admin/commerce/config/currency/conversion'))
      );
      drupal_set_message($msg, 'status');
      drupal_set_message($msg1, 'warning');
    }
    else {
      $msg = t('The Yahoo Finance sync provider is not  enabled. Either enable Yahoo Finance below or visit <a href="@ecb-settings-link">Currency conversion page</a> to configure other sync providers.',
      array(
        '@ecb-settings-link' => url('admin/commerce/config/currency/conversion'))
      );
      drupal_set_message($msg, 'warning');
    }
  }
}
