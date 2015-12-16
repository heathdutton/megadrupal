<?php

/**
 * Describes a variable that can only have one of any allowed values.
 */
class XtoolsBlueprintOrValue implements XtoolsBlueprintInterface {

  /**
   * An array of values (scalars or NULL), one of which the data should be
   * identical to.
   *
   * @var array
   */
  public $allowed_values = array();

  /**
   * Constructor.
   *
   * @param array $allowed_values
   *   An array of values, one of which the variable should be identical to.
   */
  function __construct(array $allowed_values) {
    $this->allowed_values = $allowed_values;
  }

  /**
   * Get a human-readable label for a specific allowed value.
   *
   * @param int $delta
   *   The key of the allowed value in $this->allowed_values.
   */
  function getAllowedValueLabel($delta) {
    return '<code>' . var_export($this->allowed_values[$delta], TRUE) . '</code>';
  }

  /**
   * Implements XtoolsBlueprint::getLabel().
   */
  function getLabel() {
    $labels = array();
    foreach (array_keys($this->allowed_values) as $delta) {
      $labels[] = $this->getAllowedValueLabel($delta);
    }

    return implode(', ', $labels);
  }

  /**
   * Implements XtoolsBlueprint::validate().
   */
  function validate($data, array &$asserts, $path) {
    // Preprocess allowed values for the assert messages.
    $allowed_values_message = array();
    foreach ($this->allowed_values as $allowed_value) {
      $allowed_values_message[] = '<code>' . $allowed_value . '</code> (' . gettype($allowed_value) . ')';
    }

    // Loop through all allowed values.
    foreach ($this->allowed_values as $i => $allowed_value) {
      $possible_asserts = array();
      if ($data === $allowed_value) {
        $asserts[] = new XtoolsAssert(TRUE, "variable $path is identical to $allowed_values_message[$i]");
        return $this;
      }
    }

    // Validation failed.
    $asserts[] = new XtoolsAssert(FALSE, "variable $path is identical to one of " . $this->getLabel());
    return FALSE;
  }
}