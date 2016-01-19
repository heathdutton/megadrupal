<?php
/**
 * @file
 * The base class from which to extend all other classes.  Contains error
 * handling procedures and environment variables for the Lyris integration.
 */
class Base {
  // Logging messages, Drupal style.
  protected $messages = array();

  // A standard Drupal message value for each Watchdog code
  protected $message_code = array(WATCHDOG_EMERGENCY => 'error',
                                  WATCHDOG_ALERT => 'error',
                                  WATCHDOG_CRITICAL => 'error',
                                  WATCHDOG_ERROR => 'error',
                                  WATCHDOG_WARNING => 'warning',
                                  WATCHDOG_NOTICE => 'status',
                                  WATCHDOG_INFO => 'status',
                                  WATCHDOG_DEBUG => 'debug');

  // The severity threshhold to report to watchdog.
  private $watchdog_limit = WATCHDOG_CRITICAL;

  /**
   * Constructor
   *
   * @param $params
   *   An array of parameters to instantiate the object.
   */
  protected function __construct() {
    lyris_include('soap');
  }

  /**
   * Remove any data from the variable before it's printed to prevent memory death.
   */
  protected function cleanDump(&$var) {
    /*if (meth($var->LyrisConn)) {
      $var->LyrisConn->hideAPI();
    }*/

    $var->mlapi = '*** hidden ***';
    $var->apiClient = '*** hidden ***';
  }

  /**
   * Dump the contents of this object.
   */
  public function dump($func = 'kpr') {
    // Clone the object so we don't accidentally screw up the real one.
    $not_this = clone($this);

    // Don't, for the love of Dries, print the API object. Holy memory meltdown.
    // Clean out any API objects because they are ridiculously large.
    $this->cleanDump($not_this);
    $not_this->mlapi = '*** hidden ***';

    // Optionally use a devel function to pring the var.
    if (function_exists($func)) {
      //kpr($not_this);   // Krumo alone
      //dsm($not_this);   // Krumo in theme
      //dpr($not_this);   // Blank screen
      $func($not_this);
    }
    else {
      print '<pre>' . print_r($not_this, 1) . '</pre>';
    }
  }

  /**
   * Handle thrown exceptions.
   *
   * Messages sent to the screen should be for end users only.  Anything
   * returned from Lyris should be logged only to watchdog and the function
   * should return the proper value so messages can be set by the requesting
   * method.
   */
  protected function handleException($e) {
    // Post the message if the user has access.  Otherwise show a generic error
    // message so we can mask detailed errors from users who won't understand it
    // anyway.
    if (user_access('show lyris exceptions')) {
      $this->setMessage('WATCHDOG: ' . $e->getMessage(), $this->message_code[$e->getCode()]);
    }
    else {
      //$this->setMessage(t('There was an error processing the last request.  Please contact an administrator.'), $this->message_code[$e->getCode()]);
    }

    // Limit watchdog reporting to certain severities.  The lower the severity
    // number, the higher the severity. (IE. 0 => EMERGENCY, 1 => ALERT, 5 => NOTICE)
    if ($e->getCode() <= $this->watchdog_limit) {
      $this->watchdog($e->getMessage(), $e->getCode());
    }
    elseif (!empty($e->watchdog_message)) {
      $this->watchdog($e->watchdog_message, $e->getCode(), $e->getArgs());
    }

    // If Devel is set to backtrace, print the exception trace.
    if (lyris_show_devel() && variable_get('lyris_devel_error_handler', 1) == 1 && $e->getCode() <= $this->watchdog_limit) {
      $trace = $e->getTrace();
      $count = count($trace);
      foreach ($trace as $i => $call) {
        $key = ($count - $i) . ': ' . $call['function'];
        $rich_trace[$key] = $call;
      }
      /*if (has_krumo()) {
        print krumo($rich_trace);
      }
      else {
        dprint_r($rich_trace);
      }*/
      $this->setMessage(kprint_r($rich_trace, TRUE), 'error');
      //$this->setMessage(kprint_r($this->apiClient->debug_str, TRUE), 'debug');
    }
  }

  /**
   * Purge the message queue.
   *
   * @param $type
   *   Specify the type of message to purge (i.e. 'error', 'warning', 'status').
   *   If no type is specified all messages will be purged.
   */
  public function purgeMessages($type = NULL) {
    if ($type) {
      $this->messages[$type] = array();
    }
    else {
      $this->messages = array();
    }
  }

  /**
   * Set params by running them through optional validators.
   *
   * @param $params
   *   An array of attribute name:value pairs to be stored into the object. Each
   *   attribute will be validated if a method exists matching the attribute
   *   name.
   */
  protected function setAttributes($params) {
    $params = (array)$params;

    foreach ($params as $name => $value) {
      // Define the validating method
      $method = 'set' . ucwords(strtolower($name));
      if (method_exists($this, $method)) {
        $this->$method($value);
      }
      else {
        $this->$name = $value;
      }
    }
  }

  /**
   * Set a Drupal message.
   *
   * @param $msg
   *   The translated message for Drupal to print.
   *
   * @param $type
   *   The Drupal message type.  Debug messages are handled differently for
   *   site admins and are only printed if debug mode is enabled through
   *   the UI.
   */
  public function setMessage($msg, $type = 'status') {
    if ($type != 'debug') {
      drupal_set_message($msg, $type, FALSE);
    }
    else {
      if (module_exists('devel')) {
        dpm($msg);
      }
      else {
        drupal_set_message($msg, 'debug');
      }
    }
  }

  /**
   * Determine whether the logged-in user has administrative access.
   */
  public function userIsDrupalAdmin($reset = FALSE) {
    static $userIsAdmin;

    if (!isset($userIsAdmin) || $reset) {
      $userIsAdmin = user_access('access lyris api');
    }

    return $userIsAdmin;
  }

  /**
   * Log a watchdog message.
   */
  public function watchdog($message, $severity = WATCHDOG_NOTICE, $args = array(), $link = NULL, $type = 'lyris') {
    watchdog($type, $message, $args, $severity, $link);
  }
}
