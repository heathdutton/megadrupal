<?php
/**
 * @file
 * Getting data from mobilpay.
 */

/**
 * Add keys provided by Mobilpay.
 */
function commerce_mobilpay_keys($form, &$form_state) {
  $form['private_key'] = array(
    '#name' => 'private_key',
    '#type' => 'managed_file',
    '#title' => t('Upload private key'),
    '#default_value' => variable_get('commerce_mobilpay_key_private', ''),
    '#description' => t('Upload private key that you received from Mobilpay.'),'#upload_validators' => array(
      'file_validate_extensions' => array('key'),
    ),
    '#upload_location' => 'private://',
  );

  $form['public_key'] = array(
    '#name' => 'public_key',
    '#type' => 'managed_file',
    '#title' => t('Upload public key'),
    '#default_value' => variable_get('commerce_mobilpay_key_public', ''),
    '#description' => t('Upload public key that you received from Mobilpay.'),'#upload_validators' => array(
      'file_validate_extensions' => array('cer'),
    ),
    '#upload_location' => 'private://',
  );

  $form['confirm_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Succes URL'),
    '#description' => t('URL to the page when payment is pending or approved'),
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_mobilpay_confirmurl', ''),
  );

  $form['cancel_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Cancel Url'),
    '#description' => t('URL to the page when payment is canceled'),
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_mobilpay_cancelurl', ''),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Add keys provided by Mobilpay.
 */
function commerce_mobilpay_keys_submit($form, $form_state) {
  $private_key = file_load($form_state['values']['private_key']);
  $public_key = file_load($form_state['values']['public_key']);

  // Save as permanent files.
  if ($private_key) {
    $private_key->status = 1;
    file_save($private_key);
    variable_set('commerce_mobilpay_key_private', $private_key->fid);
    drupal_set_message(t('Private key saved.'), 'status');
  }
  if ($public_key) {
    $public_key->status = 1;
    file_save($public_key);
    variable_set('commerce_mobilpay_key_public', $public_key->fid);
    drupal_set_message(t('Public key saved.'), 'status');
  }

  // Set variables for use in commerce_mobilpay_complete().
  variable_set('commerce_mobilpay_confirmurl', $form_state['values']['confirm_url']);
  variable_set('commerce_mobilpay_cancelurl', $form_state['values']['cancel_url']);
}

/**
 * Confirm payment.
 */
