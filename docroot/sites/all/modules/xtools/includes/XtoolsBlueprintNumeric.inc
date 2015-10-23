<?php

abstract class XtoolsBlueprintNumeric extends XtoolsBlueprintPrimitive {

  /**
   * The maximum value. Use NULL for no maximum value.
   *
   * @var integer|float|null
   */
  public $maximum_value = NULL;

  /**
   * The minimum value. Use NULL for no minimum value.
   *
   * @var integer|float|null
   */
  public $minimum_value = NULL;

  /**
   * Constructor.
   *
   * @param integer|float|null $minimum_value
   *   See XtoolsBlueprintNumeric::minimum_value.
   * @param integer|float|null $maximum_value
   *   See XtoolsBlueprintNumeric::maximum_value.
   */
  function __construct($minimum_value = NULL, $maximum_value = NULL) {
    $this->maximum_value = $maximum_value;
    $this->minimum_value = $minimum_value;
  }

  /**
   * Implements XtoolsBlueprint::validate().
   */
  function validate($data, array &$asserts, $path) {
    if ($valid = (bool) parent::validate($data, $asserts, $path)) {
      if (!is_null($this->minimum_value)) {
        $asserts[] = new XtoolsAssert($data >= $this->minimum_value, "numeric $path has a minimum value of $this->minimum_value");
      }
      if (!is_null($this->maximum_value)) {
        $asserts[] = new XtoolsAssert($data <= $this->maximum_value, "numeric $path has a maximum value of $this->maximum_value");
      }
      return $valid;
    }
    return FALSE;
  }
}