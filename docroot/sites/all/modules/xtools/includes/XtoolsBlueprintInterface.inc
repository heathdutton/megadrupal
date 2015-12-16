<?php

interface XtoolsBlueprintInterface {

  /**
   * Get this blueprint's human-readable variable type/label.
   *
   * @return string
   */
  function getLabel();

  /**
   * Test if a variable matches this blueprint.
   *
   * @param mixed $data
   *   The data to validate.
   * @param array $asserts
   *   An array to fill with XtoolsAssert objects.
   * @param mixed $path
   *   A variable name with which the data can be accessed in the code, such
   *   as "$machine_name", or "$foo->items[0]['title']".
   *
   * @return false|XtoolsBlueprint
   *   An XtoolsBlueprintInterface object (usually $this) if the value is
   *   valid. FALSE if it isn't.
   */
  function validate($data, array &$asserts, $path);
}