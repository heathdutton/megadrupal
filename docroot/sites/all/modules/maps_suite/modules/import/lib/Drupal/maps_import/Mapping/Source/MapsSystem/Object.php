<?php

/**
 * @file
 * Define a MaPS System® object.
 */

namespace Drupal\maps_import\Mapping\Source\MapsSystem;

/**
 * The MaPS System® Source object class.
 */
class Object extends Entity {

  /**
   * The medias related to the object.
   *
   * @var array
   */
  private $medias = array();

  /**
   * @inheritdoc
   */
  public function __construct(array $entity) {
    parent::__construct($entity);

    $this->medias = !empty($entity['medias']) ? $entity['medias'] : NULL;
    // @todo Clear the status and parent ids row values when adding a new ex
  }

  /**
   * Return the object code.
   *
   * @return string
   */
  public function getCode() {
    return $this->code;
  }

  /**
   * Return the object parent id.
   *
   * @return int
   */
  public function getObjectId() {
    return (int) $this->objectId;
  }

  /**
   * Return the medias related to the object.
   *
   * @return array
   */
  public function getMedias($type = NULL) {
    if (is_null($type)) {
    	return $this->medias;
    }

    if (empty($this->medias)) {
    	return array();
    }

    $medias = array();
  	foreach ($this->medias as $media) {
      if ($media['type'] == $type) {
        $medias[] = $media;
      }
  	}

  	return $medias;
  }
}
