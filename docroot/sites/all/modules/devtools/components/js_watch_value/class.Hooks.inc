<?php
namespace devtools\components\js_watch_value;

class Hooks extends \devtools\Hooks {
  /**
   * Extends hook_init().
   */
  public function hook_init() {
    devtools_add_js($this->_path . '/js/js_watch_value.js');
  }
}