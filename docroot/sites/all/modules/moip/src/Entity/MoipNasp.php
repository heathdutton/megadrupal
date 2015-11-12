<?php

namespace Drupal\moip\Entity;

/**
 * @file
 * Class encapsulating the business logic related to the processing of 
 * notifications data that MoIP sends back to Drupal.
 */
class MoipNasp {

  private $moip_data;
  private $order_id;
  private $payment_method = 'moip_ct';

  /**
   * 
   * @param type $data $_POST data or array based on it to process a Moip NASP
   * @return \MoipNasp
   * @throws Exception
   */
  static public function create($data) {

    if (variable_get('moip_debug', FALSE)) {
      watchdog('moip_debug', '[processMoipData] create <pre>@pre</pre>', array(
        '@pre' => print_r($data, TRUE)
      ));
    }

    // Checks what is being passed to the url, preventing hijacks
    if (empty($data['id_transacao']) || empty($data['cod_moip'])) {
      watchdog(
        'moip'
        , t('An attempt to access NASP notification url was made, with wrong parameters. See below: <pre>@pre</pre>', array(
          '@pre' => print_r($_REQUEST, TRUE)
        ))
        , array()
        , WATCHDOG_ERROR
      );
      throw new Exception(t('Wrong parameters was sent to the MoIP integration notification page'));
    }

    return new MoipNasp($data);
  }

  /**
   * base constructor of class, it gets as parameter an $_POST from MOIP
   */
  public function __construct($moip_data) {

    if (!empty($moip_data)) {
      $this->moip_data = $moip_data;
      $id_transacao = explode('_', $this->moip_data['id_transacao']);
      $this->order_id = $id_transacao[0];
      $this->moip_data['order_id'] = $this->order_id;
    }
    else {
      throw new ErrorException('Moip data is null');
    }
  }

  /**
   * Main controller for processing MoIP NASP data, ensuring database integrity 
   * with a db_transaction and making it easy to extend with another class, in 
   * projects with special use cases
   * @throws Exception
   */
  public function processMoipData() {

    $transaction = db_transaction();

    try {
      $this->savePaymentTransaction();

      if (variable_get('moip_debug', FALSE)) {
        watchdog('moip_debug', '[processMoipData] updated <pre>@pre</pre>', array(
          '@pre' => print_r($this, TRUE)
        ));
      }

      $this->updateOrderStatus();
      $this->save();
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('moip_error', $e);
      throw $e;
    }
  }

  public function savePaymentTransaction() {

    // Check if this is an update to an existing transaction or if we need to 
    // create one
    $previous_nasp_data = $this->load($this->moip_data['cod_moip']);

    if ($previous_nasp_data) {
      // Load the prior NASP's transaction and update that with the capture values.
      $transaction = commerce_payment_transaction_load($previous_nasp_data['transaction_id']);
    }
    else {
      // Create a new payment transaction for the order.
      $transaction = commerce_payment_transaction_new($this->payment_method, $this->order_id);
      $transaction->instance_id = $this->payment_method;
    }

    $transaction->remote_id = $this->moip_data['cod_moip'];
    $transaction->amount = $this->moip_data['valor'];
    // MoIP supports only Brazilian Reais.
    $transaction->currency_code = 'BRL';
    $transaction->payload[REQUEST_TIME] = $this->moip_data;

    // Set the transaction's statuses based on the NASP's status_pagamento.
    $transaction->remote_status = $this->moip_data['status_pagamento'];

    switch ($this->moip_data['status_pagamento']) {
      case MOIP_STATUS_INITIALIZED:
      case MOIP_STATUS_PRINTED:
      case MOIP_STATUS_PENDING:
        $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
        break;
      case MOIP_STATUS_AUTHORIZED:
      case MOIP_STATUS_COMPLETED:
        $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
        break;
      case MOIP_STATUS_CANCELED:
      case MOIP_STATUS_REVERSED:
      case MOIP_STATUS_REFUNDED:
        $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
        break;
    }

    $transaction->message = $this->getStatusDescription($this->moip_data['status_pagamento']);
    $transaction->message_variables = array();

    commerce_payment_transaction_save($transaction);
    $this->moip_data['transaction_id'] = $transaction->transaction_id;

    $this->transaction = $transaction;

    if (variable_get('moip_debug', FALSE)) {
      watchdog('moip_debug', '[savePaymentTransaction] processed for order @order_number, with ID @cod_moip, changing it status to @status <pre>@pre</pre>', array(
        '@cod_moip' => $this->moip_data['cod_moip']
        , '@order_number' => $this->order_id
        , '@status' => self::getCorrespondingStatus($this->moip_data['status_pagamento']
        )), WATCHDOG_INFO);
    }
  }

