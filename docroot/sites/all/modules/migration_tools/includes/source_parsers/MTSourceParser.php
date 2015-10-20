<?php

/**
 * @file
 * Includes MTSourceParser class, which parses static HTML files via queryPath.
 */

// composer_manager is supposed to take care of including this library, but
// it doesn't seem to be working.
require_once DRUPAL_ROOT . '/sites/all/vendor/querypath/querypath/src/qp.php';

/**
 * Class MTSourceParser.
 *
 * @package migration_tools
 */
abstract class MTSourceParser {

  protected $obtainersInfo;
  protected $fileId;
  protected $html;

  public $queryPath;

  /**
   * A specific source parser class should set useful set of obtainers info.
   */
  abstract protected function setDefaultObtainersInfo();

  /**
   * Constructor.
   *
   * @param string $file_id
   *   The file id, e.g. careers/legal/pm7205.html
   * @param string $html
   *   The full HTML data as loaded from the file.
   */
  public function __construct($file_id, $html) {
    $this->fileId = $file_id;

    $html = StringCleanUp::fixEncoding($html);
    $html = StringCleanUp::stripWindowsCRChars($html);
    $html = StringCleanUp::fixWindowSpecificChars($html);
    $html = StringCleanUp::removePhp($html);
    $this->html = $html;

    $this->setDefaultObtainersInfo();
    $this->drushPrintSeparator();
  }

  /**
   * Add obtainer info for this source parser to use.
   */
  public function addObtainerInfo(ObtainerInfo $oi) {
    $this->obtainersInfo[$oi->getProperty()] = $oi;
  }

  /**
   * Get information/properties from html by running the obtainers.
   */
  protected function getProperty($property) {
    if (!isset($this->{$property})) {
      $this->setProperty($property);
    }

    // We can just return the property as any issue should throw an exception
    // form setProperty.
    return $this->{$property};
  }

  /**
   * Set a property.
   */
  protected function setProperty($property) {
    // Make sure our QueryPath object has been initialized.
    $this->initQueryPath();
    // Obtain the property using obtainers.
    $this->{$property} = $this->obtainProperty($property);
  }

  /**
   * Use the obtainers mechanism to extract text from the html.
   */
  protected function obtainProperty($property) {
    $text = '';

    $obtainer_info = $this->obtainersInfo[$property];
    if (!isset($obtainer_info)) {
      throw new Exception("MTSourceParser does not have obtainer info for the {$property} property");
    }

    try {
      $class = $obtainer_info->getClass();
      $methods = $obtainer_info->getMethods();
      if (!empty($methods)) {
        // There are methods to run, so run them.
        $obtainer = new $class($this->queryPath, $methods);
        MigrationMessage::makeMessage("Obtaining @key via @obtainer_class", array('@key' => $property, '@obtainer_class' => $class));

        $text = $obtainer->obtain();
        $length = strlen($text);
        if (!$length) {
          // It's too long to be helpful in output so just show the length.
          MigrationMessage::makeMessage('@property NOT found', array('@property' => $property), WATCHDOG_DEBUG, 2);
        }
        elseif ($length < 256) {
          // It is short enough to be helpful in debug output.
          MigrationMessage::makeMessage('@property found --> @text', array('@property' => $property, '@text' => $text), WATCHDOG_DEBUG, 2);
        }
        else {
          // It's too long to be helpful in output so just show the length.
          MigrationMessage::makeMessage('@property found --> Length: @length', array('@property' => $property, '@length' => $length), WATCHDOG_DEBUG, 2);
        }
      }
      else {
        // There were no methods to run so message.
        MigrationMessage::makeMessage("There were no methods to run for @key via @obtainer_class so it was not executed", array('@key' => $property, '@obtainer_class' => $class));
      }

    }
    catch (Exception $e) {
      MigrationMessage::makeMessage("@file_id Failed to set @key, Exception: @error_message", array(
        '@file_id' => $this->fileId,
        '@key' => $property,
        '@error_message' => $e->getMessage(),
      ), WATCHDOG_ERROR);
    }

    return $text;
  }

