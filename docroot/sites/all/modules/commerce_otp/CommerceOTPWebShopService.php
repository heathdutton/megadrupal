<?php
/**
 * @file
 * Custom Webservice client extends OTP's WebShopService.
 *
 * WEBSHOP_LIB_DIR defined in commerce_otp.fiz3_control.php.inc
 * LOG4PHP_DEFAULT_INIT_OVERRIDE, WEBSHOP_CONF_DIR needed for otpwebshop Library
 */

define('WEBSHOP_CONF_DIR', libraries_get_path('otpwebshop') . '/config');
require_once WEBSHOP_LIB_DIR . '/iqsys/otpwebshop/WebShopService.php';

/**
 * Custom CommerceOTPWebShopService class to implement constructor.
 */
class CommerceOTPWebShopService extends WebShopService {

  /**
   * Merge config ini file data with Commerce OTP Payment configuration.
   */
  public function __construct($pos_id) {
    $this->logger =& LoggerManager::getLogger("WebShopClient");
    $this->property = parse_ini_file(WEBSHOPSERVICE_CONFIGURATION);

    $query = db_select('commerce_otp_posid', 'cp')
      ->fields('cp', array('instance_id'))
      ->condition('cp.pos_id', $pos_id, '=');
    $result = $query->execute()->fetchAssoc();
    $instance_id = 'otp|' . $result['instance_id'];
    $payment_method = commerce_payment_method_instance_load($instance_id);
    $drupal_config = $payment_method['settings'];
    unset($drupal_config['preset']);
    // Set config variable names as WebShopService needs.
    foreach ($drupal_config as $key => $value) {
      $otp_key = strtoupper($key);
      if (in_array($otp_key, array('SUCCESS_DIR', 'FAILED_DIR'))) {
        $otp_key = 'transaction_log_dir.' . $otp_key;
      }
      $otp_key = 'otp.webshop.' . $otp_key;
      $drupal_config[$otp_key] = $value;
      unset($drupal_config[$key]);
    }
    $drupal_config['otp.webshop.PRIVATE_KEY_FILE_' . $pos_id] = $drupal_config['otp.webshop.PRIVATE_KEY_FILE'];
    unset($drupal_config['otp.webshop.PRIVATE_KEY_FILE']);
    $this->property = array_merge($this->property, $drupal_config);

    $this->soapClient = SoapUtils::createSoapClient($this->property);
  }

}
