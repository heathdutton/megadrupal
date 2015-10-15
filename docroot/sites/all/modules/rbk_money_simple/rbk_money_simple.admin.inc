<?php

/**
 * Settings page
 */

function rbk_money_simple_settings() {
  $form['rbk_money_simple_hint'] = array(
    '#markup' => t('Simple payment page: <a href="!link">!link</a>', array('!link' => "{$GLOBALS['base_url']}/rbk_money_simple/payment")),
  );
  $form['rbk_money_simple_response'] = array(
    '#type' => 'textfield',
    '#title' => t('Response URL'),
    '#default_value' => $GLOBALS['base_url'] . '/rbk_money_simple/status',
    '#description' => t('For insertion into "Payment notification URL" field on !site settings page', array('!site' => l('RBK Money', 'http://www.rbkmoney.ru', array('attributes' => array('target' => '_blank'))))),
    '#required' => FALSE,
  );
  $form['rbk_money_simple_eshopId'] = array(
    '#type' => 'textfield',
    '#title' => t('Site ID'),
    '#default_value' => variable_get('rbk_money_simple_eshopId', ''),
    '#description' => t("Merchand ID of your site"),
    '#required' => TRUE,
  );
  $form['rbk_money_simple_secretKey'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret key'),
    '#default_value' => variable_get('rbk_money_simple_secretKey', ''),
    '#description' => t('Secret key, entered on !site settings page', array('!site' => l('RBK Money', 'http://www.rbkmoney.ru', array('attributes' => array('target' => '_blank'))))),
    '#required' => TRUE,
  );
  $form['rbk_money_simple_recipientCurrency'] = array(
    '#type' => 'select',
    '#title' => t('Payments currency'),
    '#options' => array('RUR' => 'RUR', 'USD' => 'USD', 'EUR' => 'EUR', 'UAH' => 'UAH'),
    '#default_value' => variable_get('rbk_money_simple_recipientCurrency', 'RUR'),
    '#description' => t("Please, select payments currency"),
  );
  $form['rbk_money_simple_send_hash'] = array(
    '#type' => 'select',
    '#title' => t('Send hash with payment request'),
    '#options' => array(0 => 'off (default)', 1 => 'on'),
    '#default_value' => variable_get('rbk_money_simple_send_hash', 0),
    '#description' => t("Send order hash in payment request. Option 'Check request form hash' must be checked on !site settings page", array('!site' => l('RBK Money', 'http://www.rbkmoney.ru', array('attributes' => array('target' => '_blank'))))),
  );
  $form['rbk_money_simple_hash'] = array(
    '#type' => 'select',
    '#title' => t('Hash method'),
    '#options' => array('md5' => 'MD5 (default)', 'sha512' => 'SHA512'),
    '#default_value' => variable_get('rbk_money_simple_hash', 'md5'),
    '#description' => t("Please, select hash algorithm"),
  );
  $form['rbk_money_simple_language'] = array(
    '#type' => 'select',
    '#title' => t('Interface language'),
    '#options' => array('ru' => 'ru (default)', 'en' => 'en'),
    '#default_value' => variable_get('rbk_money_simple_language', 'ru'),
    '#description' => t("Please, select payment page interface language"),
  );
  $form['rbk_money_simple_log'] = array(
    '#type' => 'radios',
    '#title' => t("Save RBK Money's server responses in <a href='!dblog'>system log</a>", array('!dblog' => 'admin/reports/dblog')),
    '#options' => array(1 => t('Yes'), 0 => t('No')),
    '#default_value' => variable_get('rbk_money_simple_log', 1),
  );
  $form['rbk_money_simple_ip'] = array(
    '#type' => 'radios',
    '#title' => t('Process notifications only from allowed IP addresses'),
    '#options' => array(1 => t('Yes'), 0 => t('No')),
    '#default_value' => variable_get('rbk_money_simple_ip', 0),
    '#description' => t('Allowed IP addresses: ') . implode(', ', unserialize(RBK_MONEY_SIMPLE_IP_LIST)),
  );
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'rbk_money_simple') . '/rbk_money_simple.js',
  );

  return system_settings_form($form);
}


