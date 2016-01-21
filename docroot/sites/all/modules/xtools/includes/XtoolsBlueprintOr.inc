<?php

class XtoolsBlueprintOr implements XtoolsBlueprintInterface {

  /**
   * An array of XtoolsBlueprintInterface objects, of which the value should
   * match one.
   *
   * @var array
   */
  public $allowed_blueprints = array();

  function __construct(array $allowed_blueprints) {
    $this->allowed_blueprints = $allowed_blueprints;
  }

  /**
   * Implements XtoolsBlueprint::getLabel().
   */
  function getLabel() {
    $types = array();
    foreach ($this->allowed_blueprints as $allowed_blueprint) {
      $types[] = $allowed_blueprint->getLabel();
    }

    return implode(', ', $types);
  }

  /**
   * Implements XtoolsBlueprint::validate().
   */
  function validate($data, array &$asserts, $path) {
    // Loop through all allowed blueprints.
    $return = FALSE;
    foreach ($this->allowed_blueprints as $allowed_blueprint) {
      $possible_assertions = array();
      if ($return = $allowed_blueprint->validate($data, $possible_assertions, $path)) {
        $asserts = array_merge($asserts, $possible_assertions);
        break;
      }
    }

    // No blueprint could validate the value.
    if (!((bool) $return)) {
      $asserts[] = new XtoolsAssert((bool) $return, "variable $path is of type " . $this->getLabel());
    }

    return $return;
  }
}