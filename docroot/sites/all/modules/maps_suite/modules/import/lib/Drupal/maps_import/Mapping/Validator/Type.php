<?php

/**
 * @file
 *
 * File for the type validator.
 */

namespace Drupal\maps_import\Mapping\Validator;

class Type extends Validator {

	/**
	 * Defines the default valid Drupal field for the MaPS System® type string.
	 */
  public static $string = array(
    'text',
    'text_long',
    'text_with_summary',
    'list_text',
  );

  /**
   * Defines the default valid Drupal field for the MaPS System® type bool.
   */
  public static $bool = array(
    'list_boolean',
  );

  /**
   * Defines the default valid Drupal field for the MaPS System® type int.
   */
  public static $int = array(
    'number_decimal',
    'number_float',
    'number_integer',
    'list_float',
    'list_integer',
  );

  /**
   * Defines the default valid Drupal field for the MaPS System® type text.
   */
  public static $text = array(
    'text',
    'text_long',
    'text_with_summary',
    'list_text',
  );

  /**
   * Defines the default valid Drupal field for the MaPS System® type date.
   */
  public static $date = array(
    'text',
    'text_long',
    'text_with_summary',
    'list_text',
  );

  /**
   * Defines the default valid Drupal field for the MaPS System® type float.
   */
  public static $float = array(
    'number_decimal',
    'number_float',
    'number_integer',
    'list_float',
    'list_integer',
  );

  /**
   * Defines the default valid Drupal field for the MaPS System® type library.
   */
  public static $library = array(
    'taxonomy_term_reference',
  );

  /**
   * Defines the default valid Drupal field for the MaPS System® type html.
   */
  public static $html = array(
    'text',
    'text_long',
    'text_with_summary',
    'list_text',
  );

  /**
   * Defines the default valid Drupal field for the MaPS System® type object.
   */
  public static $object = array(
    'taxonomy_term_reference',
  );

  /**
   * Return available MaPS System® types.
   *
   * @return array
   *   The available MaPS System® types.
   */
  public static function getMapsTypes() {
    return array('bool', 'string', 'int', 'text', 'date', 'float', 'library', 'html', 'object');
  }

  /**
   * Return available Drupal types.
   *
   * @return array
   *   The available Drupal types.
   */
  public static function getDrupalTypes() {
  	return array_keys(module_invoke_all('field_info'));
  }

  /**
   * Get the corresponding valid Drupal types for the given MaPS System® type.
   *
   * @param $mapsType
   *   The MaPS System® type.
   *
   * @return array
   *   The valid Drupal types
   */
  public static function getCriterium($mapsType) {
    $types = get_class_vars('Drupal\\maps_import\\Mapping\\Validator\\Type');
    return variable_get('maps_import:validators:' . strtolower($mapsType), $types[$mapsType]);
  }

  /**
   * Set the corresponding valid Drupal types for the given MaPS System® type.
   *
   * @param $mapsType
   *   The MaPS System® type.
   * @param criterium
   *   The valid Drupal types
   */
  public static function setCriterium($mapsType, $criterium) {
    return variable_set('maps_import:validators:' . strtolower($mapsType), $criterium);
  }

  /**
   * @inheritdoc
   */
  public function validate() {
    if (!in_array($this->getDrupalType(), $this->getDrupalTypes())) {
    	return self::VALIDATOR_WARNING;
    }

    $types = self::getCriterium($this->getMapsType());
    $types = array_combine($types, $types);

  	return isset($types[strtolower($this->getDrupalType())]) ? self::VALIDATOR_SUCCESS : self::VALIDATOR_ERROR;
  }

}
