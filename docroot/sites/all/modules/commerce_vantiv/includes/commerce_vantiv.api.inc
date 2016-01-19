<?php
/**
 * API and hooks documentation for the Commerce Litle module.
 */

class VantivApi {
  /**
   * Set merchant credentials.
   */
  public function __construct($settings) {
    $this->account = !empty($settings['account']) ? trim($settings['account']) : '';
    $this->user = !empty($settings['user']) ? trim($settings['user']) : '';
    $this->password = !empty($settings['password']) ? trim($settings['password']) : '';
    $this->merchant_id = !empty($settings['merchant_id']) ? trim($settings['merchant_id']) : '';
    $this->three_d = !empty($settings['3d_secure']) ? trim($settings['3d_secure']) : '';
    $this->transaction_type_process = !empty($settings['transaction_type_process']) ? trim($settings['transaction_type_process']) : '';
    $this->advance_fraud = !empty($settings['advance_fraud']) ? trim($settings['advance_fraud']) : '';
    $this->advance_fraud_key = !empty($settings['advance_fraud_key']) ? trim($settings['advance_fraud_key']) : '';
    $this->recycling = !empty($settings['recycling']) ? trim($settings['recycling']) : '1';
    $this->logs = !empty($settings['logs']) ? trim($settings['logs']) : '';
    $this->libraryLoad();
  }

  /**
   * Loads the library functions needed to process transactions.
   */
  public function libraryLoad() {
    if (defined('STDIN') != TRUE) {
      define('STDIN',fopen("php://stdin","r"));
    }
    $library = libraries_info('litle');
    $load_library = libraries_load('litle');
    libraries_load_files($load_library);
  }

  /**
   * Process direct payments.
   */
  public function directPayments($customer_profile, $order, $card_info = '', $payment_method, $amount = '', $token = '', $fraud_id = '', $echeck = '', $end_echeck = '') {
    $currency_code = empty($amount->currency_code) ? $order->commerce_order_total['und'][0]['currency_code'] : $amount->currency_code;
    $charge_amount = empty($amount->amount) ? $order->commerce_order_total['und'][0]['amount'] : $amount->amount;
    $card_owner_name = $customer_profile->commerce_customer_address['und'][0]['name_line'];
    $user = user_load($order->uid);
    if (empty($token)) {
      $billing_data = array(
        'user' => $this->user,
        'password' => $this->password,
        'merchantId' => $this->merchant_id,
        'reportGroup' => 'rtpGrp',
        'url' => $this->accountLinks($this->account),
        'litle_requests_path' => $this->accountLinks($this->account),
        'batch_requests_path' => '',
        'proxy' => '',
        'sftp_username' => '',
        'sftp_password' => '',
        'batch_url' => '',
        'tcp_port' => '',
        'tcp_ssl' => '',
        'tcp_timeout' => '',
        'print_xml' => '',
        'orderId' => $order->order_id,
  		  'amount' => $charge_amount,
        'email' => 'nobody@litle.com',
        //id of auth or sale response in the xml tag.
        'id'=> $order->order_id . '|' . time(),
  		  'orderSource'=>'ecommerce',
  		  'billToAddress'=>array(
  		  'name' => $customer_profile->commerce_customer_address['und'][0]['name_line'],
  		  'addressLine1' => $customer_profile->commerce_customer_address['und'][0]['thoroughfare'],
  		  'city' => $customer_profile->commerce_customer_address['und'][0]['locality'],
  		  'state' => $customer_profile->commerce_customer_address['und'][0]['administrative_area'],
  		  'zip' => $customer_profile->commerce_customer_address['und'][0]['postal_code'],
  		  'country' => $customer_profile->commerce_customer_address['und'][0]['country']),
  		  'card' => array(
    		  'number' => (!empty($card_info['credit_card']['number'])) ? $card_info['credit_card']['number'] : '',
    		  'cardValidationNum' => (!empty($card_info['credit_card']['code'])) ? $card_info['credit_card']['code'] : '',
  		  ),
      );

      if (!empty($card_info['credit_card']['number']) and !empty($card_info['credit_card']['exp_month']) and !empty($card_info['credit_card']['exp_year'])) {
        $billing_data['card']['expDate'] = $card_info['credit_card']['exp_month'] . substr($card_info['credit_card']['exp_year'], 2, 4);
        $billing_data['card']['type'] = $this->cardTypes($card_info['credit_card']['type']);
      }

      if (!empty($fraud_id) and $this->advance_fraud == '1') {
        $billing_data['advancedFraudChecks'] = array(
          'threatMetrixSessionId' => $fraud_id,
        );
        $billing_data['threatMetrixSessionId'] = $fraud_id;
      }
    }
    else {
      $billing_data = array(
        'user' => $this->user,
        'password' => $this->password,
        'merchantId' => $this->merchant_id,
        'reportGroup' => 'rtpGrp',
        'url' => $this->accountLinks($this->account),
        'litle_requests_path' => $this->accountLinks($this->account),
        'batch_requests_path' => '',
        'proxy' => '',
        'sftp_username' => '',
        'sftp_password' => '',
        'batch_url' => '',
        'tcp_port' => '',
        'tcp_ssl' => '',
        'tcp_timeout' => '',
        'print_xml' => '',
        'orderId' => $order->order_id,
  		  'amount' => $charge_amount,
        //id of auth or sale response in the xml tag.
        'id'=> $order->order_id . '|' . time(),
  		  'orderSource'=>'ecommerce',
  		  'billToAddress' => array(
  		  'name' => $customer_profile->commerce_customer_address['und'][0]['name_line'],
  	   	'addressLine1' => $customer_profile->commerce_customer_address['und'][0]['thoroughfare'],
  		  'city' => $customer_profile->commerce_customer_address['und'][0]['locality'],
  		  'state' => $customer_profile->commerce_customer_address['und'][0]['administrative_area'],
  		  'zip' => $customer_profile->commerce_customer_address['und'][0]['postal_code'],
  		  'country' => $customer_profile->commerce_customer_address['und'][0]['country']),
        /*'token' => array(
          'litleToken' => $token,
        ),*/
      );
      if ($payment_method['method_id'] == 'vantiv_direct') {
        $billing_data['token'] = array(
          'litleToken' => $token,
        );
      }
    }

    if ($this->recycling == '1') {
      $billing_data['recyclingRequest'] = array(
        'recycleBy' => 'Litle',
        'recycleId' =>  time(),
      );
    }

    $initilaize = new litle\sdk\LitleOnlineRequest();

    if (!empty($card_info['credit_card']['credit']) and $card_info['credit_card']['credit']) {
      $response = $initilaize->creditRequest($billing_data);
    }
    // Check if this is credit card transaction.
    if ($echeck != TRUE) {
      if ($this->transaction_type_process == 'pre_auth' and (is_object($card_info) == TRUE or empty($card_info['credit_card']['cardonfile_store']))) {
        $response = $initilaize->authorizationRequest($billing_data);
      }
      elseif ($this->transaction_type_process == 'sale' and (is_object($card_info) == TRUE or empty($card_info['credit_card']['cardonfile_store'])) and empty($card_info['credit_card']['credit'])) {
        $response = $initilaize->saleRequest($billing_data);
      }
      elseif (!empty($card_info['credit_card']['cardonfile_store']) /*and $card_info['credit_card']['cardonfile_store'] == TRUE*/ and is_object($card_info) != TRUE and is_array($card_info) == TRUE and $card_info['credit_card']['cardonfile_store'] == TRUE) {
        $billing_data['accountNumber'] = $card_info['credit_card']['number'];
        $response = $initilaize->registerTokenRequest($billing_data);

      }
      //$response_code = litle\sdk\XmlParser::getNode($response, 'response');
      if (!empty($response)) {
        $this->processDirectTransactions($response, $payment_method, '', $card_info);
      }
      else {
        watchdog('commerce_vantiv', "An error occur with order# @order during the request, the server did not respond.", array('@order' => $order->order_id), WATCHDOG_ERROR);
      }
    }
    // If this is an eCheck transaction.
    elseif ($echeck == TRUE) {
      if  (module_exists('commerce_shipping')) {
        unset($customer_profile);
        $customer_profile = commerce_customer_profile_load($order->commerce_customer_shipping['und'][0]['profile_id']);
      }

      $billing_data['shipToAddress'] = array(
        'name' => $customer_profile->commerce_customer_address['und'][0]['name_line'],
        'city' => $customer_profile->commerce_customer_address['und'][0]['locality'],
        'state' => '',
        'email' => $order->mail,
      );
      if (empty($token)) {
        $billing_data['echeck'] = array(
          'accType' => $card_info['accType'],
          'accNum' => $card_info['accNum'],
          'routingNum' => $card_info['routingNum'],
          'checkNum' => $card_info['checkNum'],
        );
      }
      elseif (!empty($token) and (!empty($card_info['new_card_id']) or !empty($card_info['cardonfile']))) {
        $card = !empty($card_info['new_card_id']) ? commerce_cardonfile_load($card_info['new_card_id']) : commerce_cardonfile_load($card_info['cardonfile']);
        $billing_data['echeckToken'] = array(
          'litleToken' => $token,
          'accType' => $card_info['accType'],
          'routingNum' => $card_info['routingNum'],
          'checkNum' => $card_info['checkNum'],
        );
      }
      elseif (!empty($token) and !empty($card_info['credit']) and $card_info['credit'] == TRUE) {
        $billing_data['echeckToken'] = array(
          'litleToken' => $token,
          'accType' => $card_info['accType'],
          'routingNum' => $card_info['routingNum'],
          'checkNum' => $card_info['checkNum'],
        );
      }

     if (!empty($card_info['credit']) and $card_info['credit'] == TRUE) {
       $response = $initilaize->echeckCreditRequest($billing_data);
       $payload = litle\sdk\XmlParser::getDomDocumentAsString($response);
     }

      if (!empty($card_info['credit_card']['cardonfile_store']) and is_object($card_info) != TRUE and is_array($card_info) == TRUE and $card_info['credit_card']['cardonfile_store'] == TRUE and empty($card_info['new_card_id'])) {
        $billing_data['accountNumber'] = $card_info['accNum'];
        $response = $initilaize->registerTokenRequest($billing_data);
      }
      elseif (empty($card_info['credit'])) {
        $response = $initilaize->echeckSaleRequest($billing_data);
      }
      if (!empty($response)) {
        return $this->processDirectTransactions($response, $payment_method, '', $card_info);
      }
      else {
        watchdog('commerce_vantiv', "An error occur with order# @order during the request, the server did not respond.", array('@order' => $order->order_id), WATCHDOG_ERROR);
      }
    }
  }

