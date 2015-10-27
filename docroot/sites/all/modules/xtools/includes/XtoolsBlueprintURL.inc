<?php

class XtoolsBlueprintURL extends XtoolsBlueprintString {

  /**
   * Whether the URL must be absolute.
   *
   * TRUE if the URL must be absolute, FALSE if it must be relative or NULL if
   * it can be either.
   *
   * @var boolean
   */
  public $absolute = NULL;

  /**
   * Constructor.
   *
   * @param boolean $absolute
   *   TRUE if the URL must be absolute, FALSE if it must be relative or NULL
   *   if it can be either.
   */
  function __construct($absolute = NULL) {
    $this->absolute = $absolute;
  }

  /**
   * Implements XtoolsBlueprint::validate().
   */
  function validate($data, array &$asserts, $path) {
    // Validate that the value is a string.
    $valid = (bool) parent::validate($data, $asserts, $path);

    if ($valid) {
      // Validate the value as either an absolute or relative URL.
      if (is_bool($this->absolute)) {
        $valid = valid_url($data, $this->absolute);
        $absolute_message = $this->absolute ? 'an absolute' : 'a relative';
        $asserts[] = new XtoolsAssert($valid, "variable $path is $absolute_message URL");
      }
      // Validate the value as any kind of URL.
      else {
        $valid = valid_url($data) || valid_url($data, TRUE);
        $asserts[] = new XtoolsAssert($valid, "variable $path is a URL");
      }
    }

    return $valid ? $this : FALSE;
  }
}