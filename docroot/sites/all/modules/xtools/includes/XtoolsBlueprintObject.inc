<?php

class XtoolsBlueprintObject extends XtoolsBlueprintPrimitive {

  /**
   * The name of the class or interface the value should be an instance of.
   *
   * @var string
   */
  public $instanceof = '';

  public $type = 'object';

  /**
   * Implements XtoolsBlueprint::getLabel().
   */
  function getLabel() {
    return $this->type . ($this->instanceof ? ' (' . $this->instanceof . ')' : '');
  }

  /**
   * An array of XtoolsBlueprintChildElement properties.
   *
   * @var array
   */
  public $object_properties = array();

  function __construct($instanceof = '', array $object_properties = array()) {
    $this->instanceof = $instanceof;
    $this->object_properties = $object_properties;
  }

  function validate($data, array &$asserts, $path) {
    // Check that the value is an object and that it is an instance of a
    // particular class or interface.
    $valid = (bool) parent::validate($data, $asserts, $path);
    if ($valid && $this->instanceof) {
      $valid = $data instanceof $this->instanceof;
      $asserts[] = new XtoolsAssert($valid, "object $path is an instance of $this->instanceof");
    }

    // Validate properties.
    if ($valid) {
      foreach ($this->object_properties as $property_name => $object_property) {
        $property_path = $path . '->' . $property_name;
        // If this property is required, test if it exists.
        if ($object_property->required) {
          $asserts[] = new XtoolsAssert(property_exists($data, $property_name), "required object property $property_path is set");
        }
        // If this property exists, test it.
        if (isset($data->{$property_name})) {
          $object_property->validate($data->{$property_name}, $asserts, $property_path);
        }
      }
    }

    return $valid ? $this : FALSE;
  }
}