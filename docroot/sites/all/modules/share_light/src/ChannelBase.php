<?php

namespace Drupal\share_light;

abstract class ChannelBase implements ChannelInterface {
  protected $block;
  protected $options;
  public function __construct($block, $options = array()) {
    $this->block = $block;
    $this->options = $options + static::defaults();
  }
  public static function defaults() {
    return array('toggle' => 1);
  }
  public static function optionsWidget(array &$element, array $options) {
    return array();
  }
  public function enabled() {
    return !empty($this->options['toggle']);
  }
}
