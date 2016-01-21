<?php

/**
 * @file
 * Provides the Manticore PHP API
 *
 * @package Manticore
 * @license GPL (http://www.opensource.org/licenses/gpl-2.0.php)
 * @author Rob Loach
 *
 * @example
 * @code
 *   // Initialize the Manticore API.
 *   $key = 'Your MTC_KEY Here!';
 *   $group = 'Your MTC_GROUP Here!';
 *   $id = 'Your MTC_ID Here!';
 *   $manticore = new ManticoreCurl($key, $group, $id);
 *
 *   // Add a contact.
 *   $contact = array(
 *     'FirstName' => 'John',
 *     'LastName' => 'Smith',
 *     'Email' => 'john@smith.com',
 *   );
 *   $manticore->call($contact);
 * @endcode
 * @endexample
 */


/**
 * The Manticore PHP API.
 */
abstract class Manticore {
  public $key = NULL;
  public $group = NULL;
  public $id = NULL;
  public $server = 'stats.manticoretechnology.com';

  function __construct($key, $group, $id) {
    $this->key = $key;
    $this->group = $group;
    $this->id = $id;
  }

  /**
   * Call the Manticore API.
   *
   * This is an abstract function as it's suppose to be overriden using some
   * different method of calling, like cURL.
   */
  abstract public function call(array $arguments);

  /**
   * MTC_KEY does not match or is missing from the page.
   */
  const ErrorKey = 1000;

  /**
   * EmailAddress field missing or empty.
   */
  const ErrorEmail = 200;

  /**
   * Error creating or updating Manticore Contact.
   */
  const ErrorGeneric = 300;

  /**
   * Error creating or updating Manticore Contact.
   */
  const ErrorGeneric2 = 310;

  /**
   * Error updating SalesForce.
   */
  const ErrorSalesForce= 600;

  /**
   * Error adding contact to a list.
   */
  const ErrorList = 400;

  /**
   * Error sending contact into a Demand Booster Process.
   */
  const ErrorDemand = 500;

  /**
   * Error setting cookie.
   */
  const ErrorCookie = 700;

  /**
   * Successful submission, no redirect page specified.
   */
  const ErrorNone = 100;

  /**
   * Retrieves a description of the error given.
   *
   * @param $error
   *   The error code given.
   */
  public static function getErrorString($error) {
    switch ($data) {
      case Manticore::ErrorKey:
        return 'MTC_KEY does not match or is missing from the page.';
      case Manticore::ErrorEmail:
        return 'EmailAddress field missing or empty.';
      case Manticore::ErrorGeneric:
        return 'Error creating or updating Manticore Contact.';
      case Manticore::ErrorGeneric2:
        return 'Error creating or updating Manticore Contact.';
      case Manticore::ErrorSalesForce:
        return 'Error updating SalesForce.';
      case Manticore::ErrorList:
        return 'Error adding contact to a list.';
      case Manticore::ErrorDemand:
        return 'Error sending contact into a Demand Booster Process.';
      case Manticore::ErrorCookie:
        return 'Error setting the cookie.';
      case Manticore::ErrorNone:
        return 'Successful submission, no redirect page specified.';
      default:
        return 'An unknown error occurred.';
    }
  }
}

/**
 * The Manticore PHP API, using cURL as the request platform.
 */
class ManticoreCurl extends Manticore {
  public function call(array $arguments) {
    // Construct the arguments.
    $args = '';
    if (!isset($arguments['MTC_KEY'])) {
      $arguments['MTC_KEY'] = $this->key;
    }
    if (!isset($arguments['MTC_GROUP'])) {
      $arguments['MTC_GROUP'] = $this->group;
    }
    if (!isset($arguments['MTC_ID'])) {
      $arguments['MTC_ID'] = $this->id;
    }
    foreach ($arguments as $argument => $value) {
      if (!empty($value)) {
        $args .= $argument .'='. urlencode($value) .'&';
      }
    }
    $url = "http://{$this->server}/Data/{$arguments['MTC_GROUP']}/{$arguments['MTC_ID']}/{$arguments['MTC_KEY']}/mtcContactReg.aspx";

    // Use cURL to call the Manticore API.
    $ch = curl_init();
    $post = FALSE; // Change to TRUE to have a POST request.
    if ($post) {
      curl_setopt($ch, CURLOPT_URL, $url);
      curl_setopt($ch, CURLOPT_POSTFIELDS, $args);
      curl_setopt($ch, CURLOPT_POST, 1);
    }
    else {
      curl_setopt($ch, CURLOPT_URL, $url . '?' . $args);
    }
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_TIMEOUT, 5);
    $data = curl_exec($ch);
    $info = curl_getinfo($ch);
    curl_close($ch);

    // Check the result.
    if ($info['http_code'] == 200) {
      if ($data == Manticore::ErrorNone) {
        return $data;
      }
      else {
        throw new ManticoreException(Manticore::getErrorString($data), $data, $info, $data);
      }
    }
    else {
      throw new ManticoreException('Error contacting Manticore.', $info['http_code'], $info, $data);
    }
  }
}

/**
 * Errors caused by the Manticore API will throw a ManticoreException.
 */
class ManticoreException extends Exception {
  /**
   * The information returned from the cURL call.
   */
  public $info = NULL;

  /**
   * The information returned from the Manticore call.
   */
  public $data = NULL;

  /**
   * The Manticore code that was given.
   */
  public $code = 0;

  /**
   * Creates a ManticoreException.
   * @param $message
   *   The message for the exception.
   * @param $code
   *   (optional) The error code.
   * @param $info
   *   (optional) The result from the cURL call.
   */
  public function __construct($message, $code = 0, $info = NULL, $data = NULL) {
    $this->info = $info;
    $this->data = $data;
    $this->code = $code;
    parent::__construct($message, $code);
  }

  /**
   * Converts the exception to a string.
   */
  public function __toString() {
    $message = $this->getMessage();
    return __CLASS__ .": [{$this->code}]: $message\n";
  }
}
