<?php

namespace Drupal\maps_import\Mapping\Target\Drupal\Field;

/**
 * @file 
 * Custom management of the commerce_price field.
 */

class CommercePrice extends DefaultField {
	
	/**
	 * @inheritdoc 
	 */
	public function sanitize($values) {
	  foreach ($values as &$value) {
	  	$value = array(
	  	  'amount' => (float) $value * 100,
	  	  'currency_code' => commerce_default_currency(),
	    );
	  }

	  return $values;
	}
}