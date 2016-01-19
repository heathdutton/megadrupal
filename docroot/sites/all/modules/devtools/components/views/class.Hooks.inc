<?php
namespace devtools\components\views;

class Hooks extends \devtools\Hooks {
  /**
   * Extends hook_init().
   */
  public function hook_init() {
    devtools_add_css($this->_path . '/css/views.css');
  }
}