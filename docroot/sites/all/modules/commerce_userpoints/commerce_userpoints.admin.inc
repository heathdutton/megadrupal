<?php

/**
 * @file
 * Administration page callbacks for the commerce_userpoints module.
 */
/**
 * Page callback for setting up userpoints currencies.
 */
function commerce_userpoints_currencies_page() {
  $currencies = commerce_userpoints_currencies();
  $categories = userpoints_get_categories();

  $header = array(t('Name'), t('Category'), t('Code'), t('Symbol'), t('Operations'));

  $rows = array();
  foreach ($currencies as $currency) {
    $rows[] = array(
      $currency['name'],
      $categories[$currency['tid']],
      $currency['code'],
      $currency['symbol'],
      l(t('edit'), 'admin/commerce/config/currency/userpoints/edit/' . $currency['code'], array('query' => drupal_get_destination())) . ' ' .
      l(t('delete'), 'admin/commerce/config/currency/userpoints/delete/' . $currency['code'], array('query' => drupal_get_destination())),
    );
  }

  return array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#header' => $header,
    '#empty' => t('No !point currencies have been set up.', userpoints_translation()),
  );
}

/**
 * Form builder function to add a new userpoints category.
 */
function commerce_userpoints_currencies_form($form, &$form_state, $currency = array()) {
  $currency += array(
    'code' => '',
    'name' => '',
    'tid' => NULL,
    'symbol' => '',
    'currencies' => array(),
  );

  $form_state['commerce_userpoints_currency'] = $currency;


  $form['name'] = array(
    '#title' => t('Currency name'),
    '#type' => 'textfield',
    '#size' => 10,
    '#required' => TRUE,
    '#default_value' => $currency['name'],
  );

  // Only show categories which aren't yet used for a currency.
  $remaining_categories = userpoints_get_categories();
  foreach (commerce_userpoints_currencies() as $existing_currency) {
    if (empty($currency['code']) || $existing_currency['code'] != $currency['code']) {
      unset($remaining_categories[$existing_currency['tid']]);
    }
  }

  if (empty($remaining_categories)) {
    drupal_set_message(t('You have no !points categories which are not used already as a currency.', userpoints_translation()), 'error');
  }

  $form['tid'] = array(
    '#title' => t('!Points category', userpoints_translation()),
    '#type' => 'select',
    '#options' => $remaining_categories,
    '#required' => TRUE,
    '#default_value' => $currency['tid'],
  );

  $form['code'] = array(
    '#title' => t('Currency code'),
    '#type' => 'textfield',
    '#size' => 5,
    '#required' => TRUE,
    '#default_value' => $currency['code'],
    '#disabled' => !empty($currency['code']),
  );

  $form['symbol'] = array(
    '#title' => t('Currency symbol'),
    '#type' => 'textfield',
    '#size' => 5,
    '#required' => TRUE,
    '#default_value' => $currency['symbol'],
  );


  $form['currencies']['#tree'] = TRUE;
  $rows = array();
  
  $form['conversion_rate'] = array(
    '#type' => 'textfield',
    '#title' => t('Exchange rate'),
    '#default_value' => isset($currency['conversion_rate'])? $currency['conversion_rate'] : 0.01,
    '#size' => 5,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Check if a given key (code, symbol, ..) is not already used.
 *
 * @param $value
 *   The value to be checked against.
 * @param $key
 *   The key to check against, any key in a currency definition is allowed.
 * @param $code
 *   The code of the current currency if we're editing one.
 *
 * @return
 *   TRUE if the value is unique, FALSE if not.
 */
function _commerce_userpoints_validate_uniqueness($value, $key, $code = NULL) {
  $currencies = commerce_currencies();
  foreach ($currencies as $currency) {
    // Check if the value is the same and code is empty (new currency) or not
    // the one we're comparing against.
    if ($value == $currency[$key] && (!$code || $code != $currency['code'])) {
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * Form validate callback for adding a new currency.
 */
function commerce_userpoints_currencies_form_validate($form, &$form_state) {
  // Only validate the code for new currencies as changing is not allowed for
  // existing ones.
  if (empty($form_state['commerce_userpoints_currency'])) {
    if (!_commerce_userpoints_validate_uniqueness($form_state['values']['code'], 'code')) {
      form_set_error('code', t('The code is already in use for another currency.'));
    }
  }
  if (!_commerce_userpoints_validate_uniqueness($form_state['values']['symbol'], 'symbol', $form_state['values']['code'])) {
    form_set_error('symbol', t('The symbol is already in use for another currency.'));
  }
}

/**
 * Form submit callback, saves a new currency.
 */
function commerce_userpoints_currencies_form_submit($form, &$form_state) {
  $userpoints_currencies = commerce_userpoints_currencies();

  form_state_values_clean($form_state);

  // Save the new or updated currency.
  $userpoints_currencies[$form_state['values']['code']] = $form_state['values'];
  variable_set('commerce_userpoints_currencies', $userpoints_currencies);

  // Automatically enable this currency.
  $enabled_currencies = variable_get('commerce_enabled_currencies', array('USD' => 'USD'));
  $enabled_currencies[$form_state['values']['code']] = $form_state['values']['code'];
  variable_set('commerce_enabled_currencies', $enabled_currencies);

  $form_state['redirect'] = 'admin/commerce/config/currency/userpoints';
  drupal_set_message(t('!Points currency saved and enabled.', userpoints_translation()));

  // Flush out commerce's currency cache.
  cache_clear_all('commerce_currencies:', 'cache', TRUE);
}

/**
 * Form builder callback for the delete confirmation form.
 */
function commerce_userpoints_currencies_delete($form, &$form_state, $currency) {
  $form_state['commerce_userpoints_currency'] = $currency;
  return confirm_form($form, t('Do you want to delete the currency @name', array('@name' => $currency['name'])), 'admin/commerce/config/currency/userpoints');
}

/**
 * Form submit callback for the delete confirmation form.
 */
function commerce_userpoints_currencies_delete_submit($form, &$form_state) {
  $currencies = commerce_userpoints_currencies();
  unset($currencies[$form_state['commerce_userpoints_currency']['code']]);
  variable_set('commerce_userpoints_currencies', $currencies);

  // Flush out commerce's currency cache.
  cache_clear_all('commerce_currencies:', 'cache', TRUE);
}
