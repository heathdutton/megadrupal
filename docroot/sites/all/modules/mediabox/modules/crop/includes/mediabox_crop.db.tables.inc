<?php

/**
 * @file
 * Crop data for entites
 * This is not really bind to any entity - can be used more flexible
 */
class MediaboxCrop {

  public $id;
  public $entity_id;
  public $x;
  public $y;
  public $x1;
  public $y1;
  public $ratio;
  public $origin_image_style;
  public $height;
  public $width;
  public $created;
  public $modified;

  function __construct($cropData = NULL) {
    if (!empty($cropData)) {
      $this->populateFromArray($cropData);
      $this->id = NULL;
    }
  }
  
  public function populateFromClass($class) {
    if (!empty($class)) {
      foreach (get_class_vars('MediaboxCrop') as $key => $value) {
        if (property_exists($class, $key)) {
          $this->$key = $class->$key;
        }
      }
    }
  }

  public function populateFromArray($array) {
    if (!empty($array)) {
      foreach (get_class_vars('MediaboxCrop') as $key => $value) {
        if (key_exists($key, $array) && isset($array[$key])) {
          $this->$key = $array[$key];
        }
      }
    }
  }

  /**
   * Compare with passed crop with tolerance - rounding down to integer.
   * 
   * @param MediaboxCrop $crop
   * 
   * @return boolean TRUE if crops are equal, FALSE otherwise.
   */
  public function equal(MediaboxCrop $crop) {
    // Convert to int and round down. Rounding down is for now our tolerance.
    if (intval($this->x) != intval($crop->x) || intval($this->y) != intval($crop->y) ||
      intval($this->x1) != intval($crop->x1) || intval($this->y1) != intval($crop->y1)) {
      return FALSE;
    }
    return TRUE;
  }
}
