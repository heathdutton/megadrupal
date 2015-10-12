<?php
/**
 * @file
 * Extend the basic exceptions classes.
 */
/**
 * Extend the exception class for Lyris support.
 */
class LyrisException extends Exception {
  // The lyris api object that threw the exception.
  protected $lyris;

  // The default error code.
  protected $code = WATCHDOG_ERROR;

  // An array of arguments to use in the message translation.
  private $args = array();

  // An optional prefix for messages.
  protected $msg_prefix = '';

  // A separate message for watchdog.  Overrides minimum exception reporting
  // level if set.
  public $watchdog_message;

  /**
   * Constructor
   *
   * @param $lyris
   *   The Lyris object throwing the exception.
   */
  public function LyrisException($lyris = NULL) {
    $ops = _lyris_options('lyris_mode');
    $this->lyris = $lyris;

    // Set some global arguments for messages to use.
    $this->setArg('%_file', $this->file);
    $this->setArg('%_line', $this->line);
    $this->setArg('%_lyris_mode', $ops[variable_get('lyris_mode')]);

    if (is_object($this->lyris)) {
      $this->setArg('@_mode', $ops[$this->lyris->mode]);
      $this->setArg('%_mode', $ops[$this->lyris->mode]);
      $this->setArg('@_version', $this->lyris->api_version);
      $this->setArg('%_version', $this->lyris->api_version);
      $this->setArg('!_readme', 'See the ' . l('README file', LYRIS_MOD_PATH . '/README.txt') . ' for more info.');
    }
  }

  /**
   * Return the Args array.
   */
  public function getArgs() {
    return $this->args;
  }

  /**
   * Set an argument for message translation.
   *
   * @param $ph
   *   The placeholder value for the translation string.
   *
   * @param $arg
   *   The argument value.
   */
  public function setArg($ph, $arg) {
    $this->args[$ph] = $arg;
  }

  /**
   * Set the error code.
   */
  protected function setCode($code) {
    $this->code = $code;
  }

  /**
   * Set an untranslated message.
   */
  public function setMessage($msg) {
    $this->addDebug($msg);

    // Prepend the prefix from the child Exception
    $this->setArg('@_prefix', $this->msg_prefix . ': ');
    $msg = "@_prefix$msg";

    $this->message = t($msg, $this->args);
  }

  /**
   * Set a separate message for watchdog.
   */
  protected function setWatchdogMessage($msg) {
    $this->addDebug($msg);

    // Prepend the prefix from the child Exception
    $this->setArg('@_prefix', $this->msg_prefix . ': ');
    $msg = "@_prefix$msg";

    $this->watchdog_message = $msg;
  }

  /**
   * Add debugging information to a message.
   */
  private function addDebug(&$msg) {
    // If Lyris Devel is enabled, add requested data to the message.
    if (lyris_show_devel()) {
      if (variable_get('lyris_devel_show_server')) {
        $msg .= ' [@_mode]';
      }
      if (variable_get('lyris_devel_show_line')) {
        $msg .= ' (Line %_line of %_file)';
      }
    }
  }
}

/**
 * General Lyris Exceptions
 */
class LyrisGeneralException extends LyrisException {
  /**
   * Constructor
   */
  public function LyrisGeneralException($lyris, $msg = '', $code = WATCHDOG_WARNING) {
    parent::__construct($lyris);

    $this->msg_prefix = 'Lyris';
    $this->setCode($code);
    $this->setMessage($msg);
  }
}

/**
 * Extends the Exceptions class to better report issues with Lyris Connections.
 */
class LyrisConnectionException extends LyrisException {
  /**
   * Constructor
   */
  public function LyrisConnectionException($lyris, $msg ='') {
    parent::__construct($lyris);

    $this->msg_prefix = 'Lyris Connection Error';

    // Add a string for the connection credentials:
    $lyris->watchdog($this->msg_prefix . ': ' . $lyris->getDebugCreds());

    $msg = (empty($msg) ? 'Unable to establish @_mode connection to Lyris.' : $msg);

    $this->setMessage($msg);
  }
}

/**
 * Extends exceptions for Lyris API calls.
 */
class LyrisApiException extends LyrisException {
  /**
   * Constructor
   */
  public function LyrisApiException($lyris, $msg, $code = WATCHDOG_ERROR) {
    parent::__construct($lyris);

    $this->msg_prefix = 'Lyris API Error';

    $this->setMessage($msg);
    $this->setCode($code);
  }
}

/**
 * Exceptions for API calls that have not been mapped for the current version.
 */
class LyrisMissingApiException extends LyrisException {
  /**
   * Constructor.
   *
   * @param $lyris
   *   The lyris API object.
   * @param $func
   *   The function reporting the exception.
   * @param $root
   *   Boolean to determine if the missing API call is an invalid Lyris API
   *   function or one we wrote that does not have an API version adapter.
   */
  public function LyrisMissingApiException($lyris, $func, $root = FALSE) {
    parent::__construct($lyris);

    // Set the message prefix.
    $this->msg_prefix = 'Missing API';

    $this->setArg('%f', $func);

    if ($root) {
      $this->setMessage('Invalid Lyris API call.  %f does not exist.');
    }
    elseif ($lyris->isConnected()) {
      $this->setMessage('The function %f has no adapter for Lyris API version @_version.');
    }
    else {
      $this->setMessage('Error calling API function %f.  Unable to connect to Lyris.');
    }

    $this->setCode(WATCHDOG_CRITICAL);
  }
}

/**
 * Exception handler for invalid struct arguments.
 */
class LyrisBadStructException extends LyrisException {
  /**
   * Constructor
   */
  public function LyrisBadStructException($class, $field) {
    parent::__construct();

    $this->setArg('%class', $class);
    $this->setArg('%field', $field);


    $this->msg_prefix = 'Bad Lyris Structure';

    $this->setMessage('Unable to sync with Lyris.');
    $this->setWatchdogMessage('Class %class attempted to set an invalid structure field %field.');
  }
}

/**
 * Exception handler for mode conflicts.
 */
class LyrisModeException extends LyrisException {
  /**
   * Constructor
   */
  public function LyrisModeException($lyris, $func = NULL, $msg = '', $code = WATCHDOG_WARNING) {
    parent::__construct($lyris);

    $this->msg_prefix = 'Lyris Mode';
    $this->setCode($code);

    if ($func) {
      $this->setArg('%_func', $func);
    }

    if ($msg) {
      $this->setMessage($msg);
    }
    else {
      $this->setArg('!ls', l('Offline Mode', 'admin/config/services/lyris', array('query' => drupal_get_destination())));
      $this->setMessage('Lyris is currently in !ls.  All lists, content or members will be stored locally then synced when Lyris is accessable.');
    }
  }
}