  /**
   * Process cross payments.
   */
  public function crossPayment($transaction, $partial = '', $amount = '', $type = '', $batch = '') {
    $billing_data = array(
          'user' => $this->user,
          'password' => $this->password,
          'merchantId' => $this->merchant_id,
          'reportGroup' => 'rtpGrp',
          'url' => $this->accountLinks($this->account),
          'litle_requests_path' => $this->accountLinks($this->account),
          'batch_requests_path' => '',
          'proxy' => '',
          'sftp_username' => '',
          'sftp_password' => '',
          'batch_url' => '',
          'tcp_port' => '',
          'tcp_ssl' => '',
          'tcp_timeout' => '',
          'print_xml' => '',
          'id'=> $transaction->data['id'],
          'litleTxnId' => $transaction->remote_id,
    );

    // Add additional parameters for partial capture and refund transactions.
    if (!empty($partial) and $partial == TRUE and !empty($amount) and $type == 'capture') {
      $billing_data['partial'] = 'true';
      $billing_data['amount'] = $amount;
      $partial_amount = $amount;
    }
    elseif (!empty($partial) and $partial == TRUE and !empty($amount) and $type == 'refund') {
      $billing_data['amount'] = $amount;
      $partial_amount = $amount;
    }
    elseif (!empty($partial) and $partial == TRUE and !empty($amount) and $type == 'reversal') {
      $billing_data['amount'] = $amount;
      $partial_amount = $amount;
    }

    $init = new litle\sdk\LitleOnlineRequest();

    // Perform the http request.
    switch ($type) {
      case 'capture':
        $response = $init->captureRequest($billing_data);
        break;
      case 'refund':
        $response = $init->creditRequest($billing_data);
        break;
      case 'void':
        if ($transaction->payment_method == 'vantiv_direct') {
        $response = $init->voidRequest($billing_data);
        }
        elseif ($transaction->payment_method == 'vantiv_echeck') {
          $response = $init->echeckVoidRequest($billing_data);
        }
        break;
      case 'reversal':
        $response = $init->authReversalRequest($billing_data);
        break;
    }

    $payment_method = commerce_payment_method_instance_load($transaction->instance_id);

    // Saves partial or fully captured transaction.
    if (!empty($partial) and $partial == TRUE and !empty($amount) and $type == 'capture' /*and ($batch != TRUE or empty($batch))*/) {
      $this->processCapturedTransactions($response, $payment_method, $transaction, $partial_amount);
    }
    elseif ($type == 'capture' /*and ($batch != TRUE or empty($batch))*/) {
      return $this->processCapturedTransactions($response, $payment_method, $transaction, '', $batch);
    }

    // Saves partial or fully refund transaction.
    if (!empty($partial) and $partial == TRUE and !empty($amount) and $type == 'refund') {
      $this->processRefundedTransactions($response, $payment_method, $transaction, $partial_amount);
    }
    elseif ($type == 'refund') {
      $this->processRefundedTransactions($response, $payment_method, $transaction);
    }

    // Updates voided transaction.
    if ($type == 'void') {
      $this->processVoidTransaction($response, $payment_method, $transaction);
    }

    // Saves partial or full reversed auth transaction.
    if (!empty($partial) and $partial == TRUE and !empty($amount) and $type == 'reversal') {
      $this->processReversedTransactions($response, $payment_method, $transaction, $partial_amount);
    }
    elseif ($type == 'reversal') {
      $this->processReversedTransactions($response, $payment_method, $transaction);
    }
  }

  /**
   * Process the transaction according to the respond.
   */
  public function processDirectTransactions($response, $payment_method, $transaction = '', $card_info) {
    // Get response information.
    $response_code = litle\sdk\XmlParser::getNode($response,'response');
    $order_id = litle\sdk\XmlParser::getNode($response,'orderId');
    $response_time = litle\sdk\XmlParser::getNode($response,'responseTime');
    $message = litle\sdk\XmlParser::getNode($response,'message');
    $auth_code = litle\sdk\XmlParser::getNode($response,'authCode');
    $creditResponse = litle\sdk\XmlParser::getNode($response,'creditResponse');
    $avs_result = litle\sdk\XmlParser::getNode($response,'avsResult');
    $card_validation_result = litle\sdk\XmlParser::getNode($response,'cardValidationResult');
    $litle_txn_id = litle\sdk\XmlParser::getNode($response,'litleTxnId');

    $authorizationResponse = litle\sdk\XmlParser::getNode($response,'authorizationResponse');
    $saleResponse = litle\sdk\XmlParser::getNode($response,'saleResponse');
    $registerTokenResponse = litle\sdk\XmlParser::getNode($response,'registerTokenResponse');
    $echeckSalesResponse = litle\sdk\XmlParser::getNode($response,'echeckSalesResponse');
    $echeckCreditResponse = litle\sdk\XmlParser::getNode($response,'echeckCreditResponse');

    // Get new data from account updater if changes were made.
    $accountUpdater = litle\sdk\XmlParser::getNode($response, 'accountUpdater');
    $newCardInfo = litle\sdk\XmlParser::getNode($response, 'newCardInfo');
    $type = litle\sdk\XmlParser::getNode($response, 'type');
    $number = litle\sdk\XmlParser::getNode($response, 'number');
    $expDate = litle\sdk\XmlParser::getNode($response, 'expDate');

    // Fraud detection parameters.
    $avs_result = litle\sdk\XmlParser::getNode($response, 'avsResult');
    $card_validation_result = litle\sdk\XmlParser::getNode($response, 'cardValidationResult');
    $device_review_status = litle\sdk\XmlParser::getNode($response, 'deviceReviewStatus');
    $device_reputation_score = litle\sdk\XmlParser::getNode($response, 'deviceReputationScore');

    // Insights.
    $insights_affluence = litle\sdk\XmlParser::getNode($response, 'affluence');
    $insights_issuer_country = litle\sdk\XmlParser::getNode($response, 'issuerCountry');

    if (!empty($echeckCreditResponse)) {
      $id = litle\sdk\XmlParser::getAttribute($response, 'echeckCreditResponse', 'id');
      $payload_attribute = 'echeckCreditResponse';
      $litleToken = litle\sdk\XmlParser::getNode($response,'litleToken');
    }

    if (!empty($creditResponse)) {
      $id = litle\sdk\XmlParser::getAttribute($response, 'creditResponse', 'id');
      $payload_attribute = 'creditResponse';
      $litleToken = litle\sdk\XmlParser::getNode($response,'litleToken');
    }
    if (!empty($authorizationResponse)) {
      $id = litle\sdk\XmlParser::getAttribute($response, 'authorizationResponse', 'id');
      $payload_attribute = 'authorizationResponse';
    }
    if (!empty($captureResponse)) {
      $id = litle\sdk\XmlParser::getAttribute($response, 'captureResponse', 'id');
      $payload_attribute = 'captureResponse';
    }
    if (!empty($registerTokenResponse)) {
      $id = litle\sdk\XmlParser::getAttribute($response, 'registerTokenResponse', 'id');
      $payload_attribute = 'registerTokenResponse';
      $litleToken = litle\sdk\XmlParser::getNode($response,'litleToken');
    }
    if (!empty($saleResponse)) {
      $id = litle\sdk\XmlParser::getAttribute($response, 'saleResponse', 'id');
      $payload_attribute = 'saleResponse';
    }
    if (!empty($echeckSalesResponse)) {
      $id = litle\sdk\XmlParser::getAttribute($response, 'echeckSalesResponse', 'id');
      $payload_attribute = 'echeckSalesResponse';
    }
    //Get the payload.
    $payload = litle\sdk\XmlParser::getDomDocumentAsString($response);
    //If the transaction was approved.
    if ($response_code == '000' || $response_code == '801' and (empty($device_reputation_score) or $device_reputation_score >= 0)) {
      if (!empty($order_id)) {
        $order = commerce_order_load($order_id);
      }
      elseif (!empty($transaction)) {
        $order = commerce_order_load($transaction->order_id);
      }

      // If the transaction is not registerTokenResponse.
      if (empty($registerTokenResponse)) {
        $transaction = commerce_payment_transaction_new($payment_method['method_id'], $order->order_id);
        $transaction->instance_id = $payment_method['instance_id'];
        $transaction->amount = $order->commerce_order_total['und'][0]['amount'];
        $transaction->currency_code = $order->commerce_order_total['und'][0]['currency_code'];
        $transaction->remote_id = $litle_txn_id;
        $transaction->remote_status = $message;
        $transaction->message_variables = array('@auth_code' => $auth_code);
        $transaction->data = array('auth_code' => $auth_code);
        $transaction->data['avs_result'] = !empty($avs_result) ? $avs_result : '';
        $transaction->data['card_validation_result'] = !empty($card_validation_result) ? $card_validation_result : '';

        //Get the payload.
        $payload = litle\sdk\XmlParser::getDomDocumentAsString($response);
        $payload_array = $this->convertPayloadToArray($payload, $payload_attribute);
        $transaction->payload = (is_array($payload_array) == TRUE) ? $payload_array : $payload;


        if (!empty($authorizationResponse)) {
          $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
          $transaction->message .= t('Authorized:') . t('@date', array('@date' => format_date(REQUEST_TIME, 'short')));
          $transaction->data = array('id' => $id);
        }
        elseif (!empty($saleResponse)) {
          $transaction->data = array('id' => $id);
          $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
          $transaction->message .= t('Sale:') . t('@date', array('@date' => format_date(REQUEST_TIME, 'short')));
        }
        elseif (!empty($echeckSalesResponse)) {
          $transaction->data = array('id' => $id);
          $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
          $transaction->message .= t('Sale:') . t('@date', array('@date' => format_date(REQUEST_TIME, 'short')));
        }
        elseif (!empty($creditResponse) || !empty($echeckCreditResponse)) {
          $transaction->status = COMMERCE_VANTIV_STATUS_CREDIT;
          $transaction->message .= t('Credit:') . t('@date', array('@date' => format_date(REQUEST_TIME, 'short')));
        }
        else {
          if (!empty($message) and !empty($order)) {
            if ($this->logs == 0) {
              watchdog('commerce_vantiv', "The order# @order was cancelled, due to @message", array('@order' => $order->order_id, '@message' => $message), WATCHDOG_ERROR);
            }
            $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
          }
          drupal_set_message(t('An error occur, your transaction was not processed successfully.'), 'error');
        }
        if (!empty($avs_result)) {
          $transaction->message = $transaction->message . '<br /><label style="color:#FF0000;">' . $this->avsResponseCodes($avs_result) . '</label>';
        }
        if (!empty($card_validation_result) and $card_validation_result != 'M') {
          $transaction->message = $transaction->message . '<br /><label style="color:#FF0000;">' . cardValidationResponseCodes($card_validation_result) . '</label>';
        }
        commerce_payment_transaction_save($transaction);
        if (!empty($insights_affluence) and ($insights_affluence == 'AFFLUENT' or $insights_affluence == 'MASS AFFLUENT')) {
          $user = user_load($order->uid);
          $user->data['affluence'] = ($insights_affluence == 'AFFLUENT') ? t('AFFLUENT') : t('MASS AFFLUENT');
          user_save($user);
        }
        // Saves the issuer.
        if (!empty($insights_issuer_country)) {
          $order->data['card_issuer_country'] = $insights_issuer_country;
          commerce_order_save($order);
        }
      }
      // Save card on file if required.
      if (!empty($litleToken) and !empty($registerTokenResponse)) {
        $customer_profile = commerce_customer_profile_load($order->commerce_customer_billing['und'][0]['profile_id']);
        $card_data = commerce_cardonfile_new();
        $card_data->uid = !empty($order->uid) ? $order->uid : $user->uid;
        $card_data->remote_id = $litleToken;
        $card_data->payment_method = $payment_method['method_id'];
        $card_data->instance_id = $payment_method['instance_id'];
        $card_data->card_type = !empty($card_info['credit_card']['type']) ? strtolower($card_info['credit_card']['type']) : '';
        $card_data->card_name = !empty($customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['name_line']) ? $customer_profile->commerce_customer_address[LANGUAGE_NONE][0]['name_line'] : $order['credit_card']['owner']['#value'];
        $card_data->card_number = !empty($card_info['credit_card']['number']) ? drupal_substr($card_info['credit_card']['number'], -4) : '';
        $card_data->card_exp_month = !empty($card_info['credit_card']['exp_month']) ? $card_info['credit_card']['exp_month'] : '';
        $card_data->card_exp_year = !empty($card_info['credit_card']['exp_year']) ? $card_info['credit_card']['exp_year'] : '';
        $card_data->status = 1;
        if ($payment_method['method_id'] == 'vantiv_echeck') {
          $card_data->card_type = $card_info['accType'];
          $card_data->card_number = drupal_substr($card_info['accNum'], -4);
        }
        if (!empty($accountUpdater) and !empty($newCardInfo) and $payment_method['method_id'] != 'vantiv_echeck') {
          $card_type = $api->cardTypes('', $type);
          $exp_month = substr($expDate, 1, 2);
          $exp_year = substr($expDate, -2, 2);
          $card_data->card_name = $params['credit_card']['owner'];
          $card_data->card_number = substr($number, -4, 4);
          $card_data->type = !empty($card_type) ? $card_type : '';
          $card_data->card_exp_month = $exp_month;
          $card_data->card_exp_year = $exp_year;
        }
        elseif ($payment_method['method_id'] == 'vantiv_echeck') {
          $card_data->card_exp_month = '12';
          $card_data->card_exp_year = '2100';
        }
        commerce_cardonfile_save($card_data);
        $card_info['cardonfile_store'] = '';
        if ($payment_method['method_id'] == 'vantiv_echeck') {
          $order = commerce_order_load($order_id);
          $card = commerce_vantiv_load_last_saved_cof($order->uid);
          $card_info['new_card_id'] = $card->card_id;
          $card_info['cardonfile'] = '';
          $card_info['credit_card']['cardonfile_store'] = '';
          return $this->directPayments($customer_profile, $order, $card_info, $payment_method, '', $litleToken, '', TRUE, 1);

        }
        elseif ($payment_method['method_id'] == 'vantiv_direct') {
          return $this->directPayments($customer_profile, $order, '', $payment_method, '', $litleToken);
        }
      }
    }
    else {
      if (!empty($order_id)) {
        $order = commerce_order_load($order_id);
      }
      elseif (!empty($transaction)) {
        $order = commerce_order_load($transaction->order_id);
      }
      // If an error occur.
      if ($response_code != '000' || $response_code != '801' /*and empty($avs_result) and empty($device_reputation_score)*/) {
        $transaction = commerce_payment_transaction_new($payment_method['method_id'], $order->order_id);
        $transaction->instance_id = $payment_method['instance_id'];
        $transaction->amount = $order->commerce_order_total['und'][0]['amount'];
        $transaction->currency_code = $order->commerce_order_total['und'][0]['currency_code'];
        $transaction->remote_id = $litle_txn_id;
        $transaction->remote_status = $message;
        $transaction->message_variables = array('@auth_code' => $auth_code);
        $transaction->data = array('auth_code' => $auth_code);
        $transaction->data['avs_result'] = !empty($avs_result) ? $avs_result : '';
        $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
        commerce_payment_transaction_save($transaction);
      }
      elseif (/*!empty($avs_result) and $avs_result != '00' or */!empty($device_reputation_score) and $device_reputation_score < 0) {
        $transaction = commerce_payment_transaction_new($payment_method['method_id'], $order->order_id);
        $transaction->instance_id = $payment_method['instance_id'];
        $transaction->amount = $order->commerce_order_total['und'][0]['amount'];
        $transaction->currency_code = $order->commerce_order_total['und'][0]['currency_code'];
        $transaction->remote_id = $litle_txn_id;
        $transaction->remote_status = $message;
        $transaction->data['avs_result'] = !empty($avs_result) ? $avs_result : '';
        $transaction->message_variables = array('@auth_code' => $auth_code);
        $transaction->data = array('auth_code' => $auth_code);
        $transaction->message = t('The transaction was not successful, a fraud was detected!');
        $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
        commerce_payment_transaction_save($transaction);
      }
      if ($this->logs == 0) {
        $responses = $this->transactionResponseCodes($response_code);
        watchdog('commerce_vantiv', "The transaction was not successful, the reason might be: message: @respons_message, type: @type, description: @descriptions ! Parameters returned: device reputation score: @device_reputation_score, avs result: @avs_result, device review status: @device_review_status, card validation result: @card_validation_result", array('@respons_message' => $responses['response_message'], '@type' => $responses['response_type'], '@descriptions' => $responses['descriptions'], '@card_validation_result' => $card_validation_result, '@device_reputation_score' => $device_reputation_score, '@avs_result' => $avs_result, '@device_review_status' => $device_review_status), WATCHDOG_ERROR);
      }
    }
  }

