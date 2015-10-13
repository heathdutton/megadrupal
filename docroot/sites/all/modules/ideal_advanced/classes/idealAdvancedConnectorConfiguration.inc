<?php

/**
 * @file
 * Contains the IdealAdvancedConnectorConfiguration class.
 */

/**
 * Set iDEALConnector namespaces.
 */

use iDEALConnector\Configuration\IConnectorConfiguration as IConnectorConfiguration;

/**
 * Represents an iDEAL connection configuration.
 *
 * This is necessary because the default configuration class is hardwired to
 * use a text file for setting the configuration parameters.
 */
class IdealAdvancedConnectorConfiguration extends IdealAdvancedConfigHandler implements IConnectorConfiguration {

  /**
   * The basic ideal settings properties.
   *
   * @var string
   */

  private $acquirerCertificatePath;
  private $idealServer;
  private $certificatePath;
  private $expirationPeriod;
  private $merchantId;
  private $privateKeyPass;
  private $privateKeyPath;
  private $merchantReturnUrl;
  private $subId;
  private $acquirerTimeout;
  private $proxy;
  private $proxyUrl;
  private $logFile;
  private $logLevel;

  /**
   * Creates an IdealAdvancedConnectorConfiguration object.
   *
   * The connection configuration parameters are taken from the confighandler.
   *
   * @param int $config_id
   *   The ideal configuration id.
   */
  public function __construct($config_id) {
    $this->configurationLoad($config_id);
    $this->setDefaults();
    $this->setSettings();
  }

  /**
   * Set the class settings properties.
   */
  private function setSettings() {
    $this->acquirerCertificatePath = IDEAL_ADVANCED_BASE_PATH . $this->settings['public_certificate_path'];
    $this->idealServer = $this->settings['ideal_server_url'];
    $this->certificatePath = IDEAL_ADVANCED_BASE_PATH . $this->settings['private_certificate_path'];
    $this->expirationPeriod = intval($this->settings['expiration_period']);
    $this->merchantId = strval($this->settings['merchant_id']);
    if (!empty($this->settings['sub_id'])) {
      $this->subId = intval($this->settings['sub_id']);
    }

    $this->logLevel = $this->settings['debug_mode'];
    $this->privateKeyPath = IDEAL_ADVANCED_BASE_PATH . $this->settings['private_key_path'];
    $this->privateKeyPass = $this->settings['private_key_password'];
  }

  /**
   * Set the default values. When needed other values can be supplied.
   */
  protected function setDefaults() {
    $this->logFile = 'logs/connector.log';
    $this->acquirerTimeout = 10;
    $this->subId = NULL;
    $this->proxy = NULL;
    $this->proxyUrl = NULL;
  }

  /**
   * Implements IConnectorConfiguration::getAcquirerCertificatePath().
   */
  public function getAcquirerCertificatePath() {
    return $this->acquirerCertificatePath;
  }

  /**
   * Implements IConnectorConfiguration::getAcquirerDirectoryURL().
   */
  // @codingStandardsIgnoreStart
  public function getAcquirerDirectoryURL() {
    // @codingStandardsIgnoreEnd

    return $this->idealServer;
  }

  /**
   * Implements IConnectorConfiguration::getAcquirerStatusURL().
   */
  // @codingStandardsIgnoreStart
  public function getAcquirerStatusURL() {
    // @codingStandardsIgnoreEnd

    return $this->idealServer;
  }

  /**
   * Implements IConnectorConfiguration::getAcquirerTransactionURL().
   */
  // @codingStandardsIgnoreStart
  public function getAcquirerTransactionURL() {
    // @codingStandardsIgnoreEnd

    return $this->idealServer;
  }

  /**
   * Implements IConnectorConfiguration::getCertificatePath().
   */
  public function getCertificatePath() {
    return $this->certificatePath;
  }

  /**
   * Implements IConnectorConfiguration::getExpirationPeriod().
   */
  public function getExpirationPeriod() {
    return $this->expirationPeriod;
  }

  /**
   * Implements IConnectorConfiguration::getMerchantID().
   */
  // @codingStandardsIgnoreStart
  public function getMerchantID() {
    // @codingStandardsIgnoreEnd

    return $this->merchantId;
  }

  /**
   * Implements IConnectorConfiguration::getPassphrase().
   */
  public function getPassphrase() {
    return $this->privateKeyPass;
  }

  /**
   * Implements IConnectorConfiguration::getPrivateKeyPath().
   */
  public function getPrivateKeyPath() {
    return $this->privateKeyPath;
  }

  /**
   * Implements IConnectorConfiguration::getMerchantReturnURL().
   */
  // @codingStandardsIgnoreStart
  public function getMerchantReturnURL() {
    // @codingStandardsIgnoreEnd

    return $this->merchantReturnUrl;
  }

  /**
   * Implements IConnectorConfiguration::getSubID().
   */
  // @codingStandardsIgnoreStart
  public function getSubID() {
    // @codingStandardsIgnoreEnd

    return (int) $this->subId;
  }

  /**
   * Implements IConnectorConfiguration::getAcquirerTimeout().
   */
  public function getAcquirerTimeout() {
    return $this->acquirerTimeout;
  }

  /**
   * Implements IConnectorConfiguration::getProxy().
   */
  public function getProxy() {
    return $this->proxy;
  }

  /**
   * Implements IConnectorConfiguration::getProxyUrl().
   */
  public function getProxyUrl() {
    return $this->proxyUrl;
  }

  /**
   * Implements IConnectorConfiguration::getLogFile().
   */
  public function getLogFile() {
    return $this->logFile;
  }

  /**
   * Implements IConnectorConfiguration::getLogLevel().
   */
  public function getLogLevel() {
    return $this->logLevel;
  }

}
