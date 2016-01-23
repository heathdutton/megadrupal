<?php

/**
 * @file pg_webmoney.admin.inc
 * Administration functions for webmoney gateway.
 */

/**
 * Return form with webmoney currencies
 */
function pg_webmoney_list($form, &$form_state) {

  if (FALSE === @('XML/Unserializer.php')) {
    drupal_set_message(t('You have to install !url, otherwise Webmoney Gateway will not work.', array('!url' => l('XML/Serializer', 'http://pear.php.net/package/XML_Serializer'))), 'error');
  }

  $result = db_query('SELECT * FROM {pg_webmoney_rates} ORDER BY type');
  $form['#tree'] = TRUE;
  $purses = array();
  foreach ($result as $currency) {
    $form[$currency->type]['purse']      = array('#value' => $currency->purse);
    $form[$currency->type]['secret_key'] = array('#value' => $currency->secret_key);
    $form[$currency->type]['rate']       = array('#value' => $currency->rate);
    $form[$currency->type]['configure']  = array('#value' => l('example', 'admin/pgdata/pgsettings/webmoney/example/' . $currency->purse));
    $form[$currency->type]['edit']       = array('#value' => l($currency->type, 'admin/pgdata/pgsettings/webmoney/edit/' . $currency->type));
    $purses[$currency->type] = '';
  }

  $form['check'] = array(
    '#type' => 'checkboxes',
    '#options' => $purses,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete checked Wallet'),
  );
  return $form;
}

/**
 * Submit function for webmoney list form
 */
function pg_webmoney_list_submit($form, &$form_state) {
  $wmtype = array();
  foreach ($form_state['values']['check'] as $key => $val) {
    if ($key === $val) {
      db_delete('pg_webmoney_rates')
        ->condition('type', $val)
        ->execute();
      $wmtype[] = $val;
    }
  }
  $deleted = implode(', ', $wmtype);
  drupal_set_message(t('Your wallet types %wmtype has been deleted.', array('%wmtype' => $deleted)));
  pg_webmoney_cron();
}


/**
 * Return webmoney edit form
 */
function pg_webmoney_edit($form, &$form_state, $wmtype) {
  $pursed = db_query('SELECT * FROM {pg_webmoney_rates} WHERE type = :type ', array(':type' => $wmtype))->fetchObject();
  $form['type'] = array(
    '#type' => 'hidden',
    '#value' => $wmtype,
  );
  $form['purse'] = array(
    '#type' => 'markup',
    '#title' => t('Wallet'),
    '#value' => $pursed->purse,
  );
  $form['secret_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret key'),
    '#default_value' => $pursed->secret_key,
    '#maxlength' => 64,
    '#description' => t("Please enter secret key. If your leave it blank, secret key will not apply."),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update Wallet'),
  );
  return $form;
}

/**
 * Submit function for webmoney edit form
 */
function pg_webmoney_edit_submit($form, &$form_state) {
  if (!empty($form_state['values']['type']) ) {
    $wmtype = drupal_substr($form_state['values']['type'], 2, 1);
    $rate = pg_webmoney_get_rate($wmtype);
    db_update('pg_webmoney_rates')
      ->fields(array(
        'purse' => $form_state['values']['purse'],
        'secret_key' => $form_state['values']['secret_key'],
        'rate' => $rate,
      ))
      ->condition('type', $wmtype)
      ->execute();
    drupal_set_message(t('Your wallet type %type has been modified.', array('%type' => $wmtype)));
  }

  pg_webmoney_cron();
  $form_state['redirect'] = 'admin/pgdata/pgsettings/webmoney/list';
}

/**
 * Return webmoney add form
 */
function pg_webmoney_add($form, &$form_state) {
  $form['purse'] = array(
    '#type' => 'textfield',
    '#title' => t('Wallet'),
    '#maxlength' => 13,
    '#default_value' => '',
    '#description' => t("Please, enter Wallet number."),
    '#required' => TRUE,
  );
  $form['secret_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret key'),
    '#default_value' => '',
    '#maxlength' => 64,
    '#description' => t("Please enter secret key. If your leave it blank, secret key will not apply."),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add Wallet'),
  );
  return $form;
}

/**
 * Validate function for webmoney add form
 */
