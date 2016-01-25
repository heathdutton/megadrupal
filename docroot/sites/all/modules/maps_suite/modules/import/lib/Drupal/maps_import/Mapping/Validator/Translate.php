<?php

/**
 * @file 
 * 
 * File for the translate validator.
 */

namespace Drupal\maps_import\Mapping\Validator;

class Translate extends Validator { 

	/**
	 * @inheritdoc
	 */
	public function validate() {
		return $this->property->isTranslatable() && !$this->field->isTranslatable() ? self::VALIDATOR_WARNING : self::VALIDATOR_SUCCESS; 
	}
	
}
