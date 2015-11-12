<?php


/**
 * The class used for model type entities
 */
class WoWToonFeedItemType extends Entity {

  public $type;
  public $label;

  public function __construct($values = array()) {
    parent::__construct($values, 'wowtoon_feed_item_type');
  }
}