  /**
   * Process captured transactions.
   */
  public function processCapturedTransactions($response, $payment_method, $transaction, $partial_amount = '', $batch = '') {
    // Get response information.
    $response_code = litle\sdk\XmlParser::getNode($response,'response');
    $order_id = litle\sdk\XmlParser::getNode($response,'orderId');
    $response_time = litle\sdk\XmlParser::getNode($response,'responseTime');
    $message = litle\sdk\XmlParser::getNode($response,'message');
    $auth_code = litle\sdk\XmlParser::getNode($response,'authCode');
    $avs_result = litle\sdk\XmlParser::getNode($response,'avsResult');
    $card_validation_result = litle\sdk\XmlParser::getNode($response,'cardValidationResult');
    $litle_txn_id = litle\sdk\XmlParser::getNode($response,'litleTxnId');

    $captureResponse = litle\sdk\XmlParser::getNode($response,'captureResponse');

    if (!empty($captureResponse)) {
      $id = litle\sdk\XmlParser::getAttribute($response, 'captureResponse', 'id');
      $payload_attribute = 'captureResponse';
    }

    // If the transaction was approved.
    if ($response_code == '000') {
      if (!empty($order_id)) {
        $order = commerce_order_load($order_id);
      }
      elseif (!empty($transaction)) {
        $order = commerce_order_load($transaction->order_id);
      }
      // The pending transaction becomes captured.
      $transaction->status = COMMERCE_VANTIV_STATUS_CAPTURED;
      $previous_transaction_amount = $transaction->amount;
      // Set the new transaction balance of the previous transaction from the same order.
      if (!empty($partial_amount)) {
        $transaction->amount = $transaction->amount - $partial_amount;
      }
      else {
        $transaction->amount = 0;
      }

      commerce_payment_transaction_save($transaction);

      $transaction = commerce_payment_transaction_new('vantiv_direct', $order->order_id);
      if (!empty($partial_amount)) {
        $transaction->amount = $partial_amount;
      }
      else {
        $transaction->amount = $previous_transaction_amount;
      }
      $transaction->instance_id = $payment_method['instance_id'];
      $transaction->currency_code = $order->commerce_order_total['und'][0]['currency_code'];
      $transaction->remote_id = $litle_txn_id;
      $transaction->remote_status = $message;
      $transaction->message_variables = array('@auth_code' => $auth_code);

      // Get the payload.
      $payload = litle\sdk\XmlParser::getDomDocumentAsString($response);
      $payload_array = $this->convertPayloadToArray($payload, $payload_attribute);
      $transaction->payload = (is_array($payload_array) == TRUE) ? $payload_array : $payload;

      if (!empty($captureResponse)) {
        $transaction->status = COMMERCE_VANTIV_STATUS_CAPTURE;
        $transaction->message .= t('<br/> Captureed:') . t('@date', array('@date' => format_date(REQUEST_TIME, 'short')));
        $transaction->data = array('id' => $id);
        if (empty($batch)) {
          drupal_set_message(t('Transaction was captured successfully.'), 'status');
        }
      }
      else {
        if (!empty($message) and !empty($order)) {
          if ($this->logs == 0) {
            watchdog('commerce_vantiv', "This operation was not successful due to: @message", array('@message' => $message), WATCHDOG_ERROR);
          }
        }
        drupal_set_message(t('An error occur, your transaction was not processed successfully.'), 'error');
      }
      commerce_payment_transaction_save($transaction);
      if ($batch != TRUE or empty($batch)) {
        drupal_goto('admin/commerce/orders');
      }
      else {
        $result = array(
          'result' => TRUE,
          'transaction_id' => $transaction->transaction_id,
        );
        return $result;
      }
    }
    elseif ($this->logs == 0) {
        $responses = $this->transactionResponseCodes($response_code);
        watchdog('commerce_vantiv', "The transaction was not successful, the reason might be: message: @respons_message, type: @type, description: @descriptions !", array('@respons_message' => $responses['response_message'], '@type' => $responses['response_type'], '@descriptions' => $responses['descriptions']), WATCHDOG_ERROR);
    }
  }

