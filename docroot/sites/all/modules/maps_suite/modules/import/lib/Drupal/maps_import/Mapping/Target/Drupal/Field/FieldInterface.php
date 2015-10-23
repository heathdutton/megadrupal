<?php

/**
 * @file
 * Interface that defines drupal field.
 */

namespace Drupal\maps_import\Mapping\Target\Drupal\Field;

use Drupal\maps_import\Converter\ConverterInterface;

interface FieldInterface {

  /**
   * The field constructor.
   *
   * @param $definition
   *   The field definition.
   */
  public function __construct(array $definition);

  /**
   * Return the field name.
   */
  public function getName();

  /**
   * Return the field label.
   *
   * @return string
   */
  public function getLabel();

  /**
   * Get the field storage key.
   *
   * @return string
   */
  public function getKey();

  /**
   * Get the field type.
   *
   * @return string
   */
  public function getType();

  /**
   * Get the field identifier.
   *
   * @return string
   */
  public function getId();

  /**
   * Get wether the fieldis translatable.
   *
   * @return boolean
   */
  public function isTranslatable();

  /**
   * Get wether the field may have multiple values.
   *
   * @return boolean
   */
  public function isMultiple();

  /**
   * Load a field from its name.
   *
   * @param $converter
   *   The related converter instance.
   * @param $name
   *   The field name.
   *
   * @return DefaultField
   */
  public static function load(ConverterInterface $converter, $name);

  /**
   * Get the property description.
   *
   * @param granularity
   *   A list of the settings to display.
   * @param $separator
   *   The string used to separate each requested info.
   */
  public function getDescription(array $granularity, $separator = ' - ');

  /**
   * Format and sanitize the values for corresponding to the field.
   *
   * @param $values
   *   The values to sanitize.
   *
   * @return mixed
   *   The sanitized value
   */
  public function sanitize($values);

  /**
   * Whether the field has options.
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
   * Return the field cardinality.
   *
   * @return int
   */
  public function getCardinality();

  /**
   * Process some functionnalities after the entity has been saved.
   *
   * @param ConverterInterface $converter
   *   The related converter.
   * @param $mapsEntity
   *   The MaPS System entity.
   * @param $drupalEntity
   *   The Drupal entity.
   *
   * @return bool
   */
  public function postProcess(ConverterInterface $converter, $mapsEntity, $drupalEntity);

}
