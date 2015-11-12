<?php


/**
 * The class used for model entities
 */
class WoWToonFeedItem extends Entity {

  public function __construct($values = array()) {
    parent::__construct($values, 'wowtoon_feed_item');
  }

  protected function defaultLabel() {
    $text = strip_tags($this->description);
    return strlen($text) > 20?substr($text,0,20) . '...':$text;
  }

  protected function defaultUri() {
    return array('path' => 'activity_feed/' . $this->achid);
  }


}
