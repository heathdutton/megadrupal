<?php

/**
 * @file
 * Determines if the transaction is success or failure
 * and redirects accordingly.
 */

$current_wd = getcwd();
include 'header.php';
chdir($current_wd);

/*////////////////////////////////////////////////////////////////////*/
 $query = db_select('payment_hdfc_config', 'n');
 $query->condition('n.fixedcol', '1020', '=')
       ->fields('n', array('site_base_url'));
 $sql = $query->execute()->fetchField();
 $db_site_base_url = $sql;

/*////////////////////////////////////////////////////////////////////*/

$transaction_id = isset($_REQUEST['restranid']) ? $_REQUEST['restranid'] : '';
$track_id = isset($_REQUEST['restrackid']) ? $_REQUEST['restrackid'] : '';
$amount = isset($_REQUEST['resamount']) ? $_REQUEST['resamount'] : '';
$fcode = isset($_REQUEST['resresult']) ? $_REQUEST['resresult'] : '';
$pay_id = isset($_REQUEST['respaymentid']) ? $_REQUEST['respaymentid'] : '';
$refer_no = isset($_REQUEST['resref']) ? $_REQUEST['resref'] : '';
$auth_no = isset($_REQUEST['resauth']) ? $_REQUEST['resauth'] : '';
$avr_no = isset($_REQUEST['resavr']) ? $_REQUEST['resavr'] : '';
$error_text = isset($_REQUEST['errortext']) ? $_REQUEST['errortext'] : '';
$error_no = isset($_REQUEST['errorno']) ? $_REQUEST['errorno'] : '';

/*////////////////////////Fetch AMOUNT from DB////////////////*/
 $query = db_select('field_data_commerce_order_total', 't');
 $query->join('commerce_order', 'n', 'n.revision_id = t.revision_id');
 $query->fields('t', array('commerce_order_total_amount'))
       ->condition('n.order_number', $track_id, '=');
 $sql = $query->execute()->fetchField();
 $row = $sql;
 $db_amount = floor($row/100);
 $trimamt = explode(".", "$amount");
 $trimamtval = $trimamt[0];

/*/////////////////////////Fetch TRACK-ID from DB////////////////*/
 $query = db_select('field_data_commerce_order_total', 't');
 $query->join('commerce_order', 'n', 'n.revision_id = t.revision_id');
 $query->fields('n', array('order_number'))
       ->condition('n.order_number', $track_id, '=');
 $sql = $query->execute()->fetchField();
 $db_orderid = $sql;

/*////////////Fetch Payment-ID from DB/////////////*/
 $query = db_select('field_data_commerce_order_total', 't');
 $query->join('commerce_order', 'n', 'n.revision_id = t.revision_id');
 $query->fields('n', array('pymid'))
       ->condition('n.order_number', $track_id, '=');
 $sql = $query->execute()->fetchField();
 $db_payid = $sql;

/*//////////Fetch Result from DB//////////////////*/
 $query = db_select('field_data_commerce_order_total', 't');
 $query->join('commerce_order', 'n', 'n.revision_id = t.revision_id');
 $query->fields('n', array('result'))
       ->condition('n.order_number', $track_id, '=');
 $sql = $query->execute()->fetchField();
 $db_result = $sql;

/*//////////////Fetch Transaction Id from DB///////////////////*/
 $query = db_select('field_data_commerce_order_total', 't');
 $query->join('commerce_order', 'n', 'n.revision_id = t.revision_id');
 $query->fields('n', array('tranid'))
       ->condition('n.order_number', $track_id, '=');
 $sql = $query->execute()->fetchField();
 $db_tranid = $sql;

/*//////Fetch Ref. No. from DB///////////////////*/
 $query = db_select('field_data_commerce_order_total', 't');
 $query->join('commerce_order', 'n', 'n.revision_id = t.revision_id');
 $query->fields('n', array('refno'))
       ->condition('n.order_number', $track_id, '=');
 $sql = $query->execute()->fetchField();
 $db_refno = $sql;

/*/////////////Fetch Auth. No. from DB//////////////*/
 $query = db_select('field_data_commerce_order_total', 't');
 $query->join('commerce_order', 'n', 'n.revision_id = t.revision_id');
 $query->fields('n', array('auth'))
       ->condition('n.order_number', $track_id, '=');
 $sql = $query->execute()->fetchField();
 $db_auth = $sql;

/*////////////Fetch AVR No. from DB/////////////*/
 $query = db_select('field_data_commerce_order_total', 't');
 $query->join('commerce_order', 'n', 'n.revision_id = t.revision_id');
 $query->fields('n', array('avr'))
       ->condition('n.order_number', $track_id, '=');
 $sql = $query->execute()->fetchField();
 $db_avr = $sql;

/*////////////Fetch Order Number from DB/////////////*/
 $query = db_select('field_data_commerce_order_total', 't');
 $query->join('commerce_order', 'n', 'n.revision_id = t.revision_id');
 $query->fields('n', array('order_number'))
       ->condition('n.order_number', $track_id, '=');
 $sql = $query->execute()->fetchField();
 $db_ordnum = $sql;

/*////////////Fetch Order Id from DB/////////////*/
 $query = db_select('commerce_order', 'n');
 $query->condition('n.order_number', $track_id, '=')
       ->fields('n', array('order_id'));
 $sql = $query->execute()->fetchField();
 $db_id_order = $sql;

/*///////////////////////////////////////////////////////////////////*/

if($trimamtval == $db_amount && $track_id == $db_orderid && $pay_id == $db_payid && $fcode == $db_result && $refer_no == $db_refno && $transaction_id == $db_tranid && $auth_no == $db_auth && $avr_no == $db_avr)
{
  $payment_amt = $amount * 100;
  $commerce_order = commerce_order_load($db_ordnum);
  $name = 'checkout_complete';
  $order_success = commerce_order_status_update($commerce_order, $name, $skip_save = FALSE, $revision = TRUE, $log = '');
  commerce_checkout_complete($order_success);
  $wrapper = entity_metadata_wrapper('commerce_order', $commerce_order);
  $currency = $wrapper->commerce_order_total->currency_code->value();
  $transaction = commerce_payment_transaction_new('hdfc', $db_id_order);
  $transaction->amount = $payment_amt;
  $transaction->message = t('Payment received at') . ' ' . date("d-m-Y H:i:s", REQUEST_TIME);
  $transaction->currency_code = $currency;
  $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
  commerce_payment_transaction_save($transaction);
  commerce_payment_redirect_pane_next_page($order_success);
  $url_success = $db_site_base_url . '/checkout/' . $db_id_order . '/complete';
  header("location:". $url_success);
}

else
{
  $commerce_order = commerce_order_load($db_ordnum);
  $name = 'checkout_checkout';
  $order_failure = commerce_order_status_update($commerce_order, $name, $skip_save = FALSE, $revision = TRUE, $log = '');
  $transaction = commerce_payment_transaction_new('hdfc', $db_id_order);
  $transaction->message = t('There was a problem with your order: !response_code  !reason_text', array('!response_code' => check_plain($error_no), '!reason_text' => $error_text));
  $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
  commerce_payment_transaction_save($transaction);
  commerce_payment_redirect_pane_previous_page($order_failure);
  $url_failure = $db_site_base_url . '/checkout/' . $db_id_order . '/review';
  header("location:". $url_failure);
}
