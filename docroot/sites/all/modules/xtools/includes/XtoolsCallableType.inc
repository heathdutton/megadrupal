<?php

abstract class XtoolsCallableType {

  /**
   * Groups of arguments the callable can accept when calling it. Every item
   * is an array with arguments, identical to the second argument of
   * call_user_func_array(). At least one item should be present.
   *
   * @var array
   */
  public $argument_groups = array(array());

  /**
   * This callable's blueprint. Use XtoolsBlueprintPlaceholder to refer to a signature
   * specified in a hook_xtools_blueprint_info() implementation.
   *
   * @var string
   * @var XtoolsBlueprint
   */
  public $blueprint = NULL;

   /**
    * The name of the module the hook belongs to. Use "drupal" as an alias for
    * all Drupal core modules.
    *
    * @var string
    */
   public $module = NULL;

  /**
   * This callable's signature. Use XtoolsSignaturePlaceholder to refer to a signature
   * specified in a hook_xtools_signature_info() implementation.
   *
   * @var XtoolsSignature
   */
  public $signature = NULL;

  /**
   * Constructor.
   *
   * @param array $properties
   *   Keys are property names, values are property values.
   */
  function __construct(array $properties = array()) {
    foreach ($properties as $property => $value) {
      $this->$property = $value;
    }
  }

  /**
   * Get all callables of this type for a certain module.
   *
   * @param string $module
   *   The name of the module that implements the callables.
   *
   * @return array
   *   An array of callables. See
   *   http://php.net/manual/language.types.callable.php.
   */
  function getCallables($module) {
 }
}