  /**
   * Create the queryPath object.
   */
  protected function initQueryPath() {
    // If query path is already initialized, get out.
    if (isset($this->queryPath)) {
      return;
    }

    $type_detect = array(
      'UTF-8',
      'ASCII',
      'ISO-8859-1',
      'ISO-8859-2',
      'ISO-8859-3',
      'ISO-8859-4',
      'ISO-8859-5',
      'ISO-8859-6',
      'ISO-8859-7',
      'ISO-8859-8',
      'ISO-8859-9',
      'ISO-8859-10',
      'ISO-8859-13',
      'ISO-8859-14',
      'ISO-8859-15',
      'ISO-8859-16',
      'Windows-1251',
      'Windows-1252',
      'Windows-1254',
    );
    $convert_from = mb_detect_encoding($this->html, $type_detect);
    if ($convert_from != 'UTF-8') {
      // This was not UTF-8 so report the anomaly.
      $message = "Converted from: @convert_from";
      MigrationMessage::makeMessage($message, array('@convert_from' => $convert_from), WATCHDOG_INFO, 1);
    }

    $qp_options = array(
      'convert_to_encoding' => 'UTF-8',
      'convert_from_encoding' => $convert_from,
    );

    // Create query path object.
    try {
      $this->queryPath = htmlqp($this->html, NULL, $qp_options);

    }
    catch (Exception $e) {
      MigrationMessage::makeMessage('Failed instantiate QueryPath for HTML, Exception: @error_message', array('@error_message' => $e->getMessage()), WATCHDOG_ERROR);
    }

    if (!is_object($this->queryPath)) {
      throw new Exception("{$this->fileId} failed to initialize QueryPath");
    }
  }

  /**
   * Prints a log message separator to drush.
   */
  protected function drushPrintSeparator() {
    if (drupal_is_cli() && variable_get('migration_tools_drush_debug', FALSE)) {
      drush_print(str_repeat('-', 40));
      MigrationMessage::makeMessage('@class: @file_id:', array('@class' => get_class($this), '@file_id' => $this->fileId), WATCHDOG_DEBUG, 0);
    }
  }

  /**
   * Logs a system message.
   *
   * @param string $message
   *   The message to store in the log. Keep $message translatable
   *   by not concatenating dynamic values into it! Variables in the
   *   message should be added by using placeholder strings alongside
   *   the variables argument to declare the value of the placeholders.
   *   See t() for documentation on how $message and $variables interact.
   * @param array $variables
   *   Array of variables to replace in the message on display or
   *   NULL if message is already translated or not possible to
   *   translate.
   * @param int $severity
   *   The severity of the message; one of the following values as defined in
   *   - WATCHDOG_EMERGENCY: Emergency, system is unusable.
   *   - WATCHDOG_ALERT: Alert, action must be taken immediately.
   *   - WATCHDOG_CRITICAL: Critical conditions.
   *   - WATCHDOG_ERROR: Error conditions.
   *   - WATCHDOG_WARNING: Warning conditions.
   *   - WATCHDOG_NOTICE: (default) Normal but significant conditions.
   *   - WATCHDOG_INFO: Informational messages.
   *   - WATCHDOG_DEBUG: Debug-level messages.
   *
   * @param int $indent
   *   (optional). Sets indentation for drush output. Defaults to 1.
   *
   * @link http://www.faqs.org/rfcs/rfc3164.html RFC 3164: @endlink
   */
  protected function sourceParserMessage($message, $variables = array(), $severity = WATCHDOG_NOTICE, $indent = 1) {
    $type = get_class($this);
    watchdog($type, $message, $variables, $severity);

    if (drupal_is_cli() && variable_get('migration_tools_drush_debug', FALSE)) {
      $formatted_message = format_string($message, $variables);
      drush_print($formatted_message, $indent);
      if ((variable_get('migration_tools_drush_stop_on_error', FALSE)) && ($severity <= WATCHDOG_ERROR)) {
        throw new MigrateException("$type: Stopped for debug.\n -- Run \"drush mi {migration being run}\" to try again. \n -- Run \"drush vset migration_tools_drush_stop_on_error FALSE\" to disable auto-stop.");
      }
    }
  }

