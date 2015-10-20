<?php
function robokassa_prefillform($form, &$form_state) {
  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount'),
    '#default_value' => '',
    '#size' => 10,
    '#maxlength' => 12,
    '#field_suffix' => variable_get('robokassa_currency_suffix', ' руб.'),
    '#required' => TRUE
  );

  $form['memo'] = array(
    '#type' => 'textarea',
    '#title' => t('Memo'),
    '#default_value' => t('Payment to !sitename', array('!sitename' => variable_get('site_name', 'Drupal'))),
    '#description' => t("Payment description."),
    '#required' => TRUE
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Cerate payment'),
  );
  return $form;
}

function robokassa_prefillform_submit(&$form, $form_state) {
  $payment = _robokassa_createpayment(array(
    'amount' => $form_state['values']['amount'],
    'memo' => $form_state['values']['memo'],
  ));

  if (is_array($payment) && $payment['pid'] > 0) {
    drupal_set_message(t("Please confirm payment details"));
    drupal_goto('robokassa/payment/' . $payment['pid']);
  }
}

function robokassa_success() {
  return theme('robokassa_success');
}

function robokassa_fail() {
  return theme('robokassa_fail');
}

function robokassa_status() {
  global $base_path;
  drupal_add_http_header('Content-type', 'text/html; charset=iso-8859-1');


  // check url
  $url = trim($_SERVER['REQUEST_URI'], '/');
  $alias = trim($base_path . drupal_get_path_alias('robokassa/status'), '/');
  if ($url != $alias) {
    watchdog('Robokassa', 'Wrong Result URL accessed "%url"', array("%url" => $url), WATCHDOG_WARNING);
    die("bad sign");
  }
  $created = time();
  $out_summ = $_REQUEST["OutSum"];
  $inv_id = $_REQUEST["InvId"];
  $crc = strtoupper($_REQUEST["SignatureValue"]);
  $my_crc = strtoupper(md5("$out_summ:$inv_id:" . variable_get('robokassa_secretkey2', '')));
  if ($my_crc != $crc || !preg_match("/^[0-9]{1,10}$/", $_REQUEST["InvId"])) {
    watchdog('Robokassa', 'Bad sign, vars "%vars"', array("%vars" => print_r($_REQUEST,TRUE)), WATCHDOG_ALERT);
    die("bad sign\n");
  }
   $payment = _robokassa_pid_load($_REQUEST["InvId"]);
  if (!is_array($payment)) {
    watchdog('Robokassa', 'Bad invId, vars "%vars"', array("%vars" => print_r($_REQUEST,TRUE)), WATCHDOG_ALERT);
    die("bad InvId\n");
  }
   // check if payment already enrolled
  if ($payment['enrolled'] > 0) {
    watchdog('Robokassa', 'Order InvID %invid already enrolled', array("%invid" => $_REQUEST['InvId']), WATCHDOG_WARNING);
    die("bad sign\n");
  }

  if ($_REQUEST["OutSum"] == round($payment['amount'], 2)) {
    _robokassa_enrollpayment($payment['pid'], $created);
    die("OK$inv_id\n");
  }
  else { // you can also save invalid payments for debug purposes
    watchdog('Robokassa', "Can't process order InvID %invid, vars '%vars'", array(
      "%invid" => $_REQUEST['InvId'],
      '%vars' => print_r($_REQUEST,TRUE)
    ), WATCHDOG_ERROR);
    die();
  }
}