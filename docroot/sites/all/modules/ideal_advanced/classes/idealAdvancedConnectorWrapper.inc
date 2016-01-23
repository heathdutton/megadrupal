<?php

/**
 * @file
 * Contains the IdealAdvancedConnectorWrapper class.
 */

/**
 * Set iDEALConnector namespaces.
 */

use iDEALConnector\iDEALConnector;
use iDEALConnector\Exceptions\ValidationException;
use iDEALConnector\Exceptions\SecurityException;
use iDEALConnector\Exceptions\SerializationException;
use iDEALConnector\Entities\Transaction;
use iDEALConnector\Entities\AcquirerTransactionResponse as AcquirerTransactionResponse;
use iDEALConnector\Entities\AcquirerStatusResponse as AcquirerStatusResponse;

/**
 * Ideal connector wrapper class.
 */
class IdealAdvancedConnectorWrapper {

  /**
   * Define connector constants.
   */
  const ISSUERS_CACHE_ID = 'ideal_connector_issuers';

  /**
   * Ideal connector library object.
   *
   * @var object
   */
  private $connector;

  /**
   * The log handling object.
   *
   * @var object
   */
  private $logHandler;

  /**
   * The ideal configuration id.
   *
   * @var string
   */
  private $configId;

  /**
   * The connector library configuration handling object.
   *
   * @var object
   */
  private $configHandler;

  /**
   * The issuer id selected from the getIssuers() method.
   *
   * @var string
   */
  private $issuerId;

  /**
   * Set the issuers id selected by the client.
   *
   * @param string $issuer_id
   *   The issuer id selected from the getIssuers() method.
   */
  public function setIssuerId($issuer_id) {
    $this->issuerId = $issuer_id;
  }

  /**
   * The payment amount in cents: 100,- EUR is 10000.
   *
   * @var int
   */
  private $paymentAmount;

  /**
   * The payment amount in decimal: 100,- EUR is 10000.
   *
   * @param int $amount
   *   The ideal advanced transaction amount in cents.
   */
  public function setAmount($amount) {
    $this->paymentAmount = $amount;
  }

  /**
   * The payment description.
   *
   * @var string
   */
  private $paymentDescription;

  /**
   * The payment description.
   *
   * @param string $description
   *   The ideal issuers transaction description.
   */
  public function setDescription($description) {
    $this->paymentDescription = $description;
  }

  /**
   * The return entrance code.
   *
   * This code is used for linking the payment to the order when the client
   * is redirected back to the site.
   *
   * @var string
   */
  private $entranceCode;

  /**
   * Set the return entrance code.
   *
   * @param string $entrance_code
   *   The entrance code for returning to the site.
   */
  public function setEntranceCode($entrance_code) {
    $this->entranceCode = $entrance_code;
  }

  /**
   * The sites purchase id.
   *
   * @var string
   */
  private $purchaseId;

  /**
   * The sites purchase id.
   *
   * @param string $purchase_id
   *   The issuers purchase id. For example the commerce order id.
   */
  public function setPurchaseId($purchase_id) {
    $this->purchaseId = $purchase_id;
  }

  /**
   * The sites return URL.
   *
   * @var string
   */
  private $returnURL;

  /**
   * The sites return url.
   *
   * @param string $return_url
   *   The sites return url.
   */
  public function setReturnUrl($return_url) {
    $this->returnURL = $return_url;
  }

  /**
   * Load the idealConnector library on object creation.
   *
   * @param int $config_id
   *   The used ideal configuration ID.
   */
  public function __construct($config_id) {
    $this->configId = $config_id;
    $this->connector = $this->loadLibrary();
  }

  /**
   * Load the ideal connector library.
   *
   * @return object
   *   The ideal connector library object.
   */
  private function loadLibrary() {
    $library = NULL;

    try {
      if (($connector = libraries_load('ideal_connector')) && empty($connector['loaded'])) {
        throw new Exception(t('Could not load the ideal_connector library'));
      }

      $this->configHandler = new IdealAdvancedConnectorConfiguration($this->configId);
      $this->logHandler = new IdealAdvancedConnectorLog($this->configHandler->getLogLevel());
      $library = new iDEALConnector($this->configHandler, $this->logHandler);

    }
    catch (Exception $ex) {
      watchdog('iDEAL', 'Error type: %type message: %msg', array('%msg' => $ex->getMessage(), '%type' => 'error'), WATCHDOG_ERROR);
    }

    return $library;
  }

