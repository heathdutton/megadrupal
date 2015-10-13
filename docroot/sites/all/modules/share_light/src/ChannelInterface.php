<?php

namespace Drupal\share_light;

interface ChannelInterface {
  public function __construct($block, $options);
  public static function title();
  public static function defaults();
  public static function optionsWidget(array &$element, array $options);
  public function enabled();
  public function render();
}
