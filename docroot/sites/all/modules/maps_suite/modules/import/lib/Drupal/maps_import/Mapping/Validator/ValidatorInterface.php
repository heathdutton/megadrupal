<?php

/**
 * @file
 *
 * Interface for validator classes.
 */

namespace Drupal\maps_import\Mapping\Validator;

interface ValidatorInterface {
 
	/**
	 * Check if the mapping is valid, and return the appropriate code.
	 * 
	 * @return int 
	 *   The error code.
	 */
	public function validate();
	
}
