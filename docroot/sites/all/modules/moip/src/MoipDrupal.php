<?php

namespace Drupal\moip;

/**
 * @file Class encapsulating the business logic related to integrate MoIP with
 * Drupal structures.
 */
class MoipDrupal {

  /**
   * @var \Moip
   */
  protected $moip;

  /**
   * @var \MoIPClient
   */
  protected $moipClient;
  protected $order;
  protected $order_wrapper;
  protected $token;
  protected $key;
  protected $moip_server;

  public function __construct() {
    $this->loadLibrary();
    $this->isConfigured();
    $this->basicSetup();
  }

  public function setOrder($order) {
    $this->order = $order;
    $this->order_wrapper = entity_metadata_wrapper('commerce_order', $this->order);
  }

  public function processOrder($payment_method) {

    $this->moip->setNotificationURL(url('moip/notification', array('absolute' => TRUE)));
    $this->moip->setReason($this->getReason());
    
    $amount_integer = $this->order_wrapper->commerce_order_total->amount->value();
    $currency_code = $this->order_wrapper->commerce_order_total->currency_code->value();
    $amount = round(commerce_currency_amount_to_decimal($amount_integer, $currency_code), 2);

    $this->moip->setUniqueID($this->getUniqueId());
    $this->moip->setValue($amount);

    $this->addDetailMessages();
    $this->addCustomerAddress();
    $this->addComissions();

    $this->moip->validate('Identification');

    if (variable_get('moip_debug', FALSE)) {
      watchdog('moipdbg_send', '<pre>' . print_r($this->moip, TRUE) . '</pre>');
    }

    $this->moip->send();
    $this->validateAnswer();
    return $this->processAnswer();
  }

  /**
   * Process the answer that MoIP gives you in response to the data that you send
   * @throws ErrorException
   */
  protected function processAnswer() {

    $answer = $this->moip->getAnswer();

    if ($answer->__get('response')) {
      $this->order->data['moip_payment_token'] = $answer->__get('token');
      $this->order->data['moip_payment_url'] = $answer->__get('payment_url');
      commerce_order_save($this->order);
      return $answer;
    }
    else {
      if ($answer->__get('error') == 'Formato informado para o telefone não é válido') {
        throw new Exceptions\InputValidationException(t('The informed phone number is invalid. Please use the following format: 22333344445 (10 or 11 numbers)'));
      }
      throw new \ErrorException('processAnswer: <pre>' . print_r($answer->__get('error'), TRUE) . '</pre>');
    }
  }

  protected function validateAnswer() {

    $answer = $this->moip->getAnswer();

    if (variable_get('moip_debug', FALSE)) {
      watchdog('moipdbg_answer', t('<pre>@pre</pre>', array(
        '@pre' => print_r($answer, TRUE)
      )));
    }

    if (is_string($answer) && strpos($answer, 'Error:') !== FALSE) {
      $error_msg = str_replace('Error: ', '', $answer);
      throw new \ErrorException('validateAnswer - error: ' . $error_msg);
    }

    if ($answer == 'Authentication failed') {
      $erroneous_credentials = array('server' => $this->moip_server, 'key' => $this->key, 'token' => $this->token);
      throw new \ErrorException('validateAnswer - authfailed: <pre>' . print_r($erroneous_credentials, TRUE) . '</pre>');
    }
  }

  protected function addDetailMessages() {
    if (variable_get('moip_detail_messages', 1)) {
      foreach ($this->order->commerce_line_items['und'] as $item) {
        $line_item = commerce_line_item_load($item['line_item_id']);
        $this->moip->addMessage(commerce_line_item_title($line_item));
      }
    }
  }

  protected function addComissions() {

    // Load configured comissions
    $reason = variable_get('moip_comission_reason_1');
    $user_receiver = variable_get('moip_comission_user_receiver_1');
    $type = variable_get('moip_comission_type_1');
    $value = variable_get('moip_comission_value_1');

    $configured_comissions = array();
    if (!empty($reason) && !empty($user_receiver) && isset($type) && !empty($value)) {
      $configured_comissions[] = array(
        'reason' => $reason,
        'user_receiver' => $user_receiver,
        'value' => $value,
        'is_percentual' => $type,
      );
    }

    if (count($configured_comissions) > 0) {
      foreach ($configured_comissions as $comission) {
        $this->moip->addComission(
          $comission['reason']
          , $comission['user_receiver']
          , $comission['value']
          , $comission['is_percentual']
        );
      }
    }
  }

