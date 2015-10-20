<?php
/**
 * @file
 * PaypalIpnListener class.
 */

class PaypalIpnListener extends IpnListener {
  private $verified = NULL;
  private $posted_data = array();

  /**
   * Override constructor to set public var from settings.
   */
  function __construct() {
    $this->use_curl = variable_get('paypal_ipn_use_curl', PAYPAL_IPN_USE_CURL);
    $this->follow_locatio = variable_get('paypal_ipn_follow_location', PAYPAL_IPN_FOLLOW_LOCATION);
    $this->use_ssl = variable_get('paypal_ipn_use_ssl', PAYPAL_IPN_USE_SSL);
    $this->use_sandbox = variable_get('paypal_ipn_use_sandbox', PAYPAL_IPN_USE_SANDBOX);
  }

  public function processIpn($post_data = NULL) {
    try {
      $this->verified =  parent::processIpn($post_data);
      // Capture posted data at process time.
      $this->posted_data = $_POST;
      if ($this->verified) {
        $this->verified = $this->isValidRequest();
      }
    }
    catch (Exception $e) {
      // fatal error trying to process IPN.
      watchdog_exception('paypal_ipn', $e);
      $this->verified =  FALSE;
    }
    return $this->verified;
  }

  /**
   * @return null
   */
  public function isVerified() {
    return $this->verified;
  }
  public function getPostedVar($var_name) {
    return isset($this->posted_data[$var_name])?
      $this->posted_data[$var_name]:
      NULL;
  }
  public function isCompleted() {
    return $this->getPostedVar('payment_status') == 'Completed';
  }

  /**
   * Determine if request is valid.
   * @return bool
   */
  protected function isValidRequest() {
    $vars = _paypal_ipn_get_confirm_vars();
    foreach ($vars as $var_name => $value) {
      $posted_var = $this->getPostedVar($var_name);
      if ($posted_var != $value) {
        watchdog(
          'paypal_ipn',
          'Incorrect value for @var_name: @value',
          array(
            '@var_name' => $var_name,
            '@value' => $posted_var,
          ),
          WATCHDOG_ERROR
        );
        return FALSE;
      }
    }
    // @todo Add hook_paypal_ipn_validate?
    return TRUE;
  }


}