function pg_webmoney_add_validate($form, &$form_state) {
  if (!empty($form_state['values']['purse']) ) {
    $type = substr($form_state['values']['purse'], 0, 1);
    $wmtype = 'WM' . $type;
    $pursed = db_query('SELECT * FROM {pg_webmoney_rates} WHERE type = :type ', array(':type' => $wmtype))->fetchObject();
    if (isset($pursed->type)) {
      form_set_error('purse', t('!type already exists.Please enter another wallet number.', array('!type' => $wmtype)));
    }
  }
}

/**
 * Submit function for webmoney add form
 */
function pg_webmoney_add_submit($form, &$form_state) {
  if (!empty($form_state['values']['purse']) ) {
    $type = substr($form_state['values']['purse'], 0, 1);
    $wmtype = 'WM' . $type;
    $rate = pg_webmoney_get_rate($type);
    db_insert('pg_webmoney_rates')
      ->fields(array(
        'type' => $wmtype,
        'purse' => $form_state['values']['purse'],
        'secret_key' => $form_state['values']['secret_key'],
        'rate' => $rate,
      ))
      ->execute();
    drupal_set_message(t('Your wallet %wallet has been added.', array('%wallet' => $form_state['values']['purse'])));
  }

  pg_webmoney_cron();
  $form_state['redirect'] = 'admin/pgdata/pgsettings/webmoney/list';
}

/**
 * Return webmoney settings form
 */
function pg_webmoney_settings($form, &$form_state) {

  $result = db_query('SELECT * FROM {pg_webmoney_rates} ORDER BY type');
  foreach ($result as $currency) {
    $types[$currency->type] = $currency->type;
  }

  if (empty($types)) {
    form_set_error('', t('Please add your wallet number first'));
    return "";
  }

  $form['rate_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Rate url'),
    '#default_value' => variable_get('pg_webmoney_rate_url', cWebMoneyRateUrl),
    '#description' => t("Please enter rate url. Default: !url", array('!url' => cWebMoneyRateUrl)),
  );
  $form['action_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Action url'),
    '#default_value' => variable_get('pg_webmoney_action_url', cWebMoneyPayActionUrl),
    '#description' => t("Please enter action url. Default: !url", array('!url' => cWebMoneyPayActionUrl)),
  );
  $form['wm_type'] = array(
    '#type' => 'select',
    '#options' => $types,
    '#title' => t('Wallet type'),
    '#default_value' => variable_get('pg_webmoney_type_wm', 'WMR'),
    '#description' => t("Please select your wallet type to rate to site currency."),
  );
  $form['wm_rate'] = array(
    '#type' => 'textfield',
    '#title' => t('Rate to site currency'),
    '#default_value' => variable_get('pg_webmoney_rate_wm', '1.00'),
    '#description' => t("Please enter webmoney rate according to site currency."),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

/**
 * Validate function for webmoney settings form
 */
function pg_webmoney_settings_validate($form, &$form_state) {
  if ($form_state['values']['wm_rate'] <= 0) {
    form_set_error('wm_rate', t('%wm_rate must be more 0.', array('%wm_rate' => $form_state['values']['wm_rate'])));
  }
}

/**
 * Submit function for webmoney settings form
 */
function pg_webmoney_settings_submit($form, &$form_state) {
  if (isset($form_state['values']['rate_url'])) {
    variable_set('pg_webmoney_rate_url', $form_state['values']['rate_url']);
  }
  if (isset($form_state['values']['action_url'])) {
    variable_set('pg_webmoney_action_url', $form_state['values']['action_url']);
  }
  if (isset($form_state['values']['wm_rate'])) {
    variable_set('pg_webmoney_rate_wm', $form_state['values']['wm_rate']);
  }
  if (isset($form_state['values']['wm_type'])) {
    variable_set('pg_webmoney_type_wm', $form_state['values']['wm_type']);
  }
  drupal_set_message(t('Your webmoney rate settings has been saved.'));
  pg_webmoney_cron();
}

/**
 * Return example form for webmoney
 */
function pg_webmoney_example($purse = NULL) {
  $pursed = db_query('SELECT * FROM {pg_webmoney_rates} WHERE purse = :purse', array(':purse' => $purse))->fetchObject();
  return theme('pg_webmoney_example', array(
    'purse' => $pursed->purse,
    'tradename' => variable_get('site_name', 'Drupal'),
    'secret_key' => $pursed->secret_key,
    'success' => url('webmoney/success', array('absolute' => TRUE)),
    'done' => url('webmoney/done', array('absolute' => TRUE)),
    'fail' => url('webmoney/fail', array('absolute' => TRUE)),
  ));
}
