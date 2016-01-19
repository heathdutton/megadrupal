<?php

/**
 * @file
 * Contains callbacks for pages.
 */

/**
 * Page callback: the redirect form.
 */
function transferuj_pl_form_redirect(array $form, array &$form_state, Payment $payment) {
  $form['#action'] = $payment->method->controller->redirectUrl;

  foreach ($payment->method->controller->redirectData($payment) as $parameter => $value) {
    $form[$parameter] = array(
      '#type' => 'hidden',
      '#value' => $value,
    );
  }

  $form['message'] = array(
    '#type' => 'markup',
    '#markup' => '<p>' . t('You will be redirected to the off-site payment server to authorize the payment.') . '</p>',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Continue'),
  );

  $form['#attached']['js'] = array(
    drupal_get_path('module', 'transferuj_pl') . '/js/transferuj_pl_redirect.js',
  );

  return $form;
}


/**
 * Access callback for the redirect form.
 */
function transferuj_pl_form_redirect_access(Payment $payment) {
  return is_a($payment->method->controller, 'TransferujPlPaymentMethodController')
    && payment_status_is_or_has_ancestor($payment->getStatus()->status, PAYMENT_STATUS_PENDING)
    && isset($_SESSION['transferuj_pl_pid']) && $_SESSION['transferuj_pl_pid'] == $payment->pid;
}


/**
 * Page callback: returning page.
 */
function transferuj_pl_return(Payment $payment) {
  unset($_SESSION['transferuj_pl_pid']);
  $payment->finish();
}


/**
 * Access callback for the returning page.
 */
function transferuj_pl_return_access(Payment $payment) {
  return isset($_SESSION['transferuj_pl_pid']) && $_SESSION['transferuj_pl_pid'] == $payment->pid;
}


/**
 * Page callback: return page.
 */
function transferuj_pl_verify() {
  $params = drupal_get_query_parameters($_POST);

  $pid = explode(':', $params['tr_crc'])[0];
  $payment = entity_load_single('payment', $pid);

  $payment->method->controller->processFeedback($params, $payment);
  echo "TRUE";
}


/**
 * Checks access for the return URL.
 */
function transferuj_pl_verify_access() {
  $params = drupal_get_query_parameters($_POST);

  if (empty($params)) {
    return FALSE;
  }

  if (!in_array(ip_address(), TransferujPlPaymentMethodController::$returnIPs)) {
    return FALSE;
  }

  $pid = explode(':', $params['tr_crc'])[0];
  $payment = entity_load_single('payment', $pid);
  $security_code = $payment->method->controller_data['security_code'];

  if (md5($params['id'] . $params['tr_id'] . $params['tr_amount'] . $params['tr_crc'] . $security_code) != $params['md5sum']) {
    return FALSE;
  }

  return TRUE;
}
