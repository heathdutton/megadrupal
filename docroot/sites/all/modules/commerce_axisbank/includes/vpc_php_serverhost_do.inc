<?php
/**
 * @file
 */

/**
 * function for send all data to gateway.
 */
function page_send_response_data() {
  // define Constants
  // this secret will vary from merchant to merchant
  $SECURE_SECRET = $_POST["secure_secret"];
  // add the start of the vpcURL querystring parameters
  $vpcurl = $_POST["virtualPaymentClientURL"] . "?";
  // remove the Virtual Payment Client URL from the parameter hash as we
  // do not want to send these fields to the Virtual Payment Client.
  unset($_POST["virtualPaymentClientURL"]);
  unset($_POST["SubButL"]);
  $md5HashData = $SECURE_SECRET;
  ksort($_POST);
  // set a parameter to show the first pair in the URL.
  $appendamp = 0;
  foreach($_POST as $key => $value) {
    // create the md5 input and URL leaving out any fields that have no value.
    if (strlen($value) > 0) {
    // this ensures the first paramter of the URL is preceded by the '?' char.
      if ($appendamp == 0) {
        $vpcurl .= urlencode($key) . '=' . urlencode($value);
        $appendamp = 1;
        }
      else {
        $vpcurl .= '&' . urlencode($key) . "=" . urlencode($value);
      }
        $md5HashData .= $value;
    }
  }
  // create the secure hash and append it to the Virtual Payment Client Data if
  // the merchant secret has been provided.
  if (strlen($SECURE_SECRET) > 0) {
    $vpcurl .= "&vpc_SecureHash=" . strtoupper(md5($md5HashData));
  }
  // redirect the customers using the Digital Order.
  drupal_goto($vpcurl);
}