/*
 * Payments page
 */

function rbk_money_simple_payments($form, $form_state) {
  if (!empty($_POST['payments']) && isset($_POST['operation']) && ($_POST['operation'] == 'delete')) {
    return drupal_get_form('rbk_money_simple_multiple_delete');
  }

  $header = array(
    'pid' => array('data' => 'ID', 'field' => 'r.pid', 'sort' => 'desc'),
    'created' => array('data' => t('Created'), 'field' => 'r.created'),
    'name' => array('data' => t('User'), 'field' => 'r.uid'),
    'amount' => array('data' => t('Amount'), 'field' => 'r.amount'),
    'description' => t('Description'),
    'status' => array('data' => t('Status'), 'field' => 'r.status'),
  );

  $result = db_select('rbk_money_simple', 'r')
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->fields('r', array('pid', 'uid', 'created', 'amount', 'description', 'status'))
    ->orderByHeader($header)
    ->limit(50)
    ->execute();

  $payments = array();
  foreach ($result as $payment) {
    $user = array('account' => user_load($payment->uid));
    $payments[$payment->pid] = array(
      'pid' => "<label for=\"edit-payments-{$payment->pid}\">{$payment->pid}</label>",
      'created' => date("d.m.Y H:i", $payment->created),
      'name' => theme('username', $user),
      'amount' => $payment->amount . ' ' . variable_get('rbk_money_simple_recipientCurrency', 'RUR'),
      'description' => $payment->description,
      'status' => (($payment->status > 0) ? ($payment->status == RBK_MONEY_SIMPLE_STATUS_PROCESS ? t('Processing') : t('Successful')) : t('Pending'))
    );
  }

  $form['payments'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $payments,
    '#empty' => t('No payments available.'),
  );
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#attributes' => array('class' => array('container-inline')),
    '#weight' => -1
  );
  $form['options']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#title_display' => 'invisible',
    '#options' => array(
      'delete' => t('Delete selected'),
      'enroll' => t('Enroll selected')
    ),
    '#default_value' => 'delete',
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Apply'),
  );
  $form['pager'] = array('#markup' => theme('pager'));

  return $form;
}

function rbk_money_simple_payments_validate($form, &$form_state) {
  if (empty($form_state['input']['payments']) || count(array_filter($form_state['input']['payments'])) == 0)  {
    form_set_error('', t('No payments selected.'));
  }
}

function rbk_money_simple_payments_submit($form, &$form_state) {
  $payments = array_filter($form_state['input']['payments']); // array_filter returns only elements with TRUE values
  if($form_state['input']['operation'] == 'enroll'){
      foreach ($payments as $pid) {
        rbk_money_simple_payment_enroll($pid, RBK_MONEY_SIMPLE_STATUS_SUCCESS);
      }
    $msg = format_plural(count($payments),
            t('The payment have been enrolled'),
            t('The payments have been enrolled'));
    drupal_set_message($msg);
  }
}

function rbk_money_simple_multiple_delete($form, &$form_state) {
  $form['payments'] = array('#tree' => TRUE);
  $payments = array_filter($form_state['input']['payments']);
  foreach ($payments as $pid => $value) {
    $form['payments'][$pid] = array('#type' => 'hidden', '#value' => $pid);
  }
  $form['operation'] = array('#type' => 'hidden', '#value' => 'delete');

  $question = format_plural(count($payments),
              'Are you sure you want to delete this item?',
              'Are you sure you want to delete these items?');

  return confirm_form($form, $question, 'admin/config/rbk_money_simple/payments');

}

function rbk_money_simple_multiple_delete_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $payments = $form_state['values']['payments'];
    foreach ($payments as $payment => $pid) {
      rbk_money_simple_payment_delete($pid);
    }
    $msg = format_plural(count($payments), t('Deleted 1 payment.'), t('Deleted @count payments'));
    drupal_set_message($msg);
  }
  $form_state['redirect'] = 'admin/config/rbk_money_simple/payments';
}

// TODO: payment delete confirmation