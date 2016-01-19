<?php
/**
 * @file
 * Universal Data Object
 *
 * Store variable name-value pairs for a Tealium Universal Data Object.
 */

namespace Drupal\tealium;

/**
 * Class UniversalDataObject
 */
class UniversalDataObject implements UniversalDataObjectInterface {

  private $allDataSourceValues = array();

  public function __construct($variables = array()) {
    $this->setAllDataSourceValues($variables);
  }

  /**
   * @param array|\stdClass $dataVariables
   * @return $this
   */
  public function setAllDataSourceValues($dataVariables) {
    // @todo: throw Invalid Argument Exception if parameters not correct type
    if ($dataVariables instanceof \stdClass) {
      $properties = get_object_vars($dataVariables);
      foreach ($properties as $name => $value) {
        $this->setDataSourceValue($name, $value);
      }
    }
    elseif (is_array($dataVariables)) {
      $this->allDataSourceValues = $dataVariables;
    }

    return $this;
  }

  /**
   * @return array
   */
  public function getAllDataSourceValues() {
    return $this->allDataSourceValues;
  }

  /**
   * @param string $name
   * @param mixed $value
   * @return null|string
   */
  public function setDataSourceValue($name, $value) {
    $name = strval($name);

    if (strlen($name) !== 0) {
      $this->allDataSourceValues[$name] = $value;
    }

    return $this;
  }

  /**
   * @param string $name
   * @return null|mixed
   *  The variable value if found else NULL.
   */
  public function getDataSourceValue($name) {
    // @todo: Throw Invalid Argument Exception if param not a string.
    $value = NULL;

    if (array_key_exists($name, $this->allDataSourceValues)) {
      $value = $this->allDataSourceValues[$name];
    }

    return $value;
  }

  /**
   * @param string $name
   * @return $this
   */
  public function unsetDataSourceValue($name) {

    if (array_key_exists($name, $this->allDataSourceValues)) {
      unset($this->allDataSourceValues[$name]);
    }

    return $this;
  }

}