  /**
   * Geocode a string.
   *
   * @param string $string
   *   A location string.
   *
   * @return array
   *   An array with location information extracted from the string.
   */
  public function geoCodeString($string) {

    // Geocode the location and parse into structured data for migration.
    // Geocoder module is not an explicit dependency because most migrations
    // do not rely on it. It should be disabled after use.
    if (!empty($string)) {
      if ($string == 'Washington, D.C.') {
        // The most common entry, so skip geocoding.
        $address['locality'] = 'Washington';
        $address['administrative_area_level_1'] = 'DC';
        $address['country'] = "US";
      }
      elseif (module_exists('geocoder')) {
        // Note that calling this too many times (as in very large migrations)
        // may exceed the API request limit for geocoder's source data.
        $point = geocoder('google', $string);
        module_load_include('inc', 'migration_tools', 'includes/migration_tools');
        try {
          $address = mt_migrate_convert_geocoded_point_to_address($point);
        }
        catch (Exception $e) {
          watchdog("migration_tools", "The geocoder failed: {$e->getMessage()}");
        }

        if (!$address) {
          $address['locality'] = '';
          $address['administrative_area_level_1'] = '';
          $address['country'] = '';
          $message = "@fileid Could not look up location because geocoder returned nothing. The API request limit may have been exceeded.";
          $variables = array('@fileid' => $this->fileId);
          MigrationMessage::makeMessage($address, $variables, WATCHDOG_INFO, 2);
        }
      }
      else {
        $message = "@fileid Could not look up location because geocoder module is not enabled";
        $variables = array('@fileid' => $this->fileId);
        MigrationMessage::makeMessage($address, $variables, WATCHDOG_INFO, 2);
      }

      return $address;
    }
  }

  /**
   * Set the html var after some cleaning.
   */
  protected function cleanHtml() {
    try {
      $this->initQueryPath();
      HtmlCleanUp::convertRelativeSrcsToAbsolute($this->queryPath, $this->fileId);
      HtmlCleanUp::removeFaultyImgLongdesc($this->queryPath);

      // Clean up specific to this site.
      HtmlCleanUp::stripOrFixLegacyElements($this->queryPath);
    }
    catch (Exception $e) {
      MigrationMessage::makeMessage('@file_id Failed to clean the html, Exception: @error_message', array('@file_id' => $this->fileId, '@error_message' => $e->getMessage()), WATCHDOG_ERROR);
    }
  }
}

/**
 * Information about which property we are dealing with.
 *
 * Including the class and methods to be called in that obtainer.
 */
class ObtainerInfo {
  private $property;
  private $class;
  private $methods = array();

  /**
   * Constructor.
   */
  public function __construct($property, $class = "") {
    $this->property = $property;

    $pieces = explode("_", $property);

    if (empty($class)) {
      $class = "";
      foreach ($pieces as $piece) {
        $class .= ucfirst($piece);
      }
      $class = "Obtain{$class}";
    }
    $this->setClass($class);
  }

  /**
   * Setter.
   */
  private function setClass($class) {
    // @todo Maybe we should validate the class here.
    $this->class = $class;
  }

  /**
   * Getter.
   */
  public function getProperty() {
    return $this->property;
  }

  /**
   * Getter.
   */
  public function getClass() {
    return $this->class;
  }

  /**
   * Add a new method to be called during obtainer processing.
   *
   * @param string $method_name
   *   The name of the method to call.
   *
   * @param array $arguments
   *   (optional) An array of arguments to be passed to the $method. Defaults
   *   to an empty array.
   */
  public function addMethod($method_name, $arguments = array()) {
    // @todo Maybe we should validate the method names here?
    $this->methods[] = array(
      'method_name' => $method_name,
      'arguments' => $arguments,
    );
  }

  /**
   * Getter.
   */
  public function getMethods() {
    return $this->methods;
  }
}
