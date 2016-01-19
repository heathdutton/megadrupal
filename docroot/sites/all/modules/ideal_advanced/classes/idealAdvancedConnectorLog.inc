<?php

/**
 * @file
 * Contains the IdealAdvancedConnectorLog class.
 */

/**
 * Set idealAdvancedConnectorLog namespaces.
 */

use iDEALConnector\Log\IConnectorLog as IconnectorLog;
use iDEALConnector\Entities\AbstractResponse as AbstractResponse;
use iDEALConnector\Exceptions\ConnectorException as ConnectorException;
use iDEALConnector\Exceptions\iDEALException as iDEALException;
use iDEALConnector\Entities\AbstractRequest as AbstractRequest;

/**
 * Ideal connector log class.
 *
 * This class overwrites the default file based log class.
 */
class IdealAdvancedConnectorLog implements IConnectorLog {

  /**
   * Defines the log level for the ideal_connector library and drupal logging.
   *
   * @var int
   *  The ideal connector library log level.
   */

  private $logLevel;

  /**
   * The log level is mandatory for the debugging functionality.
   *
   * @param int $log_level
   *   The ideal connector library log level.
   */
  public function __construct($log_level) {
    $this->logLevel = $log_level;
  }

  /**
   * Fetch the iDeal error exceptions.
   *
   * @param iDEALException $exception
   *   The exception which is returned by the ideal connector library.
   */
  public function logErrorResponse(iDEALException $exception) {
    drupal_set_message(check_plain($exception->getConsumerMessage()), 'error', FALSE);

    if (!$this->logLevel) {
      $this->log($exception, WATCHDOG_ERROR);
    }
    else {
      $this->log($exception->getMessage(), WATCHDOG_ERROR);
    }
  }

  /**
   * Fetch the iDeal connector library exceptions.
   *
   * @param ConnectorException $exception
   *   The exception which is returned by the ideal connector library.
   */
  public function logException(ConnectorException $exception) {
    if (!$this->logLevel) {
      $this->log($exception, WATCHDOG_ERROR);
    }
    else {
      $this->log($exception->getMessage(), WATCHDOG_ERROR);
    }
  }

  /**
   * When debugging is enabled, log the API calls.
   *
   * @param string $method
   *   The ideal connector library call method.
   * @param AbstractRequest $request
   *   The ideal connector library request object.
   */
  // @codingStandardsIgnoreStart
  public function logAPICall($method, AbstractRequest $request) {
    // @codingStandardsIgnoreEnd

    if (!$this->logLevel) {
      $this->log($request, WATCHDOG_DEBUG, 'Entering - ' . $method);
    }
  }

  /**
   * When debugging is enabled, log the API return.
   *
   * @param string $method
   *   The ideal connector library call method.
   * @param AbstractResponse $response
   *   The ideal connector library response object.
   */
  // @codingStandardsIgnoreStart
  public function logAPIReturn($method, AbstractResponse $response) {
    // @codingStandardsIgnoreEnd

    if (!$this->logLevel) {
      $this->log($response, WATCHDOG_DEBUG, 'Exiting - ' . $method);
    }
  }

  /**
   * When debugging is enabled, log the XML request.
   *
   * @param string $xml
   *   The ideal connector request XML content.
   */
  public function logRequest($xml) {
    if (!$this->logLevel) {
      $this->log($xml, WATCHDOG_DEBUG, 'Request');
    }
  }

  /**
   * When debugging is enabled, log the XML response.
   *
   * @param string $xml
   *   The ideal connector request XML content.
   */
  public function logResponse($xml) {
    if (!$this->logLevel) {
      $this->log($xml, WATCHDOG_DEBUG, 'Response');
    }
  }

  /**
   * Log the error message.
   *
   * @param string|array|object $value
   *   The main error message.
   * @param string $type
   *   Use the drupal watchdog constants: WATCHDOG_DEBUG, WATCHDOG_ERROR, etc.
   * @param string $message
   *   Additional message.
   */
  public function log($value, $type, $message = NULL) {
    if (is_array($value) || is_object($value)) {
      $value = print_r($value, TRUE);
    }

    $variables = array(
      '%value' => $value,
      '%type' => $type,
      '%msg' => $message,
    );

    watchdog('iDEAL', 'Error type: %type message: %msg %value', $variables, $type);
  }

}