function commerce_mobilpay_confirm() {
  $file = file_load(variable_get('commerce_mobilpay_key_private'));
  $private_key = drupal_realpath($file->uri);

  drupal_add_http_header('Content-Type', 'application/xml; utf-8');
  libraries_load('Mobilpay');

  $error_code = 0;
  $error_type = Mobilpay_Payment_Request_Abstract::CONFIRM_ERROR_TYPE_NONE;
  $error_message = '';
  if (strcasecmp($_SERVER['REQUEST_METHOD'], 'post') == 0) {
    if (isset($_POST['env_key']) && isset($_POST['data'])) {
      try {
        $obj_pm_req = Mobilpay_Payment_Request_Abstract::factoryFromEncrypted($_POST['env_key'], $_POST['data'], $private_key);
        $id_comanda = $obj_pm_req->orderId;
        $order = commerce_order_load($id_comanda);
        $uid = $obj_pm_req->params['uid'];
        $paid = number_format(floatval($obj_pm_req->objPmNotify->processedAmount * 100), 2, '.', '');
        switch ($obj_pm_req->objPmNotify->action) {
          // Any actions has an error and a message.
          // This could be readed by using:
          // $cod_eroare = $obj_pm_req->objPmNotify->errorCode;
          // $mesaj_eroare = $obj_pm_req->objPmNotify->errorMessage;
          case 'confirmed':
            $transaction = commerce_payment_transaction_new('commerce_mobilpay', $id_comanda);
            $transaction->instance_id = $obj_pm_req->objPmNotify->purchaseId;
            $transaction->amount = $paid;
            $transaction->currency_code = 'RON';
            $transaction->payload[REQUEST_TIME] = $obj_pm_req->objPmNotify->timestamp;
            $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
            $transaction->remote_id = $obj_pm_req->objPmNotify->purchaseId;
            $transaction->remote_status = $obj_pm_req->objPmNotify->action;
            $transaction->uid = $uid;
            $transaction->revision_uid = $uid;
            $transaction->message = t('Payment confirmed');
            $result = commerce_payment_transaction_save($transaction);
            commerce_checkout_complete($order);

            // When action is confirmed we know that,
            // money has been received.
            $error_message = $obj_pm_req->objPmNotify->getCrc();
            break;

          case 'confirmed_pending':
            // When action is confirmed_pending,
            // transaction it is verified for fraud.
            $error_message = $obj_pm_req->objPmNotify->getCrc();
            $transaction = commerce_payment_transaction_new('commerce_mobilpay', $id_comanda);
            $transaction->instance_id = $obj_pm_req->objPmNotify->purchaseId;
            $transaction->amount = $paid;
            $transaction->currency_code = 'RON';
            $transaction->payload[REQUEST_TIME] = $obj_pm_req->objPmNotify->timestamp;
            $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
            $transaction->remote_id = $obj_pm_req->objPmNotify->purchaseId;
            $transaction->uid = $uid;
            $transaction->revision_uid = $uid;
            $transaction->remote_status = $obj_pm_req->objPmNotify->action;
            $transaction->message = t('Payment waiting at Mobilpay.');
            $result = commerce_payment_transaction_save($transaction);

            break;

          case 'paid_pending':
            // When action is paid_pending,
            // transaction it's verified.
            $error_message = $obj_pm_req->objPmNotify->getCrc();
            $error_message = $obj_pm_req->objPmNotify->getCrc();
            $transaction = commerce_payment_transaction_new('commerce_mobilpay', $id_comanda);
            $transaction->instance_id = $obj_pm_req->objPmNotify->purchaseId;
            $transaction->amount = $paid;
            $transaction->currency_code = 'RON';
            $transaction->payload[REQUEST_TIME] = $obj_pm_req->objPmNotify->timestamp;
            $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
            $transaction->remote_id = $obj_pm_req->objPmNotify->purchaseId;
            $transaction->uid = $uid;
            $transaction->revision_uid = $uid;
            $transaction->remote_status = $obj_pm_req->objPmNotify->action;
            $transaction->message = t('Payment waiting at Mobilpay.');
            $result = commerce_payment_transaction_save($transaction);

            break;

          case 'paid':
            // When action is paid,
            // transaction it's pending.
            $error_message = $obj_pm_req->objPmNotify->getCrc();
            $error_message = $obj_pm_req->objPmNotify->getCrc();
            $transaction = commerce_payment_transaction_new('commerce_mobilpay', $id_comanda);
            $transaction->instance_id = $obj_pm_req->objPmNotify->purchaseId;
            $transaction->amount = $paid;
            $transaction->currency_code = 'RON';
            $transaction->payload[REQUEST_TIME] = $obj_pm_req->objPmNotify->timestamp;
            $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
            $transaction->remote_id = $obj_pm_req->objPmNotify->purchaseId;
            $transaction->uid = $uid;
            $transaction->revision_uid = $uid;
            $transaction->remote_status = $obj_pm_req->objPmNotify->action;
            $transaction->message = t('Payment confirmed but waiting at Mobilpay.');
            $result = commerce_payment_transaction_save($transaction);

            break;

          case 'canceled':
            // When action it's canceled,
            // transaction was canceled.
            $error_message = $obj_pm_req->objPmNotify->getCrc();
            $error_message = $obj_pm_req->objPmNotify->getCrc();
            $transaction = commerce_payment_transaction_new('commerce_mobilpay', $id_comanda);
            $transaction->instance_id = $obj_pm_req->objPmNotify->purchaseId;
            $transaction->currency_code = 'RON';
            $transaction->amount = $paid;
            $transaction->payload[REQUEST_TIME] = $obj_pm_req->objPmNotify->timestamp;
            $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
            $transaction->remote_id = $obj_pm_req->objPmNotify->purchaseId;
            $transaction->uid = $uid;
            $transaction->revision_uid = $uid;
            $transaction->remote_status = $obj_pm_req->action;
            $transaction->message = t('Payment canceled.');
            $result = commerce_payment_transaction_save($transaction);

            break;

          case 'credit':
            // When action is credit,
            // money is refunded.
            $error_message = $obj_pm_req->objPmNotify->getCrc();
            $error_message = $obj_pm_req->objPmNotify->getCrc();
            $error_message = $obj_pm_req->objPmNotify->getCrc();
            $transaction = commerce_payment_transaction_new('commerce_mobilpay', $id_comanda);
            $transaction->instance_id = $obj_pm_req->objPmNotify->purchaseId;
            $transaction->currency_code = 'RON';
            $transaction->amount = $paid;
            $transaction->payload[REQUEST_TIME] = $obj_pm_req->objPmNotify->timestamp;
            $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
            $transaction->remote_id = $obj_pm_req->objPmNotify->purchaseId;
            $transaction->uid = $uid;
            $transaction->revision_uid = $uid;
            $transaction->remote_status = $obj_pm_req->objPmNotify->action;
            $transaction->message = t('Payment refunded.');
            $result = commerce_payment_transaction_save($transaction);

            break;

          default:
            $error_type   = Mobilpay_Payment_Request_Abstract::CONFIRM_ERROR_TYPE_PERMANENT;
            $error_code     = Mobilpay_Payment_Request_Abstract::ERROR_CONFIRM_INVALID_ACTION;
            $error_message  = 'mobilpay_refference_action paramaters is invalid';
            break;

        }
      }
      catch (Exception $e) {
        $error_type     = Mobilpay_Payment_Request_Abstract::CONFIRM_ERROR_TYPE_TEMPORARY;
        $error_code   = $e->getCode();
        $error_message  = $e->getMessage();
      }
    }
    else {
      $error_type     = Mobilpay_Payment_Request_Abstract::CONFIRM_ERROR_TYPE_PERMANENT;
      $error_code   = Mobilpay_Payment_Request_Abstract::ERROR_CONFIRM_INVALID_POST_PARAMETERS;
      $error_message  = 'mobilpay.ro posted invalid parameters';
    }
  }
  else {
    $error_type     = Mobilpay_Payment_Request_Abstract::CONFIRM_ERROR_TYPE_PERMANENT;
    $error_code   = Mobilpay_Payment_Request_Abstract::ERROR_CONFIRM_INVALID_POST_METHOD;
    $error_message  = 'invalid request metod for payment confirmation';
  }

  echo "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
  if ($error_code == 0) {
    printf('<crc>%s</crc>', $error_message);
  }
  else {
    printf('<crc error_type="%s" error_code="%s">%s</crc>', $error_type, $error_code, $error_message);
  }
}

/**
 * Load payment and redirect to confirm/cancel page.
 */
function commerce_mobilpay_complete() {
  $id_comanda = $_GET['orderId'];
  // Load url settings;
  $confirm_url = variable_get('commerce_mobilpay_confirmurl');
  $cancel_url = variable_get('commerce_mobilpay_cancelurl');

  if ($id_comanda != '') {
    $result = db_query('SELECT n.transaction_id FROM {commerce_payment_transaction} n WHERE n.order_id = :id_comanda', array(':id_comanda' => $id_comanda));
    foreach ($result as $record) {
      $tid = $record->transaction_id;
    }
    $transaction = commerce_payment_transaction_load($tid);

    if ($transaction->remote_status == 'canceled') {
      drupal_set_message($transaction->message);
      drupal_goto($cancel_url);
    }
    else {
      drupal_set_message($transaction->message);
      drupal_goto($confirm_url);
    }
  }
  else {
    drupal_set_message(t('Something went wrong, please contact the site administrator.'));
    drupal_goto($cancel_url);
  }
}