  /**
   * Get the available issuers.
   *
   * Get the list from cache when available, otherwise do a directory request.
   *
   * @return array
   *   An array with issuers by ID per country.
   */
  public function getIssuers() {
    $result = array();
    $cache = cache_get(self::ISSUERS_CACHE_ID . $this->configId);

    if (empty($cache)) {
      $directory_response = $this->directoryRequest();

      if ($directory_response) {
        foreach ($directory_response->getCountries() as $country) {
          $country_name = $country->getCountryNames();
          foreach ($country->getIssuers() as $issuer) {
            $issuer_id = $issuer->getId();
            $result[$country_name][$issuer_id] = $issuer->getName();
          }
        }
        cache_set(self::ISSUERS_CACHE_ID . $this->configId, $result, 'cache', REQUEST_TIME + 86400);
      }
    }
    else {
      $result = $cache->data;
    }
    return $result;
  }

  /**
   * Return the ideal issuers object.
   *
   * The available methods are:
   * - getAcquirerID()
   * - getDirectoryDate()
   * - getCountries()
   *     + getCountryNames()
   *     + getIssuers()
   *         # getId()
   *         # getName()
   *
   * @return object
   *   The deal connector issuers object.
   */
  private function directoryRequest() {
    $issuers = NULL;

    try {
      if (!isset($this->connector)) {
        throw new Exception('iDEALConnector was not properly initialized.');
      }

      $issuers = $this->connector->getIssuers();
    }
    catch (ValidationException $ex) {
      $this->logHandler->log($ex, WATCHDOG_ERROR);
    }
    catch (SerializationException $ex) {
      $this->logHandler->log($ex, WATCHDOG_ERROR);
    }
    catch (SecurityException $ex) {
      $this->logHandler->log($ex, WATCHDOG_ERROR);
    }
    catch (Exception $ex) {
      $this->logHandler->log($ex, WATCHDOG_ERROR);
    }

    return $issuers;
  }

  /**
   * Invoke the ideal transaction.
   *
   * The amount is send as float. Round is used as extra security for the situation where
   * a discount is calculated without any rounding.
   *
   * @return object
   *   The ideal response
   *    ->getTransactionID()
   *    ->getAcquirerID()
   *    ->getIssuerAuthenticationURL()
   */
  public function startTransaction() {
    $response = NULL;

    try {
      if (!isset($this->connector)) {
        throw new Exception('iDEALConnector was not properly initialized.');
      }

      $this->transactionValuesValidate();
      $settings = $this->configHandler->getSettings();
      $response = $this->connector->startTransaction(
        $this->issuerId,
        new Transaction(
          round(($this->paymentAmount / 100), 2),
          $this->paymentDescription,
          $this->entranceCode,
          intval($settings['expiration_period']),
          strval($this->purchaseId),
          'EUR',
          'nl'
        ),
        $this->returnURL
      );

      $values = array(
        'status' => IDEAL_STATUS_NEW,
        'created' => REQUEST_TIME,
        'changed' => REQUEST_TIME,
        'expiration_period' => $settings['expiration_period'] * 60,
        'amount' => $this->paymentAmount,
        'entrance_code' => $this->entranceCode,
        'config_id' => $settings['id'],
      );

      $this->logTransaction($response, $values);
    }
    catch (ValidationException $ex) {
      $this->logHandler->log($ex, WATCHDOG_ERROR);
    }
    catch (SerializationException $ex) {
      $this->logHandler->log($ex, WATCHDOG_ERROR);
    }
    catch (SecurityException $ex) {
      $this->logHandler->log($ex, WATCHDOG_ERROR);
    }
    catch (Exception $ex) {
      $this->logHandler->log($ex, WATCHDOG_ERROR);
    }
    return $response;
  }

  /**
   * Get the current status of an ideal transaction.
   *
   * The following values are only returned on success.
   * - consumer_name
   * - consumer_iban
   * - consumer_bic
   * - amount
   *
   * @param string $transaction_id
   *   The issuers transaction id.
   *
   * @return object
   *   The issuers status response object.
   */
  private function getTransactionStatus($transaction_id) {
    $response = NULL;

    try {
      if (!isset($this->connector)) {
        throw new Exception('iDEALConnector was not properly initialized.');
      }

      $response = $this->connector->getTransactionStatus($transaction_id);
    }
    catch (ValidationException $ex) {
      $this->logHandler->log($ex, WATCHDOG_ERROR);
    }
    catch (SerializationException $ex) {
      $this->logHandler->log($ex, WATCHDOG_ERROR);
    }
    catch (SecurityException $ex) {
      $this->logHandler->log($ex, WATCHDOG_ERROR);
    }
    catch (Exception $ex) {
      $this->logHandler->log($ex, WATCHDOG_ERROR);
    }
    return $response;
  }

  /**
   * Log the transaction status to the database after the initial transaction.
   *
   * Additional values can be added tot the acquirer response object
   * See the ideal_advanced_transaction_create() function for more details.
   *
   * @param AcquirerTransactionResponse $response
   *   The issuers transaction response object.
   * @param array $values
   *   Additional values which can be added to the ideal transaction.
   *
   * @return object
   *   The ideal advanced transaction object.
   */
  private function logTransaction(AcquirerTransactionResponse $response, array $values = NULL) {
    $response_values = array(
      'transaction_id' => $response->getTransactionID(),
      'purchase_id' => $response->getPurchaseId(),
    );

    return ideal_advanced_transaction_create(array_merge($values, $response_values));
  }