  /**
   * Process refunded transactions.
   */
  public function processRefundedTransactions($response, $payment_method, $transaction, $partial_amount = '') {
    //Get response information.
    $response_code = litle\sdk\XmlParser::getNode($response,'response');
    $order_id = litle\sdk\XmlParser::getNode($response,'orderId');
    $response_time = litle\sdk\XmlParser::getNode($response,'responseTime');
    $message = litle\sdk\XmlParser::getNode($response,'message');
    $auth_code = litle\sdk\XmlParser::getNode($response,'authCode');
    $avs_result = litle\sdk\XmlParser::getNode($response,'avsResult');
    $card_validation_result = litle\sdk\XmlParser::getNode($response,'cardValidationResult');
    $litle_txn_id = litle\sdk\XmlParser::getNode($response,'litleTxnId');

    $creditResponse = litle\sdk\XmlParser::getNode($response,'creditResponse');

    if (!empty($creditResponse)) {
      $id = litle\sdk\XmlParser::getAttribute($response, 'creditResponse', 'id');
      $payload_attribute = 'creditResponse';
    }

    //If the transaction was approved.
    if ($response_code == '000') {
      if (!empty($order_id)) {
        $order = commerce_order_load($order_id);
      }
      elseif (!empty($transaction)) {
        $order = commerce_order_load($transaction->order_id);
      }
      $previous_transaction_amount = $transaction->amount;
      // The captured transaction becomes refunded.
      $transaction->status = COMMERCE_VANTIV_STATUS_REFUNDED;
      // Set the new transaction balance of the previous transaction from the same order.
      if (!empty($partial_amount)) {
        $transaction->amount = $transaction->amount - $partial_amount;
      }
      else {
        $transaction->amount = 0;
      }
      commerce_payment_transaction_save($transaction);
      $transaction = commerce_payment_transaction_new('vantiv_direct', $order->order_id);
      if (!empty($partial_amount)) {
        $transaction->amount = $partial_amount;
      }
      else {
        $transaction->amount = $previous_transaction_amount;
      }

      $transaction->instance_id = $payment_method['instance_id'];
      $transaction->currency_code = $order->commerce_order_total['und'][0]['currency_code'];
      $transaction->remote_id = $litle_txn_id;
      $transaction->remote_status = $message;
      $transaction->message_variables = array('@auth_code' => $auth_code);

      // Get the payload.
      $payload = litle\sdk\XmlParser::getDomDocumentAsString($response);
      $payload_array = $this->convertPayloadToArray($payload, $payload_attribute);
      $transaction->payload = (is_array($payload_array) == TRUE) ? $payload_array : $payload;
      if (!empty($creditResponse)) {
        $transaction->status = COMMERCE_VANTIV_STATUS_REFUND;
        $transaction->message .= t('<br/> Refunded:') . t('@date', array('@date' => format_date(REQUEST_TIME, 'short')));
        $transaction->data = array('id' => $id);
        drupal_set_message(t('Transaction was refunded successfully.'), 'status');
      }
      else {
        if (!empty($message) and !empty($order)) {
          if ($this->logs == 0) {
            watchdog('commerce_vantiv', "This operation was not successful due to: @message", array('@message' => $message), WATCHDOG_ERROR);
          }
        }
        drupal_set_message(t('An error occur, your transaction was not processed successfully.'), 'error');
      }
      commerce_payment_transaction_save($transaction);
      drupal_goto('admin/commerce/orders');
    }
    elseif ($this->logs == 0) {
      $responses = $this->transactionResponseCodes($response_code);
      watchdog('commerce_vantiv', "The transaction was not successful, the reason might be: message: @respons_message, type: @type, description: @descriptions !", array('@respons_message' => $responses['response_message'], '@type' => $responses['response_type'], '@descriptions' => $responses['descriptions']), WATCHDOG_ERROR);
    }
  }

  /**
   * Process void transactions.
   */
  public function processVoidTransaction($response, $payment_method, $transaction) {
      // Get response information.
    $response_code = litle\sdk\XmlParser::getNode($response,'response');
    $order_id = litle\sdk\XmlParser::getNode($response,'orderId');
    $response_time = litle\sdk\XmlParser::getNode($response,'responseTime');
    $message = litle\sdk\XmlParser::getNode($response,'message');
    $auth_code = litle\sdk\XmlParser::getNode($response,'authCode');
    $avs_result = litle\sdk\XmlParser::getNode($response,'avsResult');
    $card_validation_result = litle\sdk\XmlParser::getNode($response,'cardValidationResult');
    $litle_txn_id = litle\sdk\XmlParser::getNode($response,'litleTxnId');

    $voidResponse = litle\sdk\XmlParser::getNode($response,'voidResponse');
    $echeckVoidResponse = litle\sdk\XmlParser::getNode($response,'echeckVoidResponse');

    if (!empty($voidResponse)) {
      $id = litle\sdk\XmlParser::getAttribute($response, 'voidResponse', 'id');
      $payload_attribute = 'voidResponse';
    }

    if (!empty($echeckVoidResponse)) {
      $id = litle\sdk\XmlParser::getAttribute($response, 'echeckVoidResponse', 'id');
      $payload_attribute = 'echeckVoidResponse';
    }

    // If the transaction was approved.
    if ($response_code == '000' || $response_code = '010') {
      if (!empty($order_id)) {
        $order = commerce_order_load($order_id);
      }
      elseif (!empty($transaction)) {
        $order = commerce_order_load($transaction->order_id);
      }

      // The captured transaction becomes voided.
      $transaction->status = COMMERCE_VANTIV_STATUS_VOIDED;
      $transaction->message .= t('<br/> Voided:') . t('@date', array('@date' => format_date(REQUEST_TIME, 'short')));
      // Set the new transaction balance of the previous transaction from the same order.
      if (!empty($partial_amount)) {
        $transaction->amount = $transaction->amount - $partial_amount;
      }
      else {
        $transaction->amount = 0;
      }
      commerce_payment_transaction_save($transaction);
      $transaction = commerce_payment_transaction_new('vantiv_direct', $order->order_id);
      if (!empty($partial_amount)) {
        $transaction->amount = $partial_amount;
      }
      else {
        $transaction->amount = $order->commerce_order_total['und'][0]['amount'];
      }

      $transaction->instance_id = $payment_method['instance_id'];
      $transaction->currency_code = $order->commerce_order_total['und'][0]['currency_code'];
      $transaction->remote_id = $litle_txn_id;
      $transaction->remote_status = $message;
      $transaction->message_variables = array('@auth_code' => $auth_code);

      // Get the payload.
      $payload = litle\sdk\XmlParser::getDomDocumentAsString($response);
      $payload_array = $this->convertPayloadToArray($payload, $payload_attribute);
      $transaction->payload = (is_array($payload_array) == TRUE) ? $payload_array : $payload;
      if (!empty($voidResponse) || !empty($echeckVoidResponse)) {
        $transaction->status = COMMERCE_VANTIV_STATUS_VOID;
        $transaction->message .= t('<br/> Void:') . t('@date', array('@date' => format_date(REQUEST_TIME, 'short')));
        $transaction->data = array('id' => $id);
        drupal_set_message(t('Transaction was voided successfully.'), 'status');
      }
      else {
        if (!empty($message) and !empty($order)) {
          if ($this->logs == 0) {
            watchdog('commerce_vantiv', "This operation was not successful due to: @message", array('@message' => $message), WATCHDOG_ERROR);
          }
        }
        drupal_set_message(t('An error occur, your transaction was not processed successfully.'), 'error');
      }
      commerce_payment_transaction_save($transaction);
      drupal_goto('admin/commerce/orders');
    }
    elseif ($this->logs == 0) {
      $responses = $this->transactionResponseCodes($response_code);
      watchdog('commerce_vantiv', "The transaction was not successful, the reason might be: message: @respons_message, type: @type, description: @descriptions !", array('@respons_message' => $responses['response_message'], '@type' => $responses['response_type'], '@descriptions' => $responses['descriptions']), WATCHDOG_ERROR);
    }
  }

  /**
   * Processes reversed authorized transaction.
   */
  public function processReversedTransactions($response, $payment_method, $transaction, $partial_amount = '') {
    // Get response information.
    $response_code = litle\sdk\XmlParser::getNode($response,'response');
    $order_id = litle\sdk\XmlParser::getNode($response,'orderId');
    $response_time = litle\sdk\XmlParser::getNode($response,'responseTime');
    $message = litle\sdk\XmlParser::getNode($response,'message');
    $auth_code = litle\sdk\XmlParser::getNode($response,'authCode');
    $avs_result = litle\sdk\XmlParser::getNode($response,'avsResult');
    $card_validation_result = litle\sdk\XmlParser::getNode($response,'cardValidationResult');
    $litle_txn_id = litle\sdk\XmlParser::getNode($response,'litleTxnId');

    $authReversalResponse = litle\sdk\XmlParser::getNode($response,'authReversalResponse');

    if (!empty($authReversalResponse)) {
      $id = litle\sdk\XmlParser::getAttribute($response, 'authReversalResponse', 'id');
      $payload_attribute = 'authReversalResponse';
    }

    // If the transaction was approved.
    if ($response_code == '000') {
      if (!empty($order_id) and $order_id == $transaction->order_id) {
        $order = commerce_order_load($order_id);
      }
      elseif (!empty($transaction)) {
        $order = commerce_order_load($transaction->order_id);
      }

      $previous_transaction = $transaction;

      // Set the new transaction balance of the previous transaction from the same order.
      if (!empty($partial_amount)) {
        $transaction->amount = $transaction->amount - $partial_amount;
      }
      else {
        $transaction->amount = 0;
        // The pending transaction becomes captured.
        $transaction->status = COMMERCE_VANTIV_STATUS_VOIDED;
      }
      // Save the old transaction.
      commerce_payment_transaction_save($transaction);

      // New transaction to save.
      $transaction = commerce_payment_transaction_new('vantiv_direct', $order->order_id);
      if (!empty($partial_amount)) {
        $transaction->amount = $partial_amount;
      }
      else {
        $transaction->amount = $previous_transaction->amount;
      }
      $transaction->amount = 0;
      $transaction->instance_id = $payment_method['instance_id'];
      $transaction->currency_code = $order->commerce_order_total['und'][0]['currency_code'];
      $transaction->remote_id = $litle_txn_id;
      $transaction->remote_status = $message;
      $transaction->message_variables = array('@auth_code' => $auth_code);

      // Get the payload.
      $payload = litle\sdk\XmlParser::getDomDocumentAsString($response);
      $payload_array = $this->convertPayloadToArray($payload, $payload_attribute);
      $transaction->payload = (is_array($payload_array) == TRUE) ? $payload_array : $payload;

      if (!empty($authReversalResponse)) {
        $transaction->status = COMMERCE_VANTIV_STATUS_VOID;
        $transaction->message .= t('<br/> Reversed:') . t('@date', array('@date' => format_date(REQUEST_TIME, 'short')));
        $transaction->data = array('id' => $id);
        drupal_set_message(t('Authorization reverse operation was successful.'), 'status');
      }
      else {
        if (!empty($message) and !empty($order)) {
          if ($this->logs == 0) {
            watchdog('commerce_vantiv', "This operation was not successful due to: @message", array('@message' => $message), WATCHDOG_ERROR);
          }
        }
        drupal_set_message(t('An error occur, your transaction was not processed successfully.'), 'error');
      }
      commerce_payment_transaction_save($transaction);
      drupal_goto('admin/commerce/orders');
    }
    elseif ($this->logs == 0) {
      $responses = $this->transactionResponseCodes($response_code);
      watchdog('commerce_vantiv', "The transaction was not successful, the reason might be: message: @respons_message, type: @type, description: @descriptions !", array('@respons_message' => $responses['response_message'], '@type' => $responses['response_type'], '@descriptions' => $responses['descriptions']), WATCHDOG_ERROR);
    }
  }

