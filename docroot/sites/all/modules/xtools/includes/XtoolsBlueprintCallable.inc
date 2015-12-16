<?php

/**
 * @file
 * Contains class XtoolsBlueprintCallable.
 */

/**
 * Describes data that is a callable.
 */
class XtoolsBlueprintCallable implements XtoolsBlueprintInterface {

  /**
   * Implements XtoolsBlueprint::getLabel().
   */
  function getLabel() {
    return 'callable';
  }

  /**
   * Implements XtoolsBlueprint::validate().
   */
  function validate($data, array &$asserts, $path) {
    $valid = is_callable($data);
    $asserts[] = new XtoolsAssert($valid, "variable $path is callable");
    if ($valid) {
      return $this;
    }
    return FALSE;
  }
}