  /**
   * Log the transaction status to the database after a status request.
   *
   * The following values are only returned on success.
   * - consumer_name
   * - consumer_iban
   * - consumer_bic
   * - amount
   * It is also possible to supply your own values:
   * - consumer_name
   * - consumer_iban
   * - consumer_bic
   * - amount
   * - status
   * - purchase_id
   *
   * The amount is returned as float by the ideal service.
   * We need to convert this to cents to have one common
   * standard.
   *
   * @param object $transaction
   *   The ideal advanced transaction object.
   * @param AcquirerStatusResponse $response
   *   The issuers ideal status response object.
   * @param array $values
   *   Additional values which can be added to the ideal transaction.
   *
   * @return object
   *   The ideal advanced transaction object.
   */
  private function logTransactionStatus($transaction, AcquirerStatusResponse $response = NULL, $values = array()) {
    $success_values = array();
    $response_values = array();

    if (isset($response)) {
      $response_values = array(
        'status' => $response->getStatus(),
      );
    }

    if (isset($response) && $response->getStatus() == IDEAL_STATUS_SUCCESS) {
      $success_values = array(
        'consumer_name' => $response->getConsumerName(),
        'consumer_iban' => $response->getConsumerIBAN(),
        'consumer_bic' => $response->getConsumerBIC(),
        'amount' => $response->getAmount() * 100,
      );
    }

    $values = array_merge($response_values, $success_values, $values);

    return ideal_advanced_transaction_edit($transaction->ideal_id, $values);
  }

  /**
   * Update the transaction status.
   *
   * We can't call ideal for the transaction status all the time. That's why we
   * check the status from the log and update it when the ideal status has
   * changed. An updated transaction object is returned.
   *
   * @param int $ideal_id
   *   The ideal advanced transaction id.
   *
   * @return object
   *   The ideal advanced transaction object.
   */
  public function updateTransactionStatus($ideal_id) {
    $transaction = ideal_advanced_transaction_load($ideal_id);
    $status_old = $transaction->status;

    if ($transaction->status == IDEAL_STATUS_NEW || ($transaction->status == IDEAL_STATUS_OPEN && $this->transactionTimeValidate($transaction))) {
      $response = $this->getTransactionStatus($transaction->transaction_id);
      if ($response) {
        $this->logTransactionStatus($transaction, $response);
        $status_new = $transaction->status;
        $changed = $status_new != $status_old ? TRUE : FALSE;

        module_invoke_all('ideal_advanced_status_request', $ideal_id, $transaction, $changed);
      }
    }

    return $transaction;
  }

  /**
   * A maximum of five status request is allowed per transaction.
   *
   * The five status requests are evenly spread over 24 hours.
   *
   * @param object $transaction
   *   The ideal advanced transaction object.
   *
   * @return bool
   *   True on success.
   */
  private function transactionTimeValidate($transaction) {
    $result = FALSE;
    $expiration_period = FALSE;
    $one_day = FALSE;
    $max_request = FALSE;
    $current_time = REQUEST_TIME;

    if ($transaction->created + $transaction->expiration_period < $current_time) {
      $expiration_period = TRUE;
    }
    if ($transaction->created + $transaction->expiration_period > $current_time - 84600) {
      $one_day = TRUE;
    }
    if ($transaction->changed < $current_time - 17280) {
      $max_request = TRUE;
    }
    if ($expiration_period && $one_day && $max_request) {
      $result = TRUE;
    }

    return $result;
  }

  /**
   * Validate the transaction values.
   *
   * @throws ValidationException
   *   Throws a validation exception when there are empty values.
   */
  private function transactionValuesValidate() {
    $empty_vars = array();

    if (!isset($this->issuerId)) {
      $empty_vars[] = 'issuerId';
    }
    if (!isset($this->paymentAmount)) {
      $empty_vars[] = 'paymentAmount';
    }
    if (!isset($this->paymentDescription)) {
      $empty_vars[] = 'paymentDescription';
    }
    if (!isset($this->entranceCode)) {
      $empty_vars[] = 'entranceCode';
    }
    if (!isset($this->purchaseId)) {
      $empty_vars[] = 'purchaseId';
    }
    if (!isset($this->returnURL)) {
      $empty_vars[] = 'returnURL';
    }
    if (!empty($empty_vars)) {
      $empty_vars = implode(', ', $empty_vars);
      throw new ValidationException(t("The supplied transaction values: @emptyVars are incomplete", array('@emptyVars' => $empty_vars)));
    }
  }

}
