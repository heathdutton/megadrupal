<?php

/**
 * @file
 * Broken attribute class.
 */

namespace Drupal\maps_import\Mapping\Source\MapsSystem\Attribute;

use Drupal\maps_import\Profile\Profile;

class Broken extends Attribute {
  
	/**
   * @inheritdoc
	 */
	public function exists(Profile $profile) {
		return FALSE;
	}
	
}
