<?php

class XtoolsBlueprintString extends XtoolsBlueprintPrimitive {

  /**
   * The maximum number of characters this string should have. Use NULL for no
   * maximum value.
   *
   * @var integer|null
   */
  public $maximum_length = NULL;

  /**
   * The minimum number of characters this string should have. Use NULL for no
   * minimum value.
   *
   * @var integer|null
   */
  public $minimum_length = NULL;

  public $type = 'string';

  /**
   * Constructor.
   *
   * @param integer|null $minimum_length
   *   See XtoolsBlueprintString::minimum_length.
   * @param integer|null $maximum_length
   *   See XtoolsBlueprintString::maximum_length.
   */
  function __construct($minimum_length = NULL, $maximum_length = NULL) {
    $this->maximum_length = $maximum_length;
    $this->minimum_length = $minimum_length;
  }

  /**
   * Implements XtoolsBlueprint::validate().
   */
  function validate($data, array &$asserts, $path) {
    if ($valid = (bool) parent::validate($data, $asserts, $path)) {
      if (!is_null($this->minimum_length)) {
        $asserts[] = new XtoolsAssert(drupal_strlen($data) >= $this->minimum_length, "string $path has a minimum length of $this->minimum_length characters");
      }
      if (!is_null($this->maximum_length)) {
        $asserts[] = new XtoolsAssert(drupal_strlen($data) <= $this->maximum_length, "string $path has a maximum length of $this->maximum_length characters");
      }
      return $valid;
    }
    return FALSE;
  }
}