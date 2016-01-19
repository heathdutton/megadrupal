<?php
/**
 * @file
 * An A12 Connector exception class that handles all messages that can come back
 * from the service.
 */

class A12ConnectorException extends Exception {

  /**
   * Constructor. We make the message non optional.
   */
  public function __construct($code, Exception $previous = NULL) {
    $message = $this->createMessage($code);
    parent::__construct($message, (int) $code, $previous);
  }

  /**
   * Converts and error code to a human readable message.
   *
   * @param string $code
   *   The error code that we want to convert.
   */
  protected function createMessage($code) {
    $map = array(
      "FND-600" => "Connection error. Please contact your system administrator.",
      "FND-601" => "Invalid credentials. Please confirm your connection details and retry.",
      "FND-602" => "Subscription error. Please ensure your Subscription is active by logging into your account at <a href=\"http://www.axistwelve.com/user\">www.axistwelve.com</a>.",
      "FND-603" => "Access Key error. Please ensure your Access Key is enabled and has permissions to access your selected index.",
      "FND-604" => "Index error. Please ensure you have selected a valid Index for your content. If this problem persists please contact a site administrator.",
      "FND-605" => "There was an issue connecting to the index. Please confirm your account details are correct and you have selected a valid index. If this problem persists please contact a site administrator.",
      "FND-620" => "Your Subscription is offline. You can bring your Subscription back online by logging into your account at <a href=\"http://www.axistwelve.com/user\">www.axistwelve.com</a>.",
      "FND-623" => "Your Subscription has been cancelled. Please contact your system administrator to renew your subscription.",
      "FND-625" => "Your Subscription is currently blocked. Please contact your system administrator.",
      "FND-626" => "Your Subscription has expired. You can upgrade your Subscription by logging into your account at <a href=\"http://www.axistwelve.com/user\">www.axistwelve.com</a>.",
      "FND-630" => "You IP/hostname is not authorised to perform indexing operations for this account. To configure access permissions for this index please log into your account at <a href=\"http://www.axistwelve.com/user\">www.axistwelve.com</a>.",
    );

    if (isset($map[$code])) {
      return $map[$code];
    }
    else {
      return "Unknown error. Please contact your system administrator [$code].";
    }
  }
}
