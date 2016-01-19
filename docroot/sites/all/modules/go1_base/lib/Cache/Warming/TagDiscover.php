<?php
namespace Drupal\go1_base\Cache\Warming;

class TagDiscover {
  private $event_name;

  public function setEventName($event_name) {
    $this->event_name = $event_name;
  }

  public function tags() {
    foreach (go1_modules('go1_base', 'cache_warming') as $module) {
      if ($data = go1_config($module, 'cache_warming')->get('tags')) {
        if (isset($data[$this->event_name])) {
          return $data[$this->event_name];
        }
      }
    }

    return array();
  }
}
