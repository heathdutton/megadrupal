<?php

/**
 * @file
 * 
 * File for the multiple validator.
 */

namespace Drupal\maps_import\Mapping\Validator;

class Multiple extends Validator { 

	/**
	 * Code returned if the drupal field is not multiple. 
	 */
	const MULTIPLE_VALIDATOR_DRUPAL_NOT_MULTIPLE = 0;
	
  /**
   * Code returned if the drupal field is multiple but not infinite. 
   */
	const MULTIPLE_VALIDATOR_DRUPAL_NOT_INFINITE = 1;
	
  /**
   * Code returned if the validation is successfull. 
   */
	const MULTIPLE_VALIDATOR_DRUPAL_SUCCESS = 2;
	
	/**
	 * @inheritdoc
	 */
  public function validate() {
  	$cardinality = $this->field->getCardinality();
    if ($this->property->isMultiple()) {
      if (!$this->field->isMultiple()) {
      	return self::MULTIPLE_VALIDATOR_DRUPAL_NOT_MULTIPLE;
      }
      
      if ($this->field->getCardinality() != FIELD_CARDINALITY_UNLIMITED) {
      	return self::MULTIPLE_VALIDATOR_DRUPAL_NOT_INFINITE;
      }
    }
    
    return self::MULTIPLE_VALIDATOR_DRUPAL_SUCCESS;
  }
  
}
