<?php 

namespace Drupal\maps_links\Converter;

use Drupal\maps_import\Converter\ConverterInterface;

interface LinkInterface extends ConverterInterface {
  
  /**
   * Get the link type.
   *
   * @return string
   *   The link type.
   */	
	public function getLinkType();
	  
	/**
   * Set the link type.
   *
   * @param $linkType
   *   The link type.
   */
	public function setLinkType($linkType);
}