  /**
   * Convert the xml payload to array.
   */
  public function convertPayloadToArray($payload, $payload_attribute) {
    $xml = simplexml_load_string($payload);
    $json = json_encode($xml);
    $json_dec = json_decode($json, TRUE);
    if (!empty($json_dec[$payload_attribute])) {
      foreach ($json_dec[$payload_attribute] as $key => $value) {
        if (strstr($key, '@') != FALSE) {
          $new_key = str_replace('@', '', $key);
          $new_array[$new_key] = $value;
        }
        else {
          $new_array[$key] = $value;
        }
      }
      foreach ($new_array['attributes'] as $k => $v) {
        if (!empty($k) and !empty($v)) {
          $atributes[$k] = $v;
        }
      }
      $new_array['attributes'] = $atributes;
      return $new_array;
    }
    else {
      return FALSE;
    }
  }
  /**
   * Get account links.
   */
  public function accountLinks($account) {
    switch ($account) {
      case 'sandbox':
        $url = 'https://www.testlitle.com/sandbox/communicator/online';
      break;
      case 'postlive':
        $url = 'https://postlive.litle.com/vap/communicator/online';
      break;
      case 'transact_postlive':
        $url = 'https://transact-postlive.litle.com/vap/communicator/online';
      break;
      case 'production':
        $url = 'https://payments.litle.com/vap/communicator/online';
      break;
      case 'production_transact':
        $url = 'https://transact.litle.com/vap/communicator/online';
      break;
      case 'prelive':
        $url = 'https://prelive.litle.com/vap/communicator/online';
      break;
      case 'transact_prelive':
        $url = 'https://transact-prelive.litle.com/vap/communicator/online';
      break;
    }
    return $url;
  }

  /**
   * Returns short payment method type.
   */
  public function cardTypes($card_type = '', $reverse = '') {
    if (!empty($card_type)) {
      switch ($card_type) {
        case 'visa':
          $type = 'VI';
        break;
        case 'mastercard':
          $type = 'MC';
        break;
        case 'amex':
          $type = 'AX';
        break;
        case 'discover':
          $type = 'DI';
        break;
        case 'dc':
          $type = 'DC';
        break;
        case 'maestro':
          $type = 'MA';
        break;
        case 'paypal':
          $type = 'PP';
        break;
        case 'jcb':
          $type = 'JC';
        break;
        case 'bml':
          $type = 'BL';
        break;
        case 'echeck':
          $type = 'EC';
        break;
      }
    }
    elseif (!empty($reverse) and empty($card_type)) {
      switch ($reverse) {
        case 'VI':
          $type = 'visa';
        break;
        case 'MC':
          $type = 'mastercard';
        break;
        case 'AX':
          $type = 'amex';
        break;
        case 'DI':
          $type = 'discover';
        break;
        case 'DC':
          $type = 'dc';
        break;
        case 'MA':
          $type = 'maestro';
        break;
        case 'PP':
          $type = 'paypal';
        break;
        case 'JC':
          $type = 'jcb';
        break;
        case 'BL':
          $type = 'bml';
        break;
        case 'EC':
          $type = 'echeck';
        break;
      }
    }
    return  $type;
  }

  public function avsResponseCodes($code) {
    $result = array();
    switch ($code) {
      case '00' :
          $result = t('5-Digit zip and address match');
        break;
      case '01' :
          $result = t('9-Digit zip and address match');
        break;
      case '02' :
          $result = t('Postal code and address match');
        break;
      case '10' :
          $result = t('5-Digit zip matches, address does not match');
        break;
      case '11' :
          $result = t('9-Digit zip matches, address does not match');
        break;
      case '12' :
          $result = t('Zip does not match, address matches');
        break;
      case '13' :
          $result = t('Postal code does not match, address matches');
        break;
      case '14' :
          $result = t('Postal code matches, address not verified');
        break;
      case '20' :
          $result = t('Neither zip nor address match');
        break;
      case '30' :
          $result = t('AVS service not supported by issuer');
        break;
      case '31' :
          $result = t('AVS system not available');
        break;
      case '32' :
          $result = t('Address unavailable');
        break;
      case '33' :
          $result = t('General error');
        break;
      case '34' :
          $result = t('AVS not performed');
        break;
      case '40' :
          $result = t('Address failed Litle & Co. edit checks');
        break;
    }
    return $result;
  }

