<?php


/**
 * The class used for avatar type entities
 */
class AvatarType extends Entity {

  public $type;
  public $label;

  public function __construct($values = array()) {
    parent::__construct($values, 'avatar_type');
  }

}
