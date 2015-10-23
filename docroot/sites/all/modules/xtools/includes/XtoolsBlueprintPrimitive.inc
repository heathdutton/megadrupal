<?php

abstract class XtoolsBlueprintPrimitive implements XtoolsBlueprintInterface {

  /**
   * The gettype() type of this variable.
   *
   * @var string
   */
  public $type = '';

  /**
   * Implements XtoolsBlueprint::getLabel().
   */
  function getLabel() {
    return $this->type ? $this->type : '**UNDEFINED**';
  }

  /**
   * Implements XtoolsBlueprint::validate().
   */
  function validate($data, array &$asserts, $path) {
    if (!$path) {
      $path = '$' . $this->type;
    }
    if ($this->type) {
      $valid = gettype($data) == $this->type;
      $asserts[] = new XtoolsAssert($valid, "variable $path is of type " . $this->getLabel());
      if ($valid) {
        return $this;
      }
    }
    return FALSE;
  }
}