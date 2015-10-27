<?php

class XtoolsSignature {

  /**
   * An array of XtoolsSignatureParameter objects that describe the callable's
   * parameters.
   *
   * @var array
   */
  public $parameters = array();

  /**
   * Constructor.
   *
   * @param array $parameters
   *   See $this->parameters for details.
   */
  function __construct(array $parameters) {
    $this->parameters = $parameters;
  }

  /**
   * Validate a callable's signature.
   *
   * @param callable $callable
   *   See http://php.net/manual/language.types.callable.php for a list of
   *   values that are callable.
   * @param array $asserts
   *   An array in which to store XtoolsAssert objects that describe the
   *   validation result.
   *
   * @return false|XtoolsSignature
   *   An XtoolsSignature object (usually $this) if the callable is valid.
   *   FALSE if it isn't.
   */
  function validate($callable, array &$asserts = array()) {
    $valid = TRUE;
    $reflection = xtools_callable_reflection($callable);

    // Check $callable really is a callable.
    if (!$reflection) {
      $asserts[] = new XtoolsAssert(FALSE, "The tested value is no callable.");
      return FALSE;
    }

    // Check the parameter count.
    $reflection_parameters = $reflection->getParameters();
    $parameters_message = count($this->parameters);
    if (!($valid_count = count($reflection_parameters) >= count($this->parameters))) {
      $valid = FALSE;
    }
    $asserts[] = new XtoolsAssert($valid_count, "The callable accepts the required number of parameters ($parameters_message)");

    // Validate the parameters.
    foreach ($this->parameters as $i => $parameter) {
      if (isset($reflection_parameters[$i])) {
        if (!$parameter->validate($reflection_parameters[$i], $asserts)) {
          $valid = FALSE;
        }
      }
    }

    return $valid ? $this : FALSE;
  }
}