<?php

/**
 * @file
 * Wrapper for CTools exportable.
 */

namespace Drupal\maps_import\Plugins\CTools\ExportUI;

/**
 * Base Exportable class.
 */
abstract class Exportable {

  /**
   * Class constructor.
   */
  public function __construct($object = NULL, $type = EXPORT_IN_DATABASE) {
    $this->setWrappedObject($object);
    $this->export_type = $type;
    $this->disabled = FALSE;

    switch ($this->export_type) {
      case EXPORT_IN_DATABASE:
        $this->type = t('Normal');
        break;
      case EXPORT_IN_CODE:
        $this->type = t('Default'); // @todo manage t('Overridden')
        break;
      default:
        $this->type = t('Local');
        break;
    }
  }

  /**
   * Get the wrapped object.
   *
   * @return mixed
   */
  abstract public function getWrappedObject();

  /**
   * Set the wrapped object.
   *
   * @param mixed $object
   *   If the passed object is not set, the child class should instanciate
   *   an empty object of the class it wraps.
   */
  abstract public function setWrappedObject($object = NULL);

  /**
   * Export the current object like var_export() does.
   *
   * This method provides a code that can be evaluated with the
   * eval() function (see self::__set_state() method).
   *
   * @see features_var_export()
   *
   * @return string
   */
  abstract public function export($prefix = '');

  /**
   * Magic method __call.
   */
  public function __call($name, $arguments) {
    if (is_callable(array($this->getWrappedObject(), $name))) {
      return call_user_func_array(array($this->getWrappedObject(), $name), $arguments);
    }
  }

  /**
   * Magic method __get.
   */
  public function __get($name) {
    $method = maps_suite_drupal2camelcase($name);
    $methods = array(
      'get' . $method,
      'is' . $method,
      'has' . $method,
    );

    foreach ($methods as $method) {
      if (is_callable(array($this->getWrappedObject(), $method))) {
        return $this->getWrappedObject()->{$method}();
      }
    }

    foreach ($methods as $method) {
      if (is_callable(array($this, $method))) {
        return $this->{$method}();
      }
    }

    if (isset($this->{$name})) {
      return $this->{$name};
    }
  }

  /**
   * Magic method __set.
   */
  public function __set($name, $value) {
    $setter = 'set' . maps_suite_drupal2camelcase($name);

    if (is_callable(array($this->getWrappedObject(), $setter))) {
      $this->getWrappedObject()->{$setter}($value);
    }
    else {
      $this->{$name} = $value;
    }
  }

  /**
   * Magic method __isset.
   */
  public function __isset($name) {
    return NULL !== $this->__get($name);
  }

  /**
   * Magic method __unset.
   */
  public function __unset($name) {
    $setter = 'set' . maps_suite_drupal2camelcase($name);

    if (is_callable(array($this->getWrappedObject(), $setter))) {
      $this->getWrappedObject()->{$setter}(NULL);
    }
    elseif (isset($this->{$name})) {
      unset($this->{$name});
    }
  }

  /**
   * Export data using schema fields as reference.
   *
   * @todo define an abstract method "toArray" (see ConverterExportable and
   * Profile) and use it to generate the code.
   *
   * @return array
   */
  protected function exportFromSchema($table, $indent = '', Exportable $object = NULL) {
    $output = array();

    if ($schema = drupal_get_schema($table)) {
      if (!isset($object)) {
        $object = $this;
      }

      foreach ($schema['fields'] as $field => $info) {
        if (empty($info['no export'])) {
          $output[] = $indent . "'$field' => " . ctools_var_export($object->{$field}, $indent . '  ') . ',';
        }
      }
    }

    return $output;
  }

}
