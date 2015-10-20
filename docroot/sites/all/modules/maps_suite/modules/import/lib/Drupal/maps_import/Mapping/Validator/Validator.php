<?php

/**
 * @file
 * Abstract class for the different type of mapping validators.
 */

namespace Drupal\maps_import\Mapping\Validator;

use Drupal\maps_import\Mapping\Source\MapsSystem\PropertyWrapper;
use Drupal\maps_import\Mapping\Target\Drupal\Field\Field;

abstract class Validator implements ValidatorInterface {

	/**
	 * The success return value.
	 */
  const VALIDATOR_SUCCESS = 2;

  /**
   * The warning return value.
   */
  const VALIDATOR_WARNING = 1;

  /**
   * The error return value.
   */
  const VALIDATOR_ERROR = 0;

  /**
   * The MaPS System® property to test.
   *
   * @var PropertyWrapper
   */
	protected $property;

	/**
   * The Drupal field to test.
   *
   * @var Field
	 */
	protected $field;

	/**
	 * The class constructor.
	 *
	 * @param $property
	 * @param $field
	 */
	public function __construct(PropertyWrapper $property, Field $field) {
		$this->property = $property;
		$this->field = $field;
	}

	/**
	 * Return the MaPS System® property type code.
   *
   * @return string
   *   The MaPS System® property type code.
	 */
	public function getMapsType() {
		return $this->property->getTypeCode();
	}

	/**
	 * Return the Drupal field type.
	 *
	 * @return string
	 *   The Drupal field type.
	 */
	public function getDrupalType() {
		return $this->field->getType();
	}
  
}
