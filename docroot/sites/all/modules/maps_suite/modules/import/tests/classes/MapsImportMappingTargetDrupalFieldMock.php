<?php 

/**
 * @file
 * 
 * Describe the Mock object related to the mapping Drupal's field class.
 */

use Drupal\maps_import\Mapping\Target\Drupal\Field\DefaultField;

class DefaultFieldMock extends DefaultField {
	
	/**
	 * Defines the field cardinality.
	 */
	private $cardinality = 1;
	
	/**
	 * Set the cardinality. 
	 */
	public function setCardinality($cardinality) {
		$this->cardinality = $cardinality;
	}
	
	/**
	 * @inheritdoc
	 */
	public function getCardinality() {
		return $this->cardinality;
	}
	
}