  /**
   * Update drupal order status, according to data from Nasp, doing the 
   * correlation with MoIP and Drupal Commerce statuses.
   */
  public function updateOrderStatus() {
    if ($this->moip_data['cod_moip'] >= $this->loadLastCodMoip()) {
      $order = commerce_order_load($this->order_id);
      commerce_order_status_update(
          $order, self::getCorrespondingStatus($this->moip_data['status_pagamento'])
          , FALSE, NULL, $this->getStatusDescription($this->moip_data['status_pagamento'])
      );
    }
  }

  /**
   * Persist data from Moip Nasp to the database
   * @return boolean
   */
  public function save() {

    $previous_nasp_data = $this->load($this->moip_data['cod_moip']);

    if (!empty($this->moip_data['cod_moip']) && $previous_nasp_data) {

      if ($previous_nasp_data['id_transacao'] != $this->moip_data['id_transacao']) {
        watchdog('moip', "NASP Transaction id's doesn't match");
        throw new ErrorException("Error NASP Transaction id's doesn't match");
      }
      else {
        $this->moip_data['nasp_id'] = $previous_nasp_data['nasp_id'];
        $this->moip_data['changed'] = REQUEST_TIME;
        return drupal_write_record('moip_nasp', $this->moip_data, 'nasp_id');
      }
    }
    else {

      $this->moip_data['created'] = REQUEST_TIME;
      $this->moip_data['changed'] = REQUEST_TIME;

      return drupal_write_record('moip_nasp', $this->moip_data);
    }
  }

  /**
   * Return a list of payment statuses, or an specific payment status message.
   *
   * @param $status
   *   The status identification in which to return the message.
   */
  public function getStatusDescription($status = NULL) {
    $statuses = array(
      MOIP_STATUS_INITIALIZED => t('The payment is not done yet, or user has abandoned the payment process.'),
      MOIP_STATUS_PRINTED => t('The payment receipt "Boleto" was printed. Awaiting offline payment.'),
      MOIP_STATUS_PENDING => t('The customer paid with a credit card and the payment is awaiting manual review from MoIP team.'),
      MOIP_STATUS_AUTHORIZED => t('The payment was authorized but not completed yet, due to the normal flow of chosen payment method.'),
      MOIP_STATUS_COMPLETED => t('The payment was completed and the money was credited in the recipient account.'),
      MOIP_STATUS_CANCELED => t('The payment was canceled by the payer, payment institution, MoIP or recipient account.'),
      MOIP_STATUS_REVERSED => t('The payment was reversed by the payer.'),
      MOIP_STATUS_REFUNDED => t('The payment was refunded to the payer.'),
    );
    return empty($status) ? $statuses : $statuses[$status];
  }

  /**
   * Returns the corresponding status from Drupal Commerce, for the id used in
   * MoIP
   *
   * @param int $remote_status_id
   *   The status from MoIP
   * @return string
   */
  public static function getCorrespondingStatus($remote_status_id) {
    $statuses = array(
      MOIP_STATUS_INITIALIZED => 'pending',
      MOIP_STATUS_PRINTED => 'pending',
      MOIP_STATUS_PENDING => 'processing',
      MOIP_STATUS_AUTHORIZED => 'payment_authorized',
      MOIP_STATUS_COMPLETED => 'completed',
      MOIP_STATUS_CANCELED => 'canceled',
      MOIP_STATUS_REVERSED => 'canceled',
      MOIP_STATUS_REFUNDED => 'canceled'
    );
    if (!in_array($remote_status_id, array_keys($statuses))) {
      throw new ErrorException(t('The payment status sent by MoIP was not recognized by MoIP module. Please file an issue at http://drupal.org/project/moip.'));
    }
    return $statuses[$remote_status_id];
  }

  /**
   * Load a MoiP Nasp data from moip_nasp table based on the cod_moip
   */
  public function load($cod_moip) {
    $query = db_select('moip_nasp')
        ->fields('moip_nasp')
        ->condition('cod_moip', $cod_moip)
        ->execute()
        ->fetchAssoc();
    return empty($query) ? FALSE : $query;
  }

  /**
   * Load the last a MoiP Nasp data from moip_nasp table based on the cod_moip
   */
  public function loadLastCodMoip() {
    $result = db_query('select max(cod_moip) as cod_moip from {moip_nasp} where order_id = :order_id', array(':order_id' => $this->order_id))->fetchObject();
    return empty($result) ? FALSE : $result->cod_moip;
  }

}
