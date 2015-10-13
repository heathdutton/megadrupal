<?php
/**
 * @file
 * Contains \Inspect.
 */


// Drupal 7's autoloader cannot include this file; presumably because the class
// names are the same (no namespace support).
// Furthermore simple '../' doesnt seem to work.
include_once __DIR__ . '/../vendor/simplecomplex/inspect/src/Inspect.php';

/**
 * Drupal 7 specialization of Inspect.
 */
class Inspect extends \SimpleComplex\Inspect\Inspect {

  /**
   * Default logging severity level.
   *
   * @var integer
   */
  public $severity = WATCHDOG_DEBUG;

  /**
   * Checks if user is allowed to output to that target.
   *
   *  Special rules:
   *  - filing uses same permissions as logging
   *  - drush/CLI is always allowed to get|log|file
   *  - target 'frontend log'
   *
   * @param string $target
   *   Values: log|file|frontend log|get.
   *   Default: get.
   *
   * @return boolean
   */
  public static function permit($target = 'get') {
    static $permit = array(
      'get' => NULL,
      'log' => NULL,
      'file' => NULL,
      'frontend log' => NULL,
    );

    if (($perm = $permit[$target]) || $perm === FALSE) {
      return $perm;
    }

    if (PHP_SAPI === 'cli') {
      $permit = array(
        'get' => TRUE,
        'log' => TRUE,
        'file' => TRUE,
        'frontend log' => FALSE, // Doesn't make sense.
      );
      return $permit[$target];
    }

    $perm = FALSE;
    switch ('' . $target) {
      case 'log':
      case 'file':
        if (user_access('inspect log')) {
          $perm = TRUE;
        }
        $permit['log'] = $perm;
        $permit['file'] = $perm;
        break;
      case 'frontend log':
        // A subset of 'log'.
        if (user_access('inspect log') && user_access('inspect frontend log')) {
          $perm = TRUE;
        }
        $permit['frontend log'] = $perm;
        break;
      case 'get':
        if (user_access('inspect get')) {
          $perm = TRUE;
        }
        $permit['get'] = $perm;
        break;
      default:
    }
    return $perm;
  }


  //  Utility methods.

  /**
   * Get maximum output length.
   *
   * Sets max. in 'inspect_output_max' if that variable is empty.
   *
   * @see Inspect::OUTPUT_DEFAULT
   * @see Inspect::OUTPUT_MAX
   *
   * @param boolean $check
   *   Truthy: attempts to read database max query length; fall-back
   *   Inspect::OUTPUT_DEFAULT.
   *   Default: FALSE (~ gets from variable setting (or database max query
   *   length) and subtracts some for safe margin).
   *
   * @return integer
   */
  public static function outputMax($check = FALSE) {
    $setting = static::configGet('inspect', 'output_max', 0);

    if ($setting && !$check) {
      // Subtract 5% for database abstraction query manipulation
      // (seen 1.5% difference).
      // Subtract 500 as metadata margin.
      return (int)floor($setting * 0.95) - 500;
    }

    try {
      if (!($le = (int) db_query('SELECT @@max_allowed_packet')->fetchField())) {
        $le = static::OUTPUT_DEFAULT;
      }
      elseif ($le > static::OUTPUT_MAX) {
        $le = static::OUTPUT_MAX;
      }
    }
    catch (\Exception $xc) { // Possible PDOException.
      static::logToStandard('Failed to read max query length of (MySQL type) database', 'inspect', WATCHDOG_WARNING);
      $le = static::OUTPUT_DEFAULT;
    }

    if (!$setting) {
      // If no previous setting, and the suggested setting is larger than
      // default: use default.
      static::configSet('inspect', 'output_max', $le > static::OUTPUT_DEFAULT ? static::OUTPUT_DEFAULT : $le);
    }

    if (!$check) {
      // Subtract 5% for database abstraction query manipulation
      // (seen 1.5% difference).
      // Subtract 500 as metadata margin.
      return (int)floor($le * 0.95) - 500;
    }

    return $le;
  }

