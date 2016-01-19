<?php

class XtoolsBlueprintScalar implements XtoolsBlueprintInterface {

  /**
   * Implements XtoolsBlueprint::getLabel().
   */
  function getLabel() {
    return 'scalar';
  }

  /**
   * Implements XtoolsBlueprint::validate().
   */
  function validate($data, array &$asserts, $path) {
    $valid = is_scalar($data);
    $asserts[] = new XtoolsAssert($valid, "variable $path is of type " . $this->getLabel());

    return $valid ? $this : FALSE;
  }
}