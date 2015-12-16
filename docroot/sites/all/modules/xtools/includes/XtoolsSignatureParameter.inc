<?php

class XtoolsSignatureParameter {

  /**
   * The parameter definition.
   *
   * @var array
   *
   * An array with the following elements, all of which are optional:
   * - default_value (mixed): The parameter's default value.
   * - name (string): The parameter's default value.
   * - reference (boolean): Whether the parameter should be passed on by
   *   reference.
   * - type (string): The parameter's type hint. Can be "array" or a class
   *   name.
   */
  public $definition = array();

  /**
   * Constructor.
   *
   * @param array $definition
   *   See $this->definition for details.
   */
  function __construct(array $definition = array()) {
    $this->definition = $definition;
  }

  /**
   * Validate a parameters's definition.
   *
   * @param ReflectionParameter $parameter
   * @param array $asserts
   *   An array in which to store XtoolsAssert objects that describe the
   *   validation result.
   *
   * @return false|XtoolsSignatureParameter
   *   An XtoolsSignatureParameter object (usually $this) if the callable is
   *   valid. FALSE if it isn't.
   */
  function validate(ReflectionParameter $parameter, array &$asserts = array()) {
    $valid = TRUE;

    // Validate passing by reference.
    if (isset($this->definition['reference'])) {
      if (!($valid_reference = $this->definition['reference'] == $parameter->isPassedByReference())) {
        $valid = FALSE;
      }
      $is = $this->definition['reference'] ? 'is' : 'is not';
      $asserts[] = new XtoolsAssert($valid_reference, "Parameter {$parameter->getposition()} $is passed on by reference");
    }

    // Validate the parameter's name.
    if (isset($this->definition['name'])) {
      if (!($valid_name = $parameter->getName() == $this->definition['name'])) {
        $valid = FALSE;
      }
      $asserts[] = new XtoolsAssert($valid_name, "Parameter {$parameter->getposition()} has name <code>{$this->definition['name']}</code>");
    }

    // Validate the default value.
    if (array_key_exists('default_value', $this->definition)) {
      $valid_default_value = $parameter->isOptional() && $parameter->getDefaultValue() === $this->definition['default_value'];
      if (!$valid_default_value) {
        $valid = FALSE;
      }
      $default_value_message = var_export($this->definition['default_value'], TRUE);
      $asserts[] = new XtoolsAssert($valid_default_value, "Parameter {$parameter->getposition()} has default value <code>$default_value_message</code>");
    }

    // Validate the parameter's type.
    if (isset($this->definition['type'])) {
      $valid_type = FALSE;

      // The parameter should be an array.
      if ($this->definition['type'] == 'array') {
        $valid_type = $parameter->isArray();
      }
      // The parameter should be an object.
      else {
        $valid_type = FALSE;
        if ($class = $parameter->getClass()) {
          $valid_type = $class->name == $this->definition['type'];
        }
      }

      if (!$valid_type) {
        $valid = FALSE;
      }
      $asserts[] = new XtoolsAssert($valid_type, "Parameter {$parameter->getposition()} has a hint of type <code>{$this->definition['type']}</code>");
    }

    return $valid ? $this : FALSE;
  }
}