<?php

class XtoolsBlueprintChildElement implements XtoolsBlueprintInterface {

  /**
   * The blueprint this child should contain.
   *
   * @var XtoolsBlueprint
   */
  public $child_blueprint = NULL;

  /**
   * Whether this child element is required to be present.
   *
   * @var boolean
   */
  public $required = FALSE;

  function __construct(XtoolsBlueprintInterface $child_blueprint, $required = FALSE) {
    $this->child_blueprint = $child_blueprint;
    $this->required = $required;
  }

  /**
   * Implements XtoolsBlueprint::getLabel().
   */
  function getLabel() {
    return $this->child_blueprint->getLabel();
  }

  /**
   * Implements XtoolsBlueprint::validate().
   */
  function validate($data, array &$asserts, $path) {
    return $this->child_blueprint->validate($data, $asserts, $path);
  }
}