  /**
   * Secures existance of filing directory; optionally subdir to that.
   *
   * Uses private://module/inspect.
   *
   * @param string $sub_dir
   *   Default: empty.
   *   No leading nor trailing slash.
   * @param integer $mode
   *   Default: 0775 (~ group write mode).
   *
   * @return string|boolean
   *   FALSE: on error.
   */
  public static function ensureDirectory($sub_dir = '', $mode = 0775) {
    static $_dir;
    if (!($dir = $_dir)) {
      if ($dir === NULL) {
        if (!($dir = static::configGet('', 'file_private_path'))) {
          static::logToStandard(
            'Private files directory not defined, check /admin/config/media/file-system', 'inspect', WATCHDOG_WARNING
          );
          return ($_dir = FALSE);
        }
      }
      else {
        return FALSE;
      }
      $le = strlen($dir);
      if ($dir{$le - 1} == '/') { // Remove trailing slash.
        $dir = substr($dir, 0, $le - 1); // Deliberately not multibyte substr().
        $le -= 1;
      }
      if ($le < 2) { // Private files dir cannot be shorter than 2 chars (/x).
        return ($_dir = FALSE);
      }
      if ($le < 2
        || (!is_dir($dir .= '/module/inspect') && !mkdir($dir, $mode, TRUE))
      ) {
        static::logToStandard(
          $le < 2 ? 'Private files directory cannot be shorter than 2 chars, check /admin/config/media/file-system' :
            'Directory private://module/inspect is not a dir or cannot be created',
          'inspect',
          WATCHDOG_WARNING
        );
        return ($_dir = FALSE);
      }
      $_dir = $dir;
    }
    if (!$sub_dir
      || is_dir($dir .= '/' . $sub_dir) || mkdir($dir, $mode, TRUE)
    ) {
      return $dir;
    }
    static::logToStandard(
      'Directory private://module/inspect/' . $sub_dir . ' is not a dir or cannot be created',
      'inspect',
      WATCHDOG_WARNING
    );
    return FALSE; // Failed to ensure subdir.
  }

  /**
   * Log to watchdog.
   *
   * Caller must guarantee to escape type and message.
   *
   * May truncate message to prevent failure.
   *
   * Uses the watchdog 'link' bucket for recording $code.
   *
   * @param string $message
   *   Default: empty string.
   * @param string $type
   *   Default: inspect.
   * @param integer|string $severity
   *   Default: 'debug'.
   * @param integer|string $code
   *   Default: zero.
   *
   * @return boolean
   */
  protected static function logToStandard($message = '', $type = 'inspect', $severity = 'debug', $code = 0) {
    // Truncate.
    // No instance output_max here, so we have to resort to the, possibly
    // larger, class value (log() and trace() truncates by instance output_max).
    // Deliberately not multibyte strlen().
    if (strlen($message) > static::$outputMax) {
      $message = static::truncateBytes($message, static::$outputMax - 5) . '[...]';
    }

    try {
      watchdog(
        $type,
        $message,
        array(),
        static::severity($severity),
        !$code ? NULL : $code
      );
      return TRUE;
    }
    catch (\Exception $xc) {
    }

    return FALSE;
  }

  /**
   * @param string $str
   *
   * @return boolean
   */
  protected static function validUtf8($str) {
    return drupal_validate_utf8($str);
  }

  /**
   * @param string $str
   *
   * @return string
   */
  protected static function plaintext($str) {
    return check_plain(strip_tags($str));
  }

  /**
   * Multibyte-safe string length.
   *
   * @param string $str
   *
   * @return integer
   */
  protected static function mb_strlen($str) {
    return drupal_strlen($str);
  }

  /**
   * Multibyte-safe sub string.
   *
   * @param string $str
   * @param integer $start
   * @param integer|NULL $length
   *   Default: NULL.
   *
   * @return string
   */
  protected static function mb_substr($str, $start, $length = NULL) {
    return drupal_substr($str, $start, $length);
  }

  /**
   * Truncate multibyte safe until ASCII length is equal to/less than arg
   * length.
   *
   * @param string $str
   * @param integer $length
   *   Fails if non-integer (like float or string) and PHP>=5.4.
   *
   * @return string
   */
  protected static function truncateBytes($str, $length) {
    return drupal_truncate_bytes($str, $length);
  }

  /**
   * @return string
   */
  protected static function docRoot() {
    return DRUPAL_ROOT;
  }

  /**
   * @param string $domain
   * @param string $name
   * @param mixed $default
   *   Default: NULL.
   *
   * @return mixed
   */
  protected static function configGet($domain, $name, $default = NULL) {
    return variable_get(($domain ? ($domain . '_') : '') . $name, $default);
  }

  /**
   * @param string $domain
   * @param string $name
   * @param mixed $value
   */
  protected static function configSet($domain = 'inspect', $name, $value) {
    variable_set(($domain ? ($domain . '_') : '') . $name, $value);
  }

  /**
   * @param string $name
   * @param mixed $value
   * @param integer $expire
   *   Default: zero (~ session).
   * @param boolean $httponly
   *   Default: FALSE.
   */
  protected static function cookieSet($name, $value, $expire = 0, $httponly = FALSE) {
    setcookie(
      $name, '' . $value, $expire,
      $GLOBALS['base_path'], $GLOBALS['cookie_domain'], $GLOBALS['is_https'], $httponly
    );
  }

  /**
   * @return integer|string
   */
  protected static function userId() {
    return $GLOBALS['user']->uid;
  }

}