  public function cardValidationResponseCodes($code) {
    switch ($code) {
      case 'M' :
          $result = t('Match');
        break;
      case 'N' :
          $result = t('No Match');
        break;
      case 'P' :
          $result = t('Not Processed');
        break;
      case 'S' :
          $result = t('CVV2/CVC2/CID should be on the card, but the merchant
                     has indicated CVV2/CVC2/CID is not present');
        break;
      case 'U' :
          $result = t('Issuer is not certified for CVV2/CVC2/CID processing');
        break;
      case '' :
          $result = t('Check was not done for an unspecified reason');
        break;
    }
    return $result;
  }

  public function transactionResponseCodes($code) {
    switch ($code) {
      case '000' :
        $result = array(
          'response_message' => t('Approved'),
          'response_type' => t('Approved'),
          'descriptions' => t('No action required.'),
        );
        break;
      case '010' :
        $result = array(
          'response_message' => t('Partially Approved'),
          'response_type' => t('Approved'),
          'descriptions' => t('The authorized amount is less than the requested amount.'),
        );
        break;
      case '100' :
        $result = array(
          'response_message' => t('Processing Network Unavailable'),
          'response_type' => t('Soft Decline'),
          'descriptions' => t('There is a problem with the card network. Contact the network for more information.'),
        );
        break;
      case '101' :
        $result = array(
          'response_message' => t('Issuer Unavailable'),
          'response_type' => t('Soft Decline'),
          'descriptions' => t('There is a problem with the issuer network. Please contact the issuing bank.'),
        );
        break;
      case '102' :
        $result = array(
          'response_message' => t('Re-submit Transaction'),
           'response_type' => t('Soft Decline'),
          'descriptions' => t('There is a temporary problem with your submission. Please re-submit the transaction.'),
        );
        break;
      case '110' :
        $result = array(
          'response_message' => t('Insufficient Funds'),
          'response_type' => t('Soft Decline'),
          'descriptions' => t('The card does not have enough funds to cover the transaction.'),
        );
        break;
      case '111' :
        $result = array(
          'response_message' => t('Authorization amount has already been depleted'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The total amount of the original Authorization has been used.'),
        );
        break;
      case '120' :
        $result = array(
          'response_message' => t('Call Issuer'),
          'response_type' => t('Referral or Soft Decline'),
          'descriptions' => t('There is an unspecified problem, contact the issuing bank.'),
        );
        break;
      case '121' :
        $result = array(
          'response_message' => t('Call AMEX'),
          'response_type' => t('Referral'),
          'descriptions' => t('There is an unspecified problem; contact AMEX.'),
        );
        break;
      case '122' :
        $result = array(
          'response_message' => t('Call Diners Club'),
          'response_type' => t('Referral'),
          'descriptions' => t('There is an unspecified problem; contact Diners Club.'),
        );
        break;
      case '123' :
        $result = array(
          'response_message' => t('Call Discover'),
          'response_type' => t('Referral'),
          'descriptions' => t('There is an unspecified problem contact Discover.'),
        );
        break;
      case '124' :
        $result = array(
          'response_message' => t('Call JBS'),
          'response_type' => t('Referral'),
          'descriptions' => t('There is an unspecified problem; contact JBS.'),
        );
        break;
      case '125' :
        $result = array(
          'response_message' => t('Call Visa/MasterCard'),
          'response_type' => t('Referral'),
          'descriptions' => t('There is an unspecified problem; contact Visa or MasterCard.'),
        );
        break;
      case '126' :
        $result = array(
          'response_message' => t('Call Issuer - Update Cardholder Data'),
          'response_type' => t('Referral'),
          'descriptions' => t('Some data is out of date; contact the issuer to update this information.'),
        );
        break;
      case '127' :
        $result = array(
          'response_message' => t('Exceeds Approval Amount Limit'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('This transaction exceeds the daily approval limit for the card.'),
        );
        break;
      case '130' :
        $result = array(
          'response_message' => t('Call Indicated Number'),
          'response_type' => t('Referral'),
          'descriptions' => t('There is an unspecified problem; contact the phone number provided.'),
        );
        break;
      case '140' :
        $result = array(
          'response_message' => t('Update Cardholder Data'),
          'response_type' => t('Referral'),
          'descriptions' => t('Cardholder data is incorrect; contact the issuing bank.'),
        );
        break;
      case '191' :
        $result = array(
          'response_message' => t('The merchant is not registered in the update program.'),
          'response_type' => t('N/A'),
          'descriptions' => t('This is an Account Updater response indicating a set-up problem that must be
            resolved prior to submitting another request file. Escalate this to your Litle Customer Experience Manager.'),
        );
        break;
      case '192' :
        $result = array(
          'response_message' => t('Merchant not certified/enabled for IIAS'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('Your organization is not certified or enabled for IIAS/FSA transactions.'),
        );
        break;
      case '206' :
        $result = array(
          'response_message' => t('Issuer Generated Error'),
          'response_type' => t('Soft Decline'),
          'descriptions' => t('An unspecified error was returned by the issuer.
            Please retry the transaction and if the problem persist, contact the issuing bank.'),
        );
        break;
      case '207' :
        $result = array(
          'response_message' => t('Pickup card - Other than Lost/Stolen'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The issuer indicated that the gift card should be removed from use.'),
        );
        break;
      case '209' :
        $result = array(
          'response_message' => t('Invalid Amount'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The specified amount is invalid for this transaction.'),
        );
        break;
      case '211' :
        $result = array(
          'response_message' => t('Reversal Unsuccessful'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The reversal transaction was unsuccessful.'),
        );
        break;
      case '212' :
        $result = array(
          'response_message' => t('Missing Data'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('Contact Litle.'),
        );
        break;
      case '213' :
        $result = array(
          'response_message' => t('Pickup Card - Lost Card'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The submitted card was reported as lost and should be removed from use.'),
        );
        break;
      case '214' :
        $result = array(
          'response_message' => t('Pickup Card - Stolen Card'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The submitted card was reported as stolen and should be removed from use.'),
        );
        break;
      case '215' :
        $result = array(
          'response_message' => t('Restricted Card'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The specified Gift Card is not available for use.'),
        );
        break;
      case '216' :
        $result = array(
          'response_message' => t('Invalid Deactivate'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The Deactivate transaction is invalid for the specified card.'),
        );
        break;
      case '217' :
        $result = array(
          'response_message' => t('Card Already Active'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The submitted card is already active.'),
        );
        break;
      case '218' :
        $result = array(
          'response_message' => t('Card Not Active'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The submitted card has not been activated.'),
        );
        break;
      case '219' :
        $result = array(
          'response_message' => t('Card Already Deactivate'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The submitted card has already been deactivated.'),
        );
        break;
      case '221' :
        $result = array(
          'response_message' => t('Over Max Balance'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The activate or load amount exceeds the maximum allowed for
            the specified gift Card.'),
        );
        break;
      case '222' :
        $result = array(
          'response_message' => t('Invalid Activate'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The activate transaction is not valid or can no longer be reversed.'),
        );
        break;
      case '223' :
        $result = array(
          'response_message' => t('No transaction Found for Reversal'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The transaction referenced in the reversal transaction does not exist.'),
        );
        break;
      case '226' :
        $result = array(
          'response_message' => t('Incorrect CVV'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The transaction was declined because it was submitted with the incorrect security code.'),
        );
        break;
      case '229' :
        $result = array(
          'response_message' => t('Illegal Transaction'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The transaction would violate the law.'),
        );
        break;
      case '251' :
        $result = array(
          'response_message' => t('Duplicate Transaction'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The transaction is a duplicate of a previously submitted transaction.'),
        );
        break;
      case '252' :
        $result = array(
          'response_message' => t('System Error'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('Contact Litle.'),
        );
        break;
      case '253' :
        $result = array(
          'response_message' => t('Deconverted BIN'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The BIN is no longer valid.'),
        );
        break;
      case '254' :
        $result = array(
          'response_message' => t('Merchant Depleted'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('No balance remains on gift Card.'),
        );
        break;
      case '255' :
        $result = array(
          'response_message' => t('Gift Card Escheated'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The Gift Card has been seized by the government while resolving an estate.'),
        );
        break;
      case '257' :
        $result = array(
          'response_message' => t('System Error (message format error)'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('Issuer reported message format is incorrect. Contact Litle.'),
        );
        break;
      case '258' :
        $result = array(
          'response_message' => t('System Error (cannot process)'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('Issuer reported transaction could not be processed. Contact Litle.'),
        );
        break;
      case '301' :
        $result = array(
          'response_message' => t('Invalid Account Number'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The account number is not valid; contact the cardholder to confirm
            information or inquire about another form of payment.'),
        );
        break;
      case '302' :
        $result = array(
          'response_message' => t('Account Number Does Not Match Payment Type'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The payment type was selected as one card type (e.g. Visa), but
            the card number indicates a different card type (e.g.MasterCard).'),
        );
        break;
      case '303' :
        $result = array(
          'response_message' => t('Pick Up Card'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('This is a card present response, but in a card not present
            environment. Do not process the transaction and contact the issuing bank.'),
        );
        break;
      case '304' :
        $result = array(
          'response_message' => t('Lost/Stolen Card'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The card has been designated as lost or stolen; contact the issuing bank.'),
        );
        break;
      case '305' :
        $result = array(
          'response_message' => t('Expired Card'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The card is expired.'),
        );
        break;
      case '306' :
        $result = array(
          'response_message' => t('Authorization has expired; no need to reverse'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The original Authorization is no longer valid, because it has
            expired. You can not perform an Authorization Reversal for an expired Authorization.'),
        );
        break;
      case '307' :
        $result = array(
          'response_message' => t('Restricted Card'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The card has a restriction preventing approval for this
            transaction. Please contact the issuing bank for a specific reason.
            You may also receive this code if the transaction was declined due to
            Prior Fraud Advice Filtering and you are using a schema version V8.10 or older.'),
        );
        break;
      case '308' :
        $result = array(
          'response_message' => t('Restricted Card - Chargeback'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('This transaction is being declined due the operation of the Litle
            Prior Chargeback Card Filtering Service or the card has a restriction preventing approval if
            there are any chargebacks against it.'),
        );
        break;
      case '309' :
        $result = array(
          'response_message' => t('Restricted Card - Prepaid Card Filtering Service'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('This transaction is being declined due the operation of the Litle
            Prepaid Card Filtering service.'),
        );
        break;
      case '310' :
        $result = array(
          'response_message' => t('Invalid track data'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The track data is not valid.'),
        );
        break;
      case '311' :
        $result = array(
          'response_message' => t('Deposit is already referenced by a chargeback'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The deposit is already referenced by a chargeback; therefore, a
            refund cannot be processed against the original transaction.'),
        );
        break;
      case '312' :
        $result = array(
          'response_message' => t('Restricted Card - International Card Filtering Service'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('This transaction is being declined due the operation of the Litle
            International Card Filtering Service.'),
        );
        break;
      case '315' :
        $result = array(
          'response_message' => t('Restricted Card - Auth Fraud Velocity Filtering Service'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('This transaction is being declined due the operation of the Litle
            Auth Fraud Velocity Filtering Service.'),
        );
        break;
      case '316' :
        $result = array(
          'response_message' => t('Automatic Refund Already Issued'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('This refund transaction is a duplicate for one already
            processed automatically by the Litle Fraud Chargeback Prevention Service (FCPS).'),
        );
        break;
      case '318' :
        $result = array(
          'response_message' => t('Restricted Card - Auth Fraud Advice Filtering Service'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('This transaction is being declined due the operation of the Litle
            Auth Fraud Advice Filtering Service.'),
        );
        break;
      case '319' :
        $result = array(
          'response_message' => t('Restricted Card - Fraud AVS Filtering Service'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('This transaction is being declined due the operation of the Litle
            Auth Fraud AVS Filtering Service.'),
        );
        break;
      case '320' :
        $result = array(
          'response_message' => t('Invalid Expiration Date'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The expiration date is invalid.'),
        );
        break;
      case '321' :
        $result = array(
          'response_message' => t('Invalid Merchant'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The card is not allowed to make purchases from this merchant
            (e.g. a Travel only card trying to purchase electronics).'),
        );
        break;
      case '322' :
        $result = array(
          'response_message' => t('Invalid Transaction'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The transaction is not permitted; contact the issuing bank.'),
        );
        break;
      case '323' :
        $result = array(
          'response_message' => t('No such issuer'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The card number references an issuer that does not exist.
            Do not process the transaction.'),
        );
        break;
      case '324' :
        $result = array(
          'response_message' => t('Invalid Pin'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The PIN provided is invalid.'),
        );
        break;
      case '325' :
        $result = array(
          'response_message' => t('Transaction not allowed at terminal'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The transaction is not permitted; contact the issuing bank.'),
        );
        break;
      case '326' :
        $result = array(
          'response_message' => t('Exceeds number of PIN entries'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('(Referring to a debit card) The incorrect PIN has been entered
            excessively and the card is locked.'),
        );
        break;
      case '327' :
        $result = array(
          'response_message' => t('Cardholder transaction not permitted'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('Merchant does not allow that card type or specific transaction.'),
        );
        break;
      case '328' :
        $result = array(
          'response_message' => t('Cardholder requested that recurring or installment payment be stopped'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('Recurring/Installment Payments no longer accepted by the card issuing bank.'),
        );
        break;
      case '330' :
        $result = array(
          'response_message' => t('Invalid Payment Type'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('This payment type is not accepted by the issuer.'),
        );
        break;
      case '331' :
        $result = array(
          'response_message' => t('Invalid POS Capability for Cardholder Authorized Terminal Transaction'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('For a Cardholder Authorized Terminal Transaction the POS capability must be set to magstripe.'),
        );
        break;
      case '332' :
        $result = array(
          'response_message' => t('Invalid POS Cardholder ID for Cardholder Authorized Terminal Transaction'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('For a Cardholder Authorized Terminal Transaction the POS Cardholder ID must be set to nopin.'),
        );
        break;
      case '335' :
        $result = array(
          'response_message' => t('This method of payment does not support authorization reversals'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('You can not perform an Authorization Reversal transaction for this payment type.'),
        );
        break;
      case '336' :
        $result = array(
          'response_message' => t('Reversal amount does not match Authorization amount.'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('For a merchant initiated reversal against an American Express authorization, the reversal
            amount must match the authorization amount exactly.'),
        );
        break;
      case '340' :
        $result = array(
          'response_message' => t('Invalid Amount'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The transaction amount is invalid (too high or too low). For
            example, less than 0 for an authorization, or less than .01 for other payment types.'),
        );
        break;
      case '341' :
        $result = array(
          'response_message' => t('Invalid Healthcare Amounts'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The amount submitted with this FSA/Healthcare transaction is
            invalid. The FSA amount must be greater than 0, and cannot be greater than the transaction amount.'),
        );
        break;
       case '346' :
        $result = array(
          'response_message' => t('Invalid billing descriptor prefix'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The billing descriptor prefix submitted is not valid.'),
        );
        break;
      case '347' :
        $result = array(
          'response_message' => t('Invalid billing descriptor'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The billing descriptor is not valid because you are not authorized to
            send transactions with custom billing fields.'),
        );
        break;
      case '348' :
        $result = array(
          'response_message' => t('Invalid Report Group'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The Report Group specified in the transaction is invalid, because it
            is either not in the defined list of acceptable Report Groups or there is a mis-match between the
            Report Group and the defined Billing Descriptor.'),
        );
        break;
      case '349' :
        $result = array(
          'response_message' => t('Do Not Honor'),
          'response_type' => t('Soft Decline'),
          'descriptions' => t('The issuing bank has put a temporary hold on the card.'),
        );
        break;
      case '350' :
        $result = array(
          'response_message' => t('Generic Decline'),
          'response_type' => t('Soft or Hard Decline'),
          'descriptions' => t('There is an unspecified problem; contact the issuing bank for more details.
            Note: This code can be a hard or soft decline, depending on the
            method of payment, and other variables.'),
        );
        break;
      case '351' :
        $result = array(
          'response_message' => t('Decline - Request Positive ID'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('Card Present transaction that requires a picture ID match.'),
        );
        break;
      case '352' :
        $result = array(
          'response_message' => t('Decline CVV2/CID Fail'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The CVV2/CID is invalid.'),
        );
        break;
      case '354' :
        $result = array(
          'response_message' => t('3-D Secure transaction not supported by merchant'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('You are not certified to submit 3-D Secure transactions.'),
        );
        break;
      case '356' :
        $result = array(
          'response_message' => t('Invalid purchase level III, the transaction contained bad or missing data'),
          'response_type' => t('Soft Decline'),
          'descriptions' => t('Submitted Level III data is bad or missing.'),
        );
        break;
      case '357' :
        $result = array(
          'response_message' => t('Missing healthcareIIAS tag for an FSA transaction'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The FSA Transactions submitted does not contain the <healtcareIIAS> data element.'),
        );
        break;
      case '358' :
        $result = array(
          'response_message' => t('Restricted by Litle due to security code mismatch.'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The transaction was declined due to the security code (CVV2, CID, etc) not matching.'),
        );
        break;
      case '360' :
        $result = array(
          'response_message' => t('No transaction found with specified litleTxnId'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('There were no transactions found with the specified litleTxnId.'),
        );
        break;
      case '361' :
        $result = array(
          'response_message' => t('Authorization no longer available'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The authorization for this transaction is no longer available.
            Either the authorization has already been consumed by another capture, or the authorization has expired.'),
        );
        break;
      case '362' :
        $result = array(
          'response_message' => t('Auto-void on refund'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('This transaction cannot be voided; it has already been delivered.'),
        );
        break;
      case '363' :
        $result = array(
          'response_message' => t('Authorization no longer available'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('This transaction (both capture and refund) has been voided.'),
        );
        break;
      case '364' :
        $result = array(
          'response_message' => t('Invalid Account Number - original or NOC updated eCheck account required'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The submitted account number is invalid. Confirm the original
            account number or check NOC for new account number.'),
        );
        break;
      case '365' :
        $result = array(
          'response_message' => t('Total credit amount exceeds capture amount'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The amount of the credit is greater than the capture, or the amount of this credit plus other
            credits already referencing this capture are greater than the capture amount.'),
        );
        break;
      case '366' :
        $result = array(
          'response_message' => t('Exceed the threshold for sending redeposits'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('NACHA rules allow two redeposit attempts within 180 days of the
            settlement date of the initial deposit attempt. This threshold has been exceeded.'),
        );
        break;
      case '367' :
        $result = array(
          'response_message' => t('Deposit has not been returned for insufficient/non-sufficient funds'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('NACHA rules only allow redeposit attempts against deposits
            returned for Insufficient or Uncollected Funds.'),
        );
        break;
      case '368' :
        $result = array(
          'response_message' => t('Invalid check number'),
          'response_type' => t('Soft Decline'),
          'descriptions' => t('The check number is invalid.'),
        );
        break;
      case '369' :
        $result = array(
          'response_message' => t('Redeposit against invalid transaction type'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The redeposit attempted against an invalid transaction type.'),
        );
        break;
      case '370' :
        $result = array(
          'response_message' => t('Internal System Error - Call Litle'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('There is a problem with the Litle System. Contact support@litle.com.'),
        );
        break;
      case '372' :
        $result = array(
          'response_message' => t('Soft Decline - Auto Recycling In Progress'),
          'response_type' => t('Soft Decline'),
          'descriptions' => t('The transaction was intercepted because it is being auto recycled by the Recycling Engine.'),
        );
        break;
      case '373' :
        $result = array(
          'response_message' => t('Hard Decline - Auto Recycling Complete'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The transaction was intercepted because auto recycling has completed with a final decline.'),
        );
        break;
      case '375' :
        $result = array(
          'response_message' => t('Merchant is not enabled for surcharging'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The submitted transaction contained a surcharge and the merchant is not enabled for
            surcharging.'),
        );
        break;
      case '376' :
        $result = array(
          'response_message' => t('This method of payment does not support surcharging'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The use of a surcharge is only allowed for Visa and MasterCard methods of payment.'),
        );
        break;
      case '377' :
        $result = array(
          'response_message' => t('Surcharge is not valid for debit or prepaid cards'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('You cannot apply a surcharge to a transaction using a debit or prepaid card.'),
        );
        break;
      case '378' :
        $result = array(
          'response_message' => t('Surcharge cannot exceed 4% of the sale amount'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The surcharge in the submitted transaction exceeded 4% maximum allowed for a
            surcharge.'),
        );
        break;
      case '401' :
        $result = array(
          'response_message' => t('Invalid E-mail'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The e-mail address provided is not valid. Verify that it was entered correctly.'),
        );
        break;
      case '469' :
        $result = array(
          'response_message' => t('Invalid Recurring Request - See Recurring Response for Details'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The Recurring Request was invalid, which invalidated the transaction. The Response Code
            and Message in the Recurring Response contains additional information.'),
        );
        break;
      case '470' :
        $result = array(
          'response_message' => t('Approved - Recurring Subscription Created'),
          'response_type' => t('Approved'),
          'descriptions' => t('The recurring request was processed successfully'),
        );
        break;
      case '471' :
        $result = array(
          'response_message' => t('Parent Transaction Declined - Recurring Subscription Not Created'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The original payment transaction was declined, so the recurring payments have not been scheduled.'),
        );
        break;
      case '472' :
        $result = array(
          'response_message' => t('Invalid Plan Code'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The plan specified in the recurring request was invalid.'),
        );
        break;
      case '473' :
        $result = array(
          'response_message' => t('Scheduled Recurring Payment Processed'),
          'response_type' => t('Approved'),
          'descriptions' => t('The scheduled recurring payment has been processed successfully.'),
        );
        break;
      case '475' :
        $result = array(
          'response_message' => t('Invalid Subscription Id'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The referenced subscription Id does not exist.'),
        );
        break;
      case '476' :
        $result = array(
          'response_message' => t('Add On Code Already Exists'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The specified Add On code already exists.'),
        );
        break;
      case '477' :
        $result = array(
          'response_message' => t('Duplicate Add On Codes in Requests'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('Multiple createAddOn requests submitted with the same Add On Code.'),
        );
        break;
      case '478' :
        $result = array(
          'response_message' => t('No Matching Add On Code for the Subscription'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The Add On code specified does not exist.'),
        );
        break;
      case '480' :
        $result = array(
          'response_message' => t('No Matching Discount Code for the Subscription'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The Discount Code supplied in the updateDiscount or
            deleteDiscount transaction does not exist.'),
        );
        break;
      case '481' :
        $result = array(
          'response_message' => t('Duplicate Discount Codes in Request'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('Multiple createDiscount requests submitted with the same Discount Code.'),
        );
        break;
      case '482' :
        $result = array(
          'response_message' => t('Invalid Start Date'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The supplied Start Date is invalid.'),
        );
        break;
      case '483' :
        $result = array(
          'response_message' => t('Merchant Not Registered for Recurring Engine'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('You are not registered for the use of the Recurring Engine.'),
        );
        break;
      case '500' :
        $result = array(
          'response_message' => t('The account number was changed'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('An Account Updater response indicating the Account Number changed from the original number.'),
        );
        break;
      case '501' :
        $result = array(
          'response_message' => t('The account was closed'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('An Account Updater response indicating the account was
            closed. Contact the cardholder directly for updated information.'),
        );
        break;
      case '502' :
        $result = array(
          'response_message' => t('The expiration date was changed'),
          'response_type' => t('N/A'),
          'descriptions' => t('An Account Updater response indicating the Expiration date for the card has changed.'),
        );
        break;
      case '503' :
        $result = array(
          'response_message' => t('The issuing bank does not participate in the update program'),
          'response_type' => t('N/A'),
          'descriptions' => t('An Account Updater response indicating the issuing bank does not participate in the update program'),
        );
        break;
      case '504' :
        $result = array(
          'response_message' => t('Contact the cardholder for updated information'),
          'response_type' => t('N/A'),
          'descriptions' => t('An Account Updater response indicating you should contact the cardholder directly for updated information.'),
        );
        break;
      case '505' :
        $result = array(
          'response_message' => t('No match found'),
          'response_type' => t('N/A'),
          'descriptions' => t('An Account Updater response indicating no match was found in the updated information.'),
        );
        break;
      case '506' :
        $result = array(
          'response_message' => t('No changes found'),
          'response_type' => t('N/A'),
          'descriptions' => t('An Account Updater response indicating there have been no changes to the account information.'),
        );
        break;
      case '550' :
        $result = array(
          'response_message' => t('Restricted Device or IP - ThreatMetrix Fraud Score Below Threshold'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The transaction was declined because the resulting ThreatMetrix Fraud Score was
            below the acceptable threshold set in the merchant’s policy.'),
        );
        break;
      case '601' :
        $result = array(
          'response_message' => t('Soft Decline - Primary Funding Source Failed'),
          'response_type' => t('Soft Decline'),
          'descriptions' => t('A PayPal response indicating the transaction failed due to an issue
            with primary funding source (e.g. expired Card, insufficient funds, etc.).'),
        );
        break;
      case '602' :
        $result = array(
          'response_message' => t('Soft Decline - Buyer has alternate funding source'),
          'response_type' => t('Soft Decline'),
          'descriptions' => t('A PayPal response indicating the merchant may resubmit the transaction immediately, and the use
            of an alternate funding source will be attempted.'),
        );
        break;
      case '610' :
        $result = array(
          'response_message' => t('Hard Decline - Invalid Billing Agreement Id'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A PayPal response indicating the Billing Agreement ID is invalid.'),
        );
        break;
      case '611' :
        $result = array(
          'response_message' => t('Hard Decline - Primary Funding Source Failed'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A PayPal response indicating the issuer is unavailable.'),
        );
        break;
      case '612' :
        $result = array(
          'response_message' => t('Hard Decline - Issue with Paypal Account'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A PayPal response indicating the transaction failed due to an issue with the buyer account.'),
        );
        break;
      case '613' :
        $result = array(
          'response_message' => t('Hard Decline - PayPal authorization ID missing'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A PayPal response indicating the need to correct the authorization ID before resubmitting.'),
        );
        break;
      case '614' :
        $result = array(
          'response_message' => t('Hard Decline - confirmed email address is not available'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A PayPal response indicating your account is configured to
            decline transactions without a confirmed address. request another payment method or
            contact support@litle.com to modify your account settings.'),
        );
        break;
      case '615' :
        $result = array(
          'response_message' => t('Hard Decline - PayPal buyer account denied'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A PayPal response indicating account unauthorized payment risk.'),
        );
        break;
      case '616' :
        $result = array(
          'response_message' => t('Hard Decline - PayPal buyer account restricted'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A PayPal response indicating PayPal is unable to process the
            payment. Buyer should contact PayPal with questions.'),
        );
        break;
      case '617' :
        $result = array(
          'response_message' => t('Hard Decline - PayPal order has been voided, expired, or completed'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A PayPal response indicating no further authorizations/captures
            can be processed against this order. A new order must be created.'),
        );
        break;
      case '618' :
        $result = array(
          'response_message' => t('Hard Decline - issue with PayPal refund'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A PayPal response indicating one of these potential refund related
            issues: duplicate partial refund must be less than or equal to original or remaining amount,
            past time limit, not allowed for transaction type, consumer account locked/inactive, or
            complaint exists - only a full refund of total/remaining amount allowed.
            Contact support@litle.com for specific details.'),
        );
        break;
      case '619' :
        $result = array(
          'response_message' => t('Hard Decline - PayPal credentials issue'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A PayPal response indicating you do not have permissions to make this API call.'),
        );
        break;
      case '620' :
        $result = array(
          'response_message' => t('Hard Decline - PayPal authorization voided or expired'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A PayPal response indicating you cannot capture against this authorization. You need to
            perform a brand new authorization for the transaction.'),
        );
        break;
      case '621' :
        $result = array(
          'response_message' => t('Hard Decline - required PayPal parameter missing'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A PayPal response indicating missing parameters are required.
            Contact support@litle.com for specific details.'),
        );
        break;
      case '622' :
        $result = array(
          'response_message' => t('Hard Decline - PayPal transaction ID or auth ID is invalid'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A PayPal response indicating the need to check the validity of the
            authorization ID prior to reattempting the transaction.'),
        );
        break;
      case '623' :
        $result = array(
          'response_message' => t('Hard Decline - Exceeded maximum number of PayPal authorization attempts'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A PayPal response indicating you should capture against a previous authorization.'),
        );
        break;
      case '624' :
        $result = array(
          'response_message' => t('Hard Decline - Transaction amount exceeds merchant’s PayPal account limit.'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A PayPal response indicating the transaction amount exceeds the
            merchant’s account limit. Contact support@litle.com to modify your account settings.'),
        );
        break;
      case '625' :
        $result = array(
          'response_message' => t('Hard Decline - PayPal funding sources unavailable.'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A PayPal response indicating the buyer needs to add another funding sources to their account.'),
        );
        break;
      case '626' :
        $result = array(
          'response_message' => t('Hard Decline - issue with PayPal primary funding source.'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A PayPal response indicating there are issues with the buyer’s primary funding source.'),
        );
        break;
      case '627' :
        $result = array(
          'response_message' => t('Hard Decline - PayPal profile does not allow this transaction type.'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('Contact Litle to adjust your PayPal merchant profile preferences.'),
        );
        break;
      case '628' :
        $result = array(
          'response_message' => t('Internal System Error with PayPal - Contact Litle'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('There is a problem with Litle’s username and password. Contact support@litle.com.'),
        );
        break;
      case '629' :
        $result = array(
          'response_message' => t('Hard Decline - Contact PayPal consumer for another payment method'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A PayPal response indicating you should contact the consumer for another payment method.'),
        );
        break;
      case '637' :
        $result = array(
          'response_message' => t('Invalid terminal Id'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The terminal Id submitted with the POS transaction is invalid.'),
        );
        break;
      case '701' :
        $result = array(
          'response_message' => t('Under 18 years old'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A Bill Me Later (BML) response indicating the customer is under 18 years of age based upon the date of birth.'),
        );
        break;
      case '702' :
        $result = array(
          'response_message' => t('Bill to outside USA.'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A BML response indicating the billing address is outside the United States.'),
        );
        break;
      case '703' :
        $result = array(
          'response_message' => t('Bill to address is not equal to ship to address'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A BML response indicating that the billing address does not match the shipping address.'),
        );
        break;
      case '704' :
        $result = array(
          'response_message' => t('Declined, foreign currency, must be USD'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A BML response indicating the transaction is declined, because it is not in US dollars.'),
        );
        break;
      case '705' :
        $result = array(
          'response_message' => t('On negative file'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A BML response indicating the account is on the negative file.'),
        );
        break;
      case '706' :
        $result = array(
          'response_message' => t('Blocked agreement'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A BML response indicating a blocked agreement account status.'),
        );
        break;
      case '707' :
        $result = array(
          'response_message' => t('Insufficient buying power'),
          'response_type' => t('Other'),
          'descriptions' => t('A BML response indicating that the account holder does not have sufficient credit available for the transaction amount.'),
        );
        break;
      case '708' :
        $result = array(
          'response_message' => t('Invalid Data'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A BML response indicating that there are one or more problems with the submitted data.'),
        );
        break;
      case '709' :
        $result = array(
          'response_message' => t('Invalid Data - data elements missing'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A BML response indicating one or more required data elements are missing.
            Also, returned for an eCheck transaction that is missing a required data element.
            For example, failure to include the name element in an echeckSale or echeckCredit transaction
            would result in this code being returned.'),
        );
        break;
      case '710' :
        $result = array(
          'response_message' => t('Invalid Data - data format error'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A BML response indicating that some data was formatted incorrectly.'),
        );
        break;
      case '711' :
        $result = array(
          'response_message' => t('Invalid Data - Invalid T&C version'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A BML response indicating the T&C version is invalid.'),
        );
        break;
      case '712' :
        $result = array(
          'response_message' => t('Duplicate transaction'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A BML response indicating that the transaction is a duplicate.'),
        );
        break;
      case '713' :
        $result = array(
          'response_message' => t('Verify billing address'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A BML response indicating that you should verify the billing address.'),
        );
        break;
      case '714' :
        $result = array(
          'response_message' => t('Inactive Account'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A BML response indicating the customer account is inactive.'),
        );
        break;
      case '716' :
        $result = array(
          'response_message' => t('Invalid Auth'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A BML response indicating that the referenced authorization is invalid.'),
        );
        break;
      case '717' :
        $result = array(
          'response_message' => t('Authorization already exists for the order'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A BML response indicating that an authorization already exists for the transaction.'),
        );
        break;
      case '801' :
        $result = array(
          'response_message' => t('Account number was successfully registered'),
          'response_type' => t('Approved'),
          'descriptions' => t('The card number was successfully registered and a token number was returned.'),
        );
        break;
      case '802' :
        $result = array(
          'response_message' => t('Account number was previously registered'),
          'response_type' => t('Approved'),
          'descriptions' => t('The card number was previously registered for tokenization.'),
        );
        break;
      case '805' :
        $result = array(
          'response_message' => t('Card Validation Number Updated'),
          'response_type' => t('Approved'),
          'descriptions' => t('The stored value for CVV2/CVC2/CID has been successfully updated.'),
        );
        break;
      case '820' :
        $result = array(
          'response_message' => t('Credit card number was invalid'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The card number submitted for tokenization is invalid.'),
        );
        break;
      case '821' :
        $result = array(
          'response_message' => t('Merchant is not authorized for tokens'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('Your organization is not authorized to use tokens.'),
        );
        break;
      case '822' :
        $result = array(
          'response_message' => t('Token was not found'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The token number submitted with this transaction was not found.'),
        );
        break;
      case '850' :
        $result = array(
          'response_message' => t('Tax Billing only allowed for MCC9311'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('Tax Billing elements are allowed only for MCC 9311.'),
        );
        break;
      case '851' :
        $result = array(
          'response_message' => t('MCC 9311 requires taxType element'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('Missing taxType element'),
        );
        break;
      case '852' :
        $result = array(
          'response_message' => t('Debt Repayment only allowed for VI transactions on MCCs 6012 and 6051'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('You must be either MCC 6012 or 6051 to designate a Visa
          transaction as Debt Repayment (debtRepayment element set to true).'),
        );
        break;
      case '877' :
        $result = array(
          'response_message' => t('Invalid Pay Page Registration Id'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A Pay Page response indicating that the Pay Page Registration ID submitted is invalid.'),
        );
        break;
      case '878' :
        $result = array(
          'response_message' => t('Expired Pay Page Registration Id'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('A Pay Page response indicating that the Pay Page Registration ID has expired (Pay Page
            Registration IDs expire 24 hours after being issued).'),
        );
        break;
      case '879' :
        $result = array(
          'response_message' => t('Merchant is not authorized for Pay Page'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('Your organization is not authorized to use the Pay Page.'),
        );
        break;
      case '898' :
        $result = array(
          'response_message' => t('Generic token registration error'),
          'response_type' => t('Soft Decline'),
          'descriptions' => t('There is an unspecified token registration error; contact Litle & Co.'),
        );
        break;
      case '899' :
        $result = array(
          'response_message' => t('Generic token use error'),
          'response_type' => t('Soft Decline'),
          'descriptions' => t('There is an unspecified token use error; contact Litle & Co.'),
        );
        break;
      case '900' :
        $result = array(
          'response_message' => t('Invalid Bank Routing Number'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('The eCheck routing number submitted with this transaction has failed validation.'),
        );
        break;
      case '950' :
        $result = array(
          'response_message' => t('Decline - Negative Information on File'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('An eCheck response indicating the account is on the negative file.'),
        );
        break;
      case '951' :
        $result = array(
          'response_message' => t('Absolute Decline'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('An eCheck response indicating that this transaction was declined.'),
        );
        break;
      case '952' :
        $result = array(
          'response_message' => t('The Merchant Profile does not allow the requested operation'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('An eCheck response indicating that your Merchant Profile does not allow the requested
            operation. Contact your Litle & Co. Customer Experience Manager for additional information.'),
        );
        break;
      case '953' :
        $result = array(
          'response_message' => t('The account cannot accept ACH transactions'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('An eCheck response indicating the customer’s checking account
            does not accept ACH transactions.'),
        );
        break;
      case '954' :
        $result = array(
          'response_message' => t('The account cannot accept ACH transactions or site drafts'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('An eCheck response indicating the customer’s checking account
            does not accept ACH transactions or site drafts.'),
        );
        break;
      case '955' :
        $result = array(
          'response_message' => t('Amount greater than limit specified in the Merchant Profile'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('An eCheck response indicating that the dollar amount of this transaction exceeds the
            maximum amount specified in your Merchant Profile. Contact your Litle & Co. Customer Experience Manager for
            additional information.'),
        );
        break;
      case '956' :
        $result = array(
          'response_message' => t('Merchant is not authorized to perform eCheck Verification transactions'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('An eCheck response indicating that your organization is not authorized to perform eCheck
            verifications. Contact your Litle & Co. Customer Experience Manager for additional information.'),
        );
        break;
      case '957' :
        $result = array(
          'response_message' => t('First Name and Last Name required for eCheck Verifications'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('An eCheck response indicating that the first and last name of
            the customer is required for eCheck verifications.'),
        );
        break;
      case '958' :
        $result = array(
          'response_message' => t('Company Name required for corporate account for eCheck Verifications'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('An eCheck response indicating that the company name is required for verifications on
            corporate accounts.'),
        );
        break;
      case '959' :
        $result = array(
          'response_message' => t('Phone number required for eCheck Verifications'),
          'response_type' => t('Hard Decline'),
          'descriptions' => t('An eCheck response indicating
            that the phone number of the customer is required for eCheck verifications.'),
        );
        break;
    }
    return $result;
  }
}