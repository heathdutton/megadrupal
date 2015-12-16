<?php

/**
 * Represents any signature defined in a hook_xtools_signature_info() hook
 * implementation. Use this when you cannot get a signature through
 * xtools_signature_info().
 */
class XtoolsSignaturePlaceholder extends XtoolsSignature {

  /**
   * The machine name of the represented signature.
   *
   * @var string
   */
  public $signature_name = NULL;

  /**
   * Constructor.
   *
   * @param string $signature_name
   *   The machine name of the signature as defined in a
   *   hook_xtools_signature_info() implementation.
   */
  function __construct($signature_name) {
    $this->signature_name = $signature_name;
  }

  /**
   * Implements XtoolsSignature::validate().
   */
  function validate($callable, array &$asserts = array()) {
    return $this->signature()->validate($callable, $asserts);
  }

  /**
   * Load the signature.
   *
   * @throws Exception
   *
   * @return XtoolsSignature
   */
  function signature () {
    if ($signature = xtools_signature_info($this->signature_name)) {
      return $signature;
    }
    throw new Exception(t('Signature !signature_name could not be loaded by xtools_signature_info().', array(
      '!signature_name' => $this->signature_name,
    )));
  }
}