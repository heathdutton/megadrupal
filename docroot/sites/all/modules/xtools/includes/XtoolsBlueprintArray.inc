<?php

class XtoolsBlueprintArray extends XtoolsBlueprintPrimitive {

  /**
   * An array of XtoolsBlueprintChildElement objects that should match certain
   * array keys. The XtoolsBlueprintChildElement::name properties should match
   * the keys of the arrays that are to be validated.
   *
   * @var array
   */
  public $blueprints_named = array();

  /**
   * Either "string" or "integer", in case the array keys need to be of a
   * certain type, or NULL if the keys can be of either type.
   *
   * @var string
   */
  public $key_type = NULL;

  /**
   * An XtoolsBlueprintOr object to match values against unnamed array item
   * blueprints.
   *
   * @var XtoolsBlueprintOr
   */
  public $or = NULL;

  public $type = 'array';

  /**
   * Constructor.
   *
   * @param array $blueprints_named
   *   An array with XtoolsBlueprintChildElement objects. Keys correspond to
   *   the keys of the data's array items the blueprints should match
   *   (sometimes informally known as 'associative' array (items)).
   * @param array $blueprints_unnamed
   *   An array with XtoolsBlueprintInterface objects. Every array item of the
   *   data being validated that should not already match a named blueprint,
   *   will be validated against all unnamed blueprints.
   * @param boolean|null $key_type
   *   Either "string" or "integer" to indicate that array keys should be of
   *   this type, or NULL so skip key validation.
   */
  function __construct(array $blueprints_named = array(), array $blueprints_unnamed = array(), $key_type = NULL) {
    $this->blueprints_named = $blueprints_named;
    $this->or = new XtoolsBlueprintOr($blueprints_unnamed);
    $this->key_type = $key_type;
  }

  /**
   * Implements XtoolsBlueprint::validate().
   *
   * @param array $data
   */
  function validate($data, array &$asserts, $path) {
    if ($return = parent::validate($data, $asserts, $path)) {
      // Test required key type.
      if ($this->key_type) {
        foreach (array_keys($data) as $key) {
          $element_path = $path . (is_string($key) ? "['$key']" : "[$key]");
          $asserts[] = new XtoolsAssert(gettype($key) == $this->key_type, "array key $element_path is of type $this->key_type.");
        }
      }

      // Validate 'associative' array elements.
      foreach ($this->blueprints_named as $key => $blueprint) {
        $element_path = $path . (is_string($key) ? "['$key']" : "[$key]");
        if ($blueprint->required) {
          $asserts[] = new XtoolsAssert(array_key_exists($key, $data), "required array element $element_path is set.");
        }
        if (array_key_exists($key, $data)) {
          $blueprint->validate($data[$key], $asserts, $element_path);
        }
      }

      // Validate 'non-associative' array elements.
      if ($this->or->allowed_blueprints) {
        foreach (array_diff_key($data, $this->blueprints_named) as $key => $element) {
          $element_path = $path . (is_string($key) ? "['$key']" : "[$key]");
          $this->or->validate($element, $asserts, $element_path);
        }
      }
    }

    return $return;
  }
}