<?php
/**
 * Original author: Leighton Whiting - Released under GENERAL PUBLIC LICENSE
 * Current maintenance by multiple MoneySuite users.
 * Re: new initiative: https://www.drupal.org/node/2315653 
 */

class ms_paypal_wps_class {
  var $lastError;
  var $ipnResult;
  var $ipn = array();
  var $values = array();

  function ms_paypal_wps_class($ipn_vars = NULL) {
    $this->ipnLink = (isset($ipn_vars['test_ipn']) AND $ipn_vars['test_ipn']) ? 'https://www.sandbox.paypal.com/cgi-bin/webscr' : 'https://www.paypal.com/cgi-bin/webscr';
    $this->ipn_vars = $ipn_vars;
    $this->lastError = '';
    $this->ipnResult = '';
  }

  function add($field, $value) {
    $this->values["$field"] = $value;
  }

  function submit($button_value) {
    $form = "<form id='paypalPaymentForm' target='_self' method='post' name='paypal_form' action='". $this->ipnLink ."'>";
    foreach ($this->values as $name => $value) {
      $form .= "<input type='hidden' name='$name' value='$value' />";
    }
    $form .= "<input type='submit' value='$button_value'></form>";
    return $form;
  }

  function verify() {
    $vars = $this->ipn_vars;
    $this->ipn = $this->ipn_vars;
    $vars['cmd'] = '_notify-validate';
    $options = array(
      'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
      'method' => 'POST',
      'data' => drupal_http_build_query($vars),
    );
    $result = drupal_http_request($this->ipnLink, $options);

    $this->ipnResult = $result->data;
    if (!empty($result->error)) {
      $this->lastError = t('IPN Validation Error: @error', array('@error' => $result->error));
      return FALSE;
    }
    else {
      if ($result->code == 200) {
        if ($result->data == 'VERIFIED') {
          return TRUE;
        }
        else {
          $this->lastError = t('IPN Validation Failed: @error', array('@error' => $result->data));
          return FALSE;
        }
      }
      else {
        // The server might be down, let's log an error but still pass the
        // validation.
        ms_core_log_error('ms_paypal_wps', 'The Validation Server had an error processing a request. Request: !request Response: !response',
          array('!request' => ms_core_print_r($options), '!response' => ms_core_print_r($result)), WATCHDOG_CRITICAL);
        return TRUE;
      }
    }
  }
}
