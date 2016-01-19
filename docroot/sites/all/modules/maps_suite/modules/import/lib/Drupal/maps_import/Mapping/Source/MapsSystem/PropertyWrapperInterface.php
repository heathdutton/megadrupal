<?php

/**
 * @file
 * Interface for PropertyWrapper class.
 */

namespace Drupal\maps_import\Mapping\Source\MapsSystem;

use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Profile\Profile;

/**
 * The MaPS System® Property Wrapper interfarce.
 */
interface PropertyWrapperInterface {

  /**
   * The class constructor.
   *
   * @param $definition
   *   The MaPS System® property definition.
   */
  public function __construct(array $definition = array());

  /**
   * Get the property id.
   *
   * @return string
   */
  public function getId();

  /**
   * Get the property/attribute storage key.
   *
   * @return string
   */
  public function getKey();

  /**
   * Get the property/attribute label.
   *
   * @return string
   */
  public function getLabel();

  /**
   * Get the property/attribute label, as it has to be displayed in select.
   *
   * @return string
   */
  public function getSelectLabel();


  /**
   * Get the property description.
   *
   * @param granularity
   *   A list of the settings to display.
   * @param $separator
   *   The string used to separate each requested info.
   *
   * @return string.
   */
  public function getDescription(array $granularity, $separator = ' - ');

  /**
   * Get the group label this property belongs to.
   *
   * @return string
   */
  public function getGroupLabel();

  /**
   * Get the translated property title.
   *
   * @return string
   */
  public function getTranslatedTitle();

  /**
   * Whether the property is translatable.
   *
   * @return bool
   */
  public function isTranslatable();

  /**
   * Whether the property is multiple.
   *
   * @return bool
   */
  public function isMultiple();

  /**
   * Load the given property or attribute.
   *
   * @param $converter
   *   The related converter.
   * @param $id
   *   The property / attribute id.
   *
   * @return PropertyWrapperInterface
   */
  public static function load(ConverterInterface $converter, $id);

  /**
   * Extract values of the current property from a given MaPS System® entity.
   * Used to populate entity's raw values.
   *
   * @param $entity
   *   The related MaPS System® entity.
   * @param array $options
   *   Some specific options that are defined in the mapping UI.
   * @param ConverterInterface $currentConverter
   *   The current converter (some knid of context).
   *
   * @return array
   *   The values.
   */
  public function extractValues(EntityInterface $entity, $options = array(), ConverterInterface $currentConverter);

  /**
   * Return values of the current property from the entity's raw values.
   *
   * @param $entity
   *   The related MaPS System® entity.
   *
   * @return array
   *   The values.
   */
  public function getValues(EntityInterface $entity);

  /**
   * Add a specific handling in function of the type of property.
   */
  public function processValues($values, EntityInterface $entity, ConverterInterface $currentConverter);

  /**
   * Set the property options.
   *
   * @param array $options
   */
  public function setOptions(array $options);

  /**
   * Get the property options.
   *
   * @return array
   */
  public function getOptions();

  /**
   * Whether the property has options.
   *
   * @return boolean
   */
  public function hasOptions();

  /**
   * Return the default options.
   *
   * @return array
   */
  public function getOptionsDefault();

  /**
   * Format and sanitize the values for corresponding to the property.
   *
   * @param $values
   *   The values to sanitize.
   *
   * @return mixed
   *   The sanitized value
   */
  public function sanitize($values);

  /**
   * Check if the property / attribute still exists.
   *
   * @param Profile $profile
   *   The related profile.
   *
   * @return boolean
   *   Whether the property exists.
   */
  public function exists(Profile $profile);

}
