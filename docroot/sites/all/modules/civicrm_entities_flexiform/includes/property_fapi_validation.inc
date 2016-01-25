<?php

/**
 * Handles converting validation rules from the property_validation module to
 * the fapi_validation module.
 *
 * Class PropertyFapiValidation
 */
class PropertyFapiValidation {

  public $entityType;
  public $bundle;
  public $propertyName;

  function __construct($entity_type, $bundle, $property_name) {
    $this->entityType = $entity_type;
    $this->bundle = $bundle;
    $this->propertyName = $property_name;
  }

  /**
   * Adds validation rules from the property_validation module for the provided form element.
   * @param array $element The FAPI element to apply the property validation rules to.
   * @return false|array Returns the updated $element.
   */
  public function addPropertyValidation($element) {
    $rules = self::getRuleSets($this->entityType, $this->bundle);
    $element_rules = self::getValidationRulesForElement($rules);
    foreach($element_rules as $name => $rule) {
      // Special case for "required".
      if($rule->type == 'required') {
        $element['#required'] = true;
        continue;
      }
      // Add the rules in the fapi_validation module's expected format.
      $element['#rules'][] = array(
        'rule' => $rule->fapi, 'error' => $rule->error_message,
      );
    }
    return $element;
  }

  /**
   * Gets the property_validation rules for a entity and bundle.
   * Uses static caching for repeat calls.
   *
   * @param string $entity_type
   * @param string $bundle
   * @return null|array Returns an array of validation rules keyed on the rule's machine name.
   */
  private static function getRuleSets($entity_type, $bundle) {
    $rule_key = $entity_type . "_" . $bundle;
    // Do some static caching as this will get called for the same entity and bundle often.
    static $rule_sets;
    if (!$rule_sets) {
      $rule_sets = array();
    }
    // If we haven't retrieved any rules for this entity and bundle yet, do so now.
    if (!array_key_exists($rule_key)) {
      $rule_sets[$rule_key] = ctools_export_load_object(
        'property_validation_rule',
        'conditions',
        array('entity_type' => $entity_type, 'bundle' => $bundle));
    }

    // Check if we have any rules recently retrieved or from cache.
    if (!empty($rule_sets[$rule_key])) {
      return $rule_sets[$rule_key];
    }

    return null;
  }

  /**
   * Returns an array of rules specific to the provided element.
   *
   * @param array $rule_sets An array of property_validation rules.
   * @return array Array of rules keyed on the rule machine name.
   */
  private function getValidationRulesForElement($rule_sets) {
    $element_rules = array();

    foreach($rule_sets as $rule_name => $rule) {

      // Is this rule for our element? Check based on the entity and property.
      if($rule->entity_type == $this->entityType
        && $rule->bundle == $this->bundle
        && $rule->property_name == $this->propertyName) {

        self::translateToFapiRule($rule);
        $element_rules[$rule_name] = $rule;
      }
    }

    return $element_rules;
  }

  private static function translateToFapiRule($rule) {
    $rule->type = self::translateRuleName($rule->validator);
    $rule->fapi = "";

    // Handle the translation of property_validation to FAPI validation rules.
    switch ($rule->type) {
      case 'length':
        $rule->fapi = $rule->type . "[" . $rule->settings['min'] . "," . $rule->settings['max'] . "]";
        break;
      case 'regexp':
        $rule->fapi = $rule->type . "[/" . $rule->settings['data'] . "/]";
        break;
    }
  }


  /**
   * Converts a property_validation rule name to a fapi_validation rule name.
   *
   * @param string $validator_name
   * @return null|string
   */
  private static function translateRuleName($validator_name) {
    $matches = array();
    $found = preg_match('/property_validation_(.*?)_validator/', $validator_name, $matches);

    if(!$found) {
      return null;
    }

    $validator_type = $matches[1];

    $translation_table = array(
      'regex' => 'regexp',
      'email' => 'email',
      'length' => 'length',
      'match_field' => 'match_field',
    );

    if(array_key_exists($validator_type, $translation_table)) {
      return $translation_table[$validator_type];
    }

    return $validator_type;
  }

}
