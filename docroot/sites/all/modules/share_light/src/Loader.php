<?php

namespace Drupal\share_light;

class Loader {
  protected static $instance = NULL;

  public static function instance() {
    if (!static::$instance) {
      static::$instance = new static();
    }
    return static::$instance;
  }

  protected $channels;
  protected $channelStatus;

  public function __construct() {
    $this->channels = module_invoke_all('share_light_channel_info');
    $all_enabled = array_fill_keys(array_keys($this->channels), 1);
    $this->channelStatus = variable_get('share_light_channels_enabled', $all_enabled) + $all_enabled;
  }

  public function allChannels() {
    return $this->channels;
  }

  public function channelClass($name) {
    return $this->channels[$name];
  }

  public function channel($name, $block, $options) {
    $class = $this->channels[$name];
    return new $class($block, $options);
  }

  public function channelStatus($name) {
  }

  public function channelOptions() {
    $options = array();
    foreach ($this->channels as $name => $class) {
      if ($this->channelStatus[$name]) {
        $options[$name] = $class::title();
      }
    }
    return $options;
  }

  public function defaults() {
    $defaults = array();
    foreach ($this->channels as $name => $class) {
      $defaults[$name] = array('toggle' => 1) + $class::defaults();
    }
    return $defaults;
  }
}