  protected function addCustomerAddress() {

    if (isset($this->order_wrapper->commerce_customer_billing->commerce_customer_address)) {

      // Moip needs the thoroughfare number to be separate from the street
      $thoroughfare = explode(',', $this->order_wrapper->commerce_customer_billing->commerce_customer_address->thoroughfare->value());

      $payer = array(
        'name' => $this->order_wrapper->commerce_customer_billing->commerce_customer_address->name_line->value(),
        'email' => $this->order->mail,
        'payerId' => $this->order->uid,
        'billingAddress' => array(
          'address' => trim($thoroughfare[0]),
          'number' => trim($thoroughfare[1]),
          'complement' => $this->order_wrapper->commerce_customer_billing->commerce_customer_address->premise->value(),
          'neighborhood' => $this->order_wrapper->commerce_customer_billing->commerce_customer_address->dependent_locality->value(),
          'city' => $this->order_wrapper->commerce_customer_billing->commerce_customer_address->locality->value(),
          'state' => $this->order_wrapper->commerce_customer_billing->commerce_customer_address->administrative_area->value(),
          'country' => $this->order_wrapper->commerce_customer_billing->commerce_customer_address->country->value(),
          'zipCode' => $this->order_wrapper->commerce_customer_billing->commerce_customer_address->postal_code->value(),
        )
      );

      /**
       * Phone isn't a default information on Drupal Commerce, so it should be
       * checked before been sent
       */
      $phone = $this->getPhoneNumber();
      if (!empty($phone)) {
        $payer['billingAddress']['phone'] = $phone;
      }
      else {
        throw new \ErrorException('MoIP: phone was not informed for Order @order_id', array('@order_id' => $this->order->order_id));
      }

      $this->moip->setPayer($payer);
    }
  }

  /**
   * It' useful to append time() when in sandbox to avoid unique id colisions.
   */
  public function getUniqueId() {
    return $this->order->order_id . '_' . REQUEST_TIME;
  }

  /**
   *  Define a reason dinamically, with tokens, or use the default text
   */
  protected function getReason() {
    $reason_token = variable_get('moip_order_reason_token');
    if (empty($reason_token)) {
      return t('Order @order_number at @store', array('@order_number' => $this->order->order_number, '@store' => variable_get('site_name', url('<front>', array('absolute' => TRUE)))));
    }
    else {
      return token_replace($reason_token, array('@order' => $this->order));
    }
  }

  // Define a reason dinamically, with tokens, or use the default text
  protected function getPhoneNumber() {
    $phone_token = variable_get('moip_phone_token');
    if (empty($phone_token)) {
      return NULL;
    }
    else {
      return token_replace($phone_token, array('commerce-order' => $this->order));
    }
  }

  /**
   * Go on Moip to get the latest info about a commerce order
   */
  public function getOrderDetailsOnMoip($payment_transaction_token) {
    if (empty($this->moipClient)) {
      $this->moipClient = new \MoIPClient();
    }
    $credentials = $this->token . ':' . $this->key;
    $url = $this->getEnvironmentURL() . '/ws/alpha/ConsultarInstrucao/' . $payment_transaction_token;
    $moipResponse = $this->moipClient->curlGet($credentials, $url);
    return simplexml_load_string($moipResponse->__get('xml'));
  }

  public function getEnvironmentURL() {
    if ($this->isSandbox()) {
      return 'https://desenvolvedor.moip.com.br/sandbox';
    }
    else {
      return 'https://www.moip.com.br';
    }
  }

  protected function isSandbox() {
    return $this->moip_server;
  }

  /**
   * Basic data to be sent to MoIP
   */
  protected function basicSetup() {
    $this->moip = new \Moip();
    $this->moip->setEnvironment($this->moip_server);
    $this->moip->setCredential(array(
      'key' => $this->key,
      'token' => $this->token
    ));
  }

  /**
   * Checks if the basic information about MoIP credentials and environment were
   * informed.
   * @throws Exception
   */
  protected function isConfigured() {
    $this->token = variable_get('moip_token');
    $this->key = variable_get('moip_key');
    $this->moip_server = variable_get('moip_server');
    // Return an error if MoIP haven't been configured.
    if (empty($this->token) || empty($this->key) || !isset($this->moip_server)) {
      throw new \Exception(t('MoIP API Integration is not configured for use. Please go to '
        . l('/admin/config/services/moip', '/admin/config/services/moip'))
      , 'error');
    }
  }

  /**
   * Returns TRUE if the Moip class is available.
   */
  protected function loadLibrary() {

    $moip_lib_url = 'https://github.com/moiplabs/moip-php';
    $path = 'sites/all/libraries/moip-php-master';

    if (file_exists($path)) {
      require_once $path . '/lib/Moip.php';
      require_once $path . '/lib/MoipClient.php';
      require_once $path . '/lib/MoipStatus.php';
    }
    else {
      throw new \Exception(t('MoIP library was not found on "' . $path . '". Download it in !link', array('!link' => $moip_lib_url)));
    }

    // Maybe something loaded the class without telling libraries API.
    if (class_exists('Moip') && class_exists('MoipClient') && class_exists('MoipStatus')) {
      return TRUE;
    }
    return (class_exists('Moip') && class_exists('MoipClient') && class_exists('MoipStatus'));
  }

}
