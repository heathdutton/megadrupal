<?php
/**
 * @file
 * Contains a Dumper.
 */

namespace Drupal\ld;

use Drupal\ld\Dumper\Ladybug;
use Ladybug\Format\JsonFormat;

/**
 * Class Dumper
 *
 * @package \Drupal\xd
 */
class Inspector {

  /**
   * ladybug
   *
   * @var \Drupal\ld\Dumper\Ladybug
   */
  public $ladybug;

  /**
   * Constructor.
   */
  public function __construct() {
    $this->ladybug = new Ladybug();

    // Build a list of class::method names which will be filtered from the
    // backtrace list.
    $class_name = get_class($this);
    $our_helpers = array(
      sprintf('%s:%s', $class_name, 'ladybugDump'),
      sprintf('%s:%s', $class_name, 'dump'),
      sprintf('%s:%s', $class_name, 'dpmFiltered'),
      sprintf('%s:%s', $class_name, 'dpm'),
      'ld_inspect',
      'ldpm',
    );

    // Setting the 'extra_helpers' option here, prior to instantiating our
    // Ladybug, will pass it to the internal Application at runtime.
    $this->ladybug->setOptions(array('extra_helpers' => $our_helpers));
  }

  /**
   * Lazy constructor.
   *
   * @return \Drupal\ld\Inspector
   *   This object.
   */
  static public function init() {

    return new static();
  }

  /**
   * Dump some output to Drupal's message area.
   *
   * @param mixed $input
   *   The input
   * @param string|null $name
   *   (Optional) An optional identifier.
   * @param string $type
   *   (Optional) The type of message to output. Default to 'status'.
   */
  public function dpm($input, $name = NULL, $type = 'status') {

    if (user_access('access devel information')) {

      $export = $this->dump($input, $name);
      drupal_set_message($export, $type, TRUE);
    }
  }

  /**
   * Dump with a filter, to Drupal's message area.
   *
   * This is useful for dumping information selectively. For example, to only
   * dump when inspecting an entity of a given type, you would do something
   * like:
   *
   * @code
   * ld_inspect()->dpmFilterEquals($entity, $entity_type, 'node');
   * @code
   *
   * @param mixed $input
   *   The values to be dumped.
   * @param mixed $value
   *   The value to test
   * @param mixed $condition
   *   The value to test equality to.
   *
   * @return mixed
   *   The result, or nothing.
   */
  public function dpmFilterEquals($input, $value, $condition) {
    if ($value == $condition) {
      $this->dpm($input, $value);
    }
  }

  /**
   * Return some dumped output.
   *
   * @param mixed $input
   *   The input
   * @param string|null $name
   *   (Optional) An optional identifier.
   *
   * @return string
   *   The dumped output.
   */
  public function dump($input, $name = NULL) {

    if (user_access('access devel information')) {
      return $this->insecureDump($input, $name);
    }
  }

  /**
   * Return dump information regardless of user permissions.
   *
   * @param mixed $input
   *   The input
   * @param string|null $name
   *   (Optional) An optional identifier.
   *
   * @return string
   *   The dumped output.
   */
  public function insecureDump($input, $name) {

    return (isset($name) ? $name . ":<br /> " : '') . $this->ladybugDump($input);
  }

  /**
   * Enable JSON output.
   *
   * @return Inspector
   *   This inspector.
   */
  public function enableJson() {
    $this->ladybug->setFormat(JsonFormat::FORMAT_NAME);

    return $this;
  }

  /**
   * Return dump information from Ladybug.
   *
   * @param string $input
   *   The thing to dump
   *
   * @return string
   *   Ladybugs output.
   */
  protected function ladybugDump($input) {

    return $this->ladybug->dump($input);
  }

  /**
   * Get the value for Ladybug.
   *
   * @return \Drupal\ld\Dumper\Ladybug
   *   The value of Ladybug.
   */
  public function getLadybug() {

    return $this->ladybug;
  }
}
