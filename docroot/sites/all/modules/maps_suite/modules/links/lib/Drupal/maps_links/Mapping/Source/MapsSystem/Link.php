<?php

/**
 * @file
 *
 * Defines a MaPS System® link.
 */

namespace Drupal\maps_links\Mapping\Source\MapsSystem;

use Drupal\maps_import\Mapping\Source\MapsSystem\Entity;

/**
 * The MaPS System® Source link class.
 */
class Link extends Entity {

  /**
   * The source object.
   */
	private $source;

	/**
	 * The target object.
	 */
	private $target;

	/**
	 * The link's count.
	 */
	private $count;

	/**
	 * The link's type
	 */
	private $linkType;

  /**
   * @inheritdoc
   */
  public function __construct(array $entity) {
    parent::__construct($entity);
    $this->source = isset($entity['source_id']) ? $entity['source_id'] : NULL;
    $this->target = isset($entity['target_id']) ? $entity['target_id'] : NULL;
    $this->count = isset($entity['count']) ? $entity['count'] : 1;
    $this->linkType = isset($entity['type_id']) ? $entity['type_id'] : NULL;
  }

  /**
   * Get the source object id.
   *
   * @return int
   */
  public function getSource() {
  	return (int) $this->source;
  }

  /**
   * Get the target object id.
   *
   * @return int
   */
  public function getTarget() {
  	return (int) $this->target;
  }

  /**
   * Get the number of link.
   *
   * @return int
   */
  public function getCount() {
  	return (int) $this->count;
  }

  /**
   * Get the link type id
   *
   * @return int
   */
  public function getLinkType() {
  	return (int) $this->linkType;
  }